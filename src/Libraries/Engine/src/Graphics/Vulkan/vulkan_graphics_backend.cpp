#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
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
static constexpr auto ValidationLayerName = "VK_LAYER_KHRONOS_validation";


namespace Kmplete
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        KMP_MB_UNUSED VkDebugUtilsMessageTypeFlagsEXT messageType,
        KMP_MB_UNUSED const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        KMP_MB_UNUSED void* pUserData)
    {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            KMP_LOG_DEBUG_FN("VulkanGraphicsBackend DebugCallback: {}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        {
            KMP_LOG_INFO_FN("VulkanGraphicsBackend DebugCallback: {}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            KMP_LOG_WARN_FN("VulkanGraphicsBackend DebugCallback: {}", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            KMP_LOG_ERROR_FN("VulkanGraphicsBackend DebugCallback: {}", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
    //--------------------------------------------------------------------------


    VulkanGraphicsBackend::VulkanGraphicsBackend()
        : GraphicsBackend(GraphicsBackendType::Vulkan)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    VulkanGraphicsBackend::~VulkanGraphicsBackend()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void VulkanGraphicsBackend::_Initialize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
        {
            if (!_CheckValidationLayerSupport())
            {
                KMP_LOG_ERROR("{} layer requested but not found", ValidationLayerName);
                throw std::runtime_error("VulkanGraphicsBackend: failed to initialize");
            }
        }

        VkApplicationInfo applicationInfo = _CreateApplicationInfo();
        auto extensionsNames = _GetRequiredExtensionsNames();
        VkInstanceCreateInfo instanceCreateInfo = _CreateInstanceCreateInfo(applicationInfo, extensionsNames);
        _AttachDebugMessenger(instanceCreateInfo);

        _PrintAvailableExtensions();

        const auto success = vkCreateInstance(&instanceCreateInfo, nullptr, &_instance);
        if (success != VK_SUCCESS)
        {
            KMP_LOG_ERROR("failed to create VkInstance");
            throw std::runtime_error("VulkanGraphicsBackend: failed to create VkInstance");
        }
    }
    //--------------------------------------------------------------------------

    void VulkanGraphicsBackend::_Finalize()
    {
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
        VkApplicationInfo applicationInfo{};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
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
        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledExtensionCount = UInt32(extensionsNames.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensionsNames.data();

        return instanceCreateInfo;
    }
    //--------------------------------------------------------------------------

    void VulkanGraphicsBackend::_AttachDebugMessenger(VkInstanceCreateInfo& instanceCreateInfo) const
    {
        if (KMP_ENABLE_VULKAN_VALIDATION_LAYER)
        {
            VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
            debugMessengerCreateInfo = {};
            debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
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
            debugMessengerCreateInfo.pUserData = nullptr;

            const Vector<const char*> layerNames = { ValidationLayerName };
            instanceCreateInfo.enabledLayerCount = 1;
            instanceCreateInfo.ppEnabledLayerNames = layerNames.data();
            instanceCreateInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugMessengerCreateInfo);
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
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