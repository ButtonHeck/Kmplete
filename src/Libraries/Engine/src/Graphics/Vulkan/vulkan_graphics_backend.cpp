#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Version/kmplete_version.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <cstring>
#include <stdexcept>

#if defined (KMP_CONFIG_TYPE_PRODUCTION)
    #define KMP_ENABLE_VULKAN_VALIDATION_LAYER false
#else
    #define KMP_ENABLE_VULKAN_VALIDATION_LAYER true
#endif


namespace Kmplete
{
    static constexpr auto ValidationLayerName = "VK_LAYER_KHRONOS_validation";
    static const Vector<const char*> LayerNames = { ValidationLayerName };

    namespace Graphics
    {
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            KMP_MB_UNUSED VkDebugUtilsMessageTypeFlagsEXT messageType,
            KMP_MB_UNUSED const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
            KMP_MB_UNUSED void* userData)
        {
            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
            {
                KMP_LOG_DEBUG_FN("VulkanGraphicsBackend DebugCallback: {}", callbackData->pMessage);
            }
            else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
            {
                KMP_LOG_INFO_FN("VulkanGraphicsBackend DebugCallback: {}", callbackData->pMessage);
            }
            else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            {
                KMP_LOG_WARN_FN("VulkanGraphicsBackend DebugCallback: {}", callbackData->pMessage);
            }
            else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            {
                KMP_LOG_ERROR_FN("VulkanGraphicsBackend DebugCallback: {}", callbackData->pMessage);
            }

            return VK_FALSE;
        }
        //--------------------------------------------------------------------------

        static VkDebugUtilsMessengerCreateInfoEXT CreateDebugMessengerCreateInfo()
        {
            auto debugMessengerCreateInfo = VulkanUtils::GetVkDebugUtilsMessengerCreateInfo();
            debugMessengerCreateInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugMessengerCreateInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugMessengerCreateInfo.pfnUserCallback = DebugCallback;

            return debugMessengerCreateInfo;
        }
        //--------------------------------------------------------------------------

        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger)
        {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                auto debugMessengerCreateInfo = CreateDebugMessengerCreateInfo();
                return func(instance, &debugMessengerCreateInfo, allocator, debugMessenger);
            }
            else
            {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }
        //--------------------------------------------------------------------------

        static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator)
        {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                func(instance, debugMessenger, allocator);
            }
        }
        //--------------------------------------------------------------------------


        VulkanGraphicsBackend::VulkanGraphicsBackend(Window& window)
            : GraphicsBackend(window)
            , _instance(VK_NULL_HANDLE)
            , _debugMessenger(VK_NULL_HANDLE)
            , _currentBufferIndex(0)
        {
            _Initialize();
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsBackend::~VulkanGraphicsBackend()
        {
            _Finalize();
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::StartFrame(float frameTimestep)
        {
            _physicalDevice->StartFrame(frameTimestep);
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::EndFrame()
        {
            _physicalDevice->EndFrame();

            _currentBufferIndex = (_currentBufferIndex + 1) % NumConcurrentFrames;
        }
        //--------------------------------------------------------------------------

        VkInstance VulkanGraphicsBackend::GetVkInstance() const noexcept
        {
            return _instance;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_Initialize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
            {
                if (!_CheckValidationLayerSupport())
                {
                    KMP_LOG_CRITICAL("{} layer requested but not found", ValidationLayerName);
                    throw std::runtime_error("VulkanGraphicsBackend: failed to initialize");
                }
            }

            VkApplicationInfo applicationInfo = _CreateApplicationInfo();
            auto extensionsNames = _GetRequiredExtensionsNames();
            VkInstanceCreateInfo instanceCreateInfo = _CreateInstanceCreateInfo(applicationInfo, extensionsNames);

            auto debugMessengerCreateInfo = CreateDebugMessengerCreateInfo();
            _AttachDebugMessengerInfo(instanceCreateInfo, debugMessengerCreateInfo);

            _PrintAvailableExtensions();

            const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &_instance);
            VulkanUtils::CheckResult(result, "VulkanGraphicsBackend: failed to create VkInstance");

            _InitializeDebugMessenger();

            _surface.reset(new VulkanGraphicsSurface(_window, _instance));
            _physicalDevice.reset(new VulkanPhysicalDevice(_window, _currentBufferIndex, _instance, dynamic_cast<VulkanGraphicsSurface*>(_surface.get())->GetVkSurface()));
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_Finalize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
            {
                DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
            }

            _physicalDevice.reset();
            _surface.reset();

            vkDestroyInstance(_instance, nullptr);
        }
        //--------------------------------------------------------------------------

        bool VulkanGraphicsBackend::_CheckValidationLayerSupport() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            UInt32 layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            Vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            KMP_LOG_DEBUG("available layers:");
            for (const auto& layer : availableLayers)
            {
                KMP_MB_UNUSED const auto layerVersionMajor = VK_API_VERSION_MAJOR(layer.specVersion);
                KMP_MB_UNUSED const auto layerVersionMinor = VK_API_VERSION_MINOR(layer.specVersion);
                KMP_MB_UNUSED const auto layerVersionPatch = VK_API_VERSION_PATCH(layer.specVersion);
                KMP_LOG_DEBUG("\t{} ({}), Spec version: {}.{}.{}, Impl. version: {}", 
                    layer.layerName, layer.description, 
                    layerVersionMajor, layerVersionMinor, layerVersionPatch, layer.implementationVersion
                );

                if (std::strcmp(ValidationLayerName, layer.layerName) == 0)
                {
                    return true;
                }
            }

            return false;
        }
        //--------------------------------------------------------------------------

        VkApplicationInfo VulkanGraphicsBackend::_CreateApplicationInfo() const
        {
            auto applicationInfo = VulkanUtils::GetVkApplicationInfo();
            applicationInfo.pApplicationName = "Kmplete Application";
            applicationInfo.applicationVersion = VK_MAKE_VERSION(GetKmpleteVersionMajor(), GetKmpleteVersionMinor(), GetKmpleteVersionPatch());
            applicationInfo.pEngineName = "Kmplete engine";
            applicationInfo.engineVersion = VK_MAKE_VERSION(GetKmpleteVersionMajor(), GetKmpleteVersionMinor(), GetKmpleteVersionPatch());
            applicationInfo.apiVersion = VK_API_VERSION_1_4;

            return applicationInfo;
        }
        //--------------------------------------------------------------------------

        VkInstanceCreateInfo VulkanGraphicsBackend::_CreateInstanceCreateInfo(const VkApplicationInfo& applicationInfo, Vector<const char*>& extensionsNames) const
        {
            auto instanceCreateInfo = VulkanUtils::GetVkInstanceCreateInfo();
            instanceCreateInfo.pApplicationInfo = &applicationInfo;
            instanceCreateInfo.enabledExtensionCount = UInt32(extensionsNames.size());
            instanceCreateInfo.ppEnabledExtensionNames = extensionsNames.data();

            return instanceCreateInfo;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_AttachDebugMessengerInfo(VkInstanceCreateInfo& instanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo) const
        {
            if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
            {
                instanceCreateInfo.enabledLayerCount = UInt32(LayerNames.size());
                instanceCreateInfo.ppEnabledLayerNames = LayerNames.data();
                instanceCreateInfo.pNext = &debugMessengerCreateInfo;
            }
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_PrintAvailableExtensions() const
        {
#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
            UInt32 extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            Vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            KMP_LOG_DEBUG("available extensions:");
            for (const auto& extension : extensions)
            {
                KMP_MB_UNUSED const auto extensionVersionMajor = VK_API_VERSION_MAJOR(extension.specVersion);
                KMP_MB_UNUSED const auto extensionVersionMinor = VK_API_VERSION_MINOR(extension.specVersion);
                KMP_MB_UNUSED const auto extensionVersionPatch = VK_API_VERSION_PATCH(extension.specVersion);
                KMP_LOG_DEBUG("\t{}, Spec version: {}.{}.{}", extension.extensionName, extensionVersionMajor, extensionVersionMinor, extensionVersionPatch);
            }
#endif
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_InitializeDebugMessenger()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
            {
                const auto result = CreateDebugUtilsMessengerEXT(_instance, nullptr, &_debugMessenger);
                VulkanUtils::CheckResult(result, "VulkanGraphicsBackend: failed to setup debug messenger");
            }
        }
        //--------------------------------------------------------------------------

        Vector<const char*> VulkanGraphicsBackend::_GetRequiredExtensionsNames() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            UInt32 extensionsCount = 0;
            const char** extensionsStrings = glfwGetRequiredInstanceExtensions(&extensionsCount);

            Vector<const char*> extensionsNames(extensionsStrings, extensionsStrings + extensionsCount);

            if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
            {
                extensionsNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensionsNames;
#else
            return Vector<const char*>();
#endif
        }
        //--------------------------------------------------------------------------
    }
}