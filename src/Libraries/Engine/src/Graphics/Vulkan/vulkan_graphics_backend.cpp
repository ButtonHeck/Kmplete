#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Version/kmplete_version.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <cstring>
#include <stdexcept>


namespace Kmplete
{
    static constexpr auto ValidationLayerName = "VK_LAYER_KHRONOS_validation";
    static const Vector<const char*> LayerNames = { ValidationLayerName };

    namespace Graphics
    {
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
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
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            auto debugMessengerCreateInfo = VulkanUtils::InitVkDebugUtilsMessengerCreateInfo();
            debugMessengerCreateInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
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
#endif


        VulkanGraphicsBackend::VulkanGraphicsBackend(Window& window)
            : GraphicsBackend(window)
            , _instance(VK_NULL_HANDLE)
            , _surface(nullptr)
            , _physicalDevice(nullptr)
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

        const GraphicsSurface& VulkanGraphicsBackend::GetGraphicsSurface() const noexcept
        {
            return *_surface.get();
        }
        //--------------------------------------------------------------------------

        const VulkanPhysicalDevice& VulkanGraphicsBackend::GetPhysicalDevice() const noexcept
        {
            return *_physicalDevice.get();
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice& VulkanGraphicsBackend::GetPhysicalDevice() noexcept
        {
            return *_physicalDevice.get();
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

        void VulkanGraphicsBackend::HandleWindowResize()
        {
            if (!_window.IsIconified())
            {
                _physicalDevice->HandleWindowResize();
            }
        }
        //--------------------------------------------------------------------------

        Nullable<Texture*> VulkanGraphicsBackend::CreateTexture(const Image& image)
        {
            return _physicalDevice->GetLogicalDevice().CreateTexture(image);
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

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            if (!_CheckValidationLayerSupport())
            {
                KMP_LOG_CRITICAL("{} layer requested but not found", ValidationLayerName);
                throw std::runtime_error("VulkanGraphicsBackend: failed to initialize");
            }
#endif

            VkApplicationInfo applicationInfo = _CreateApplicationInfo();
            auto extensionsNames = _GetRequiredExtensionsNames();
            VkInstanceCreateInfo instanceCreateInfo = _CreateInstanceCreateInfo(applicationInfo, extensionsNames);

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            auto debugMessengerCreateInfo = CreateDebugMessengerCreateInfo();
            _AttachDebugMessengerInfo(instanceCreateInfo, debugMessengerCreateInfo);
            _PrintAvailableExtensions();
#endif

            const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &_instance);
            VulkanUtils::CheckResult(result, "VulkanGraphicsBackend: failed to create VkInstance");

            if (!VulkanCommands::LoadExtensionFunctions(_instance))
            {
                KMP_LOG_CRITICAL("extension functions failed to load");
                throw std::runtime_error("VulkanGraphicsBackend: extension functions failed to load");
            }

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            _InitializeDebugMessenger();
#endif

            _surface.reset(new VulkanGraphicsSurface(_window, _instance));
            _physicalDevice.reset(new VulkanPhysicalDevice(_window, _currentBufferIndex, _instance, _surface->GetVkSurface()));
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_Finalize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            VulkanCommands::DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
#endif

            _physicalDevice.reset();
            _surface.reset();

            vkDestroyInstance(_instance, nullptr);
        }
        //--------------------------------------------------------------------------

        VkApplicationInfo VulkanGraphicsBackend::_CreateApplicationInfo() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto applicationInfo = VulkanUtils::InitVkApplicationInfo();
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
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto instanceCreateInfo = VulkanUtils::InitVkInstanceCreateInfo(applicationInfo);
            instanceCreateInfo.enabledExtensionCount = UInt32(extensionsNames.size());
            instanceCreateInfo.ppEnabledExtensionNames = extensionsNames.data();

            return instanceCreateInfo;
        }
        //--------------------------------------------------------------------------

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
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

        void VulkanGraphicsBackend::_AttachDebugMessengerInfo(VkInstanceCreateInfo& instanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo) const
        {
            instanceCreateInfo.enabledLayerCount = UInt32(LayerNames.size());
            instanceCreateInfo.ppEnabledLayerNames = LayerNames.data();
            instanceCreateInfo.pNext = &debugMessengerCreateInfo;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_PrintAvailableExtensions() const
        {
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
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsBackend::_InitializeDebugMessenger()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto debugMessengerCreateInfo = CreateDebugMessengerCreateInfo();
            const auto result = VulkanCommands::CreateDebugUtilsMessengerEXT(_instance, &debugMessengerCreateInfo, nullptr, &_debugMessenger);
            VulkanUtils::CheckResult(result, "VulkanGraphicsBackend: failed to setup debug messenger");
        }
        //--------------------------------------------------------------------------
#endif

        Vector<const char*> VulkanGraphicsBackend::_GetRequiredExtensionsNames() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            UInt32 extensionsCount = 0;
            const char** extensionsStrings = glfwGetRequiredInstanceExtensions(&extensionsCount);

            Vector<const char*> extensionsNames(extensionsStrings, extensionsStrings + extensionsCount);

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            extensionsNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            return extensionsNames;
#else
            return Vector<const char*>();
#endif
        }
        //--------------------------------------------------------------------------
    }
}