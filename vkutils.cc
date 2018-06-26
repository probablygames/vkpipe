
#include "vkutils.h"
#include "vkmacros.h"

#include <string.h>
#include <unistd.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int
CreateVulkanInstance
(
  VkInstance *instance)
{
  int major, minor, rev;
  glfwGetVersion(&major, &minor, &rev);
  std::cerr << "GLFW Version: " << major << " " << minor << " " << rev << std::endl;

  if (GLFW_TRUE != glfwInit()) {
    std::cerr << "glfwInit" << std::endl;
    return -1;
  } else {
    std::cerr << "GLFW_TRUE == glfwInit()" << std::endl;
  }
  if (GLFW_TRUE != glfwVulkanSupported()) {
    std::cerr << "glfwVulkanSupported()" << std::endl;
    return -1;
  } else {
    std::cerr << "GLFW_TURE == glfwVulkanSupported()" << std::endl;
  }

  uint32_t glfw_extensions_count;
  const char **glfw_extensions = glfwGetRequiredInstanceExtensions(
      &glfw_extensions_count);
  std::cerr << "RequiredInstanceExtensions: " << std::endl;
  for (uint32_t k = 0 ; k < glfw_extensions_count; ++k) {
    std::cerr << "    " << glfw_extensions[k] << std::endl;
  }

  uint32_t property_count;
  vk(EnumerateInstanceLayerProperties(&property_count, NULL));
  VkLayerProperties *layer_properties = new VkLayerProperties[property_count];
  vk(EnumerateInstanceLayerProperties(&property_count, layer_properties));
  std::cerr << "Instance Layer Properties: " << std::endl;
  for (uint32_t k = 0; k < property_count; ++k) {
    uint32_t layer_property_count;
    vk(EnumerateInstanceExtensionProperties(
        layer_properties[k].layerName, &layer_property_count,
         NULL)); 
    std::unique_ptr<VkExtensionProperties[]> extension_properties(
        new VkExtensionProperties[layer_property_count]);
    vk(EnumerateInstanceExtensionProperties(
        layer_properties[k].layerName, &layer_property_count,
        extension_properties.get()));
    std::cerr << "    " << layer_properties[k].layerName << " : "
        << layer_properties[k].description << std::endl;
    for (uint32_t n = 0; n < layer_property_count; ++n) {
      std::cerr << "        " <<  extension_properties.get()[n].extensionName
           << std::endl;
    }
  }

  VkApplicationInfo app;
  app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app.pNext = NULL;
  app.pApplicationName = "waveycrocket";
  app.applicationVersion = 1;
  app.pEngineName = "waveycrocket";
  app.engineVersion = 1;
  app.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create;
  create.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create.pNext = NULL;
  create.flags = 0;
  create.pApplicationInfo = &app;

  char const *enabled_layers[2]= {
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_LUNARG_parameter_validation"
  };
  create.enabledLayerCount = 1;
  create.ppEnabledLayerNames = enabled_layers;

  char const *enabled_extensions[glfw_extensions_count + 1];
  for (uint32_t k = 0; k < glfw_extensions_count; ++k) {
    enabled_extensions[k] = glfw_extensions[k];
  }
  enabled_extensions[glfw_extensions_count] = "VK_EXT_debug_report";

  create.enabledExtensionCount =  glfw_extensions_count + 1;
  create.ppEnabledExtensionNames = enabled_extensions;

  PFN_vkCreateInstance vk_create_instance = (
      (PFN_vkCreateInstance) glfwGetInstanceProcAddress(
          NULL, "vkCreateInstance"));

  vk(_create_instance(&create, NULL, instance));

  return 0;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
    VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
    uint64_t object, size_t location, int32_t messageCode,
    const char *pLayerPrefix, const char *pMessage, void *pUserData)
{
  std::cerr
      << "DebugReportCallback [" 
      << ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
          ? "ERRR" :
          (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
          ? "WARN" :
          (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
          ? "PERF" : 
          (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
          ? "INFO" :
          (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
          ? "DEBG" : "????") << "] "
      << messageCode << " : " << pMessage << std::endl;
  return VK_FALSE;
}

int
CreateDebugReportCallback(
    VkInstance instance,
    VkDebugReportCallbackEXT *callback)
{
  VkDebugReportCallbackCreateInfoEXT drc;
  drc.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  drc.pNext = NULL;
  drc.flags = (
    VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
    VK_DEBUG_REPORT_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_ERROR_BIT_EXT |
    VK_DEBUG_REPORT_DEBUG_BIT_EXT);
  drc.pfnCallback = DebugReportCallback;
  drc.pUserData = NULL;

  PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
      reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
          vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

  vk(CreateDebugReportCallbackEXT(instance,  &drc, NULL, callback));
  return 0;
}

int
CreatePhysicalDevice
(
    VkInstance instance,
    VkPhysicalDevice *physical_device,
    uint32_t *queue_family,
    VkPhysicalDeviceFeatures *device_features)
{
  uint32_t device_count;
  vk(EnumeratePhysicalDevices(instance, &device_count, NULL));
  std::unique_ptr<VkPhysicalDevice[]> physical_devices(
      new VkPhysicalDevice[device_count]);
  vk(EnumeratePhysicalDevices(instance, &device_count, physical_devices.get()));

  for (uint32_t d = 0; d < device_count; ++d) {
    VkPhysicalDeviceProperties device_props;
    vkGetPhysicalDeviceProperties(physical_devices[d], &device_props);

    VkPhysicalDeviceMemoryProperties memory_props;
    vkGetPhysicalDeviceMemoryProperties(physical_devices[d], &memory_props);

    VkPhysicalDeviceFeatures device_features_d;
    vkGetPhysicalDeviceFeatures(physical_devices[d], &device_features_d);

 
    std::cerr << "Device #" <<  d << std::endl
        << "  apiVersion = " << device_props.apiVersion << std::endl
        << "  driverVersion = " << device_props.driverVersion << std::endl
        << "  vendorID = " << device_props.vendorID << std::endl
        << "  deviceID = " << device_props.deviceID << std::endl
        << "  deviceType = "
            << vkPhysicalDeviceTypeString(device_props.deviceType)
            << std::endl
        << "  deviceName = " << device_props.deviceName << std::endl
        << "  memoryTypeCount = " << memory_props.memoryTypeCount << std::endl
        << "  Features: " 
            << vkPhysicalDeviceFeaturesString(device_features_d) << std::endl;

    for (uint32_t m = 0; m < memory_props.memoryTypeCount; ++m) {
      std::cerr << "  Memory #" << m << std::endl
          << "    propertyFlags = "
               << vkMemoryPropertyFlagsString(
                      memory_props.memoryTypes[m].propertyFlags) << std::endl
          << "    heapIndex = "
               << memory_props.memoryTypes[m].heapIndex << std::endl;
    
    }
    for (uint32_t m = 0; m < memory_props.memoryHeapCount; ++m) {
      std::cerr << "  Heap #" << m << std::endl
          << "    size = " << memory_props.memoryHeaps[m].size << std::endl
          << "    flags = "
               << vkMemoryHeapFlagsString(
                      memory_props.memoryHeaps[m].flags) << std::endl;
    }

    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_devices[d], &queue_family_count, NULL);
    std::unique_ptr<VkQueueFamilyProperties[]> queue_family_props(
        new VkQueueFamilyProperties[queue_family_count]);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_devices[d], &queue_family_count, queue_family_props.get());

    for (uint32_t q = 0; q < queue_family_count; ++q) {
      std::cerr << "  Queue # " << q << std::endl
          << "    queueFlags = " 
              << vkQueueFlagsString(
                  queue_family_props[q].queueFlags) << std::endl
          << "    queueCount = " << queue_family_props[q].queueCount << std::endl
          << "    timestampValidBits = "
               << queue_family_props[q].timestampValidBits << std::endl
          << "    minImageTransferGranularity = " 
               << queue_family_props[q].minImageTransferGranularity.width
               << " x "
               << queue_family_props[q].minImageTransferGranularity.height
               << std::endl;
      std::cerr << "    glfw presentation: ";
      if (GLFW_TRUE == glfwGetPhysicalDevicePresentationSupport(
              instance, physical_devices[d], q)) {
        std::cerr  << " yes" << std::endl;

        *physical_device = physical_devices[d];
        *queue_family = q;
        *device_features = device_features_d;
    
      } else {
        std::cerr  << " no" << std::endl;
      }
    }
  }
  return 0;
}

int
SelectMemoryTypeWithProperties(
  VkPhysicalDevice physical_device,
  VkMemoryRequirements mem_reqs,
  VkMemoryPropertyFlags properties,
  uint32_t *memory_type
)
{
  VkPhysicalDeviceMemoryProperties mem_props;
  vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_props);
  for (uint32_t m = 0; m < mem_props.memoryTypeCount; ++m) {
    if ((mem_reqs.memoryTypeBits & (1 << m)) &&
        (properties == (mem_props.memoryTypes[m].propertyFlags & properties))) {
      // return the first suitable memory type. 
      *memory_type = m;
      return 0;
    }
  }
  return -1;
}

int
CreateWindowSurface(
    VkInstance instance,
    VkPhysicalDevice physical_device,
    VkFormat surface_format,
    VkExtent2D surface_extent,
    GLFWwindow **window,
    VkSurfaceKHR *surface)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  *window = glfwCreateWindow(
      surface_extent.width, surface_extent.height, "Wavey Crocket", NULL, NULL);

  VkResult r = glfwCreateWindowSurface(instance, *window, NULL, surface);
  if (VK_SUCCESS != r) {
    std::cerr << __FILE__ << " : " << __LINE__  \
        << "  \"glfwCreateWindowSurface(instance, *window, NULL, surface)\" "
        << vkResultString(r) << std::endl; \
    return -1;
  }

  uint32_t format_count;
  vk(GetPhysicalDeviceSurfaceFormatsKHR(
      physical_device, *surface, &format_count, NULL));
  std::unique_ptr<VkSurfaceFormatKHR[]> surface_formats(
      new VkSurfaceFormatKHR[format_count]);
  vk(GetPhysicalDeviceSurfaceFormatsKHR(
      physical_device, *surface, &format_count, surface_formats.get()));
  
  bool found_format = false;

  // VkFormat surface_format = VK_FORMAT_R8G8B8A8_UNORM;

  for (uint32_t f = 0; f < format_count; ++f) {
    std::cerr  << "Surface Format #" << f << ": "
        << vkFormatString(surface_formats[f].format) << std::endl;
    if (surface_format == surface_formats[f].format) {
      found_format = true;
    }
  }
  std::cerr << "Found format: " << (found_format ? "yes" : "no") << std::endl;
  return (found_format ? 0 : -1);
}

int
CreateDevice
(
  VkPhysicalDevice physical_device,
  VkPhysicalDeviceFeatures device_features,
  uint32_t queue_family,
  VkDevice *device)
{
  float queue_priority = 1.0f;

  VkDeviceQueueCreateInfo qcr;
  qcr.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  qcr.pNext = NULL;  // VkDeviceQueueGlobalPriorityCreateInfoEXT
  qcr.flags = 0;
  qcr.queueFamilyIndex = queue_family;
  qcr.queueCount = 1;
  qcr.pQueuePriorities = &queue_priority;

  char const *swapchain_extension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

  VkDeviceCreateInfo dcr;
  dcr.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dcr.pNext = NULL;
  dcr.flags = 0;  // VkDeviceCreateFlags is reserved for future use.
  dcr.queueCreateInfoCount = 1;
  dcr.pQueueCreateInfos = &qcr;
  dcr.enabledLayerCount =  0;
  dcr.ppEnabledLayerNames = NULL;
  dcr.enabledExtensionCount = 1;
  dcr.ppEnabledExtensionNames =  &swapchain_extension;
  dcr.pEnabledFeatures = &device_features;

  vk(CreateDevice(physical_device, &dcr, NULL, device));

  return 0;
}

int
CreatePipelineCache(
    VkDevice device,
    VkPipelineCache *pipeline_cache)
{
  VkPipelineCacheCreateInfo pcc;
  pcc.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pcc.pNext = NULL;
  pcc.flags = 0;
  pcc.initialDataSize = 0;
  pcc.pInitialData = NULL;

  vk(CreatePipelineCache(device, &pcc, NULL, pipeline_cache));
  return 0;
}

int
CreateCommandPoolAndQueue
(
  VkDevice device,
  uint32_t queue_family,
  VkCommandPool *command_pool,
  VkQueue *queue)
{
  VkCommandPoolCreateInfo pcr;
  pcr.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pcr.pNext = NULL;
  pcr.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT = 0x00000001,
  // VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT = 0x00000002,
  // VK_COMMAND_POOL_CREATE_PROTECTED_BIT 
  pcr.queueFamilyIndex = queue_family;

  vk(CreateCommandPool(device, &pcr, NULL, command_pool));

  uint32_t queue_index = 0;  // queueCreateInfoCount == 1
  vkGetDeviceQueue(device, queue_family, queue_index, queue);

  return 0;
}

int
CreateCommandBuffer(
  VkDevice device,
  VkCommandPool command_pool,
  VkCommandBuffer *command_buffer)
{
  VkCommandBufferAllocateInfo cba;
  cba.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cba.pNext = NULL;
  cba.commandPool = command_pool;
  cba.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cba.commandBufferCount = 1;

  vk(AllocateCommandBuffers(device, &cba, command_buffer));

  return 0;
}

int
CreateSemaphore
(
    VkDevice device,
    VkSemaphore *semaphore)
{
  VkSemaphoreCreateInfo scr;
  scr.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  scr.pNext = NULL;
  scr.flags = 0;
  vk(CreateSemaphore(device, &scr, NULL, semaphore));
  return 0;
}

int
CreateSwapchain(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t queue_family,
    VkSurfaceKHR surface,
    VkFormat surface_format,
    uint32_t *image_count,
    VkSwapchainKHR *swapchain)
{
  VkSurfaceCapabilitiesKHR surface_caps;
  vk(GetPhysicalDeviceSurfaceCapabilitiesKHR(
          physical_device, surface, &surface_caps));

  std::cerr << "Surface capabilities:" << std::endl
      << "  minImageCount = " << surface_caps.minImageCount << std::endl
      << "  maxImageCount = " << surface_caps.maxImageCount << std::endl
      << "  currentExtent = "
          << surface_caps.currentExtent.width << " x "
          << surface_caps.currentExtent.height << std::endl
      << "  minImageExtent = "
          << surface_caps.minImageExtent.width << " x "
          << surface_caps.minImageExtent.height << std::endl
      << "  maxImageExtent = "
          << surface_caps.maxImageExtent.width << " x "
          << surface_caps.maxImageExtent.height << std::endl
      << "  supportedTransforms = "
          << vkSurfaceTransformFlagsKHRString(
              surface_caps.supportedTransforms) << std::endl
      << "  currentTransform = "
          << vkSurfaceTransformFlagBitsKHRString( 
              surface_caps.currentTransform) << std::endl
      << "  supportedCompositeAlpha = " 
          << vkCompositeAlphaFlagsKHRString(
              surface_caps.supportedCompositeAlpha) << std::endl
      << "  supportedUsageFlags = "
          << vkImageUsageFlagsString(
              surface_caps.supportedUsageFlags) << std::endl;

  VkExtent2D extent;
  if ((surface_caps.currentExtent.width == 0xFFFFFFFF &&
       surface_caps.currentExtent.height == 0xFFFFFFFF)) {
    extent.width = 1920;
    extent.height = 1080;
  } else {
    extent = surface_caps.currentExtent;
  }

  uint32_t modes_count;
  vk(GetPhysicalDeviceSurfacePresentModesKHR(
      physical_device, surface, &modes_count, NULL));
  std::unique_ptr<VkPresentModeKHR[]> modes(
    new VkPresentModeKHR[modes_count]);
  vk(GetPhysicalDeviceSurfacePresentModesKHR(
      physical_device, surface, &modes_count, modes.get()));
  std::cerr << "Presentation modes:" << std::endl;
  for (uint32_t m = 0; m < modes_count; ++m) {
    std::cerr << "  " << vkPresentModeKHRString(modes[m]) << std::endl;
  }

  VkBool32 supported;
  vk(GetPhysicalDeviceSurfaceSupportKHR(
      physical_device, queue_family, surface, &supported));
  std::cerr << "Physical Device supports surface: " << (supported ? "yes" : "no") << std::endl;
  if (supported != VK_TRUE) {
    return -1;
  }

  VkSwapchainCreateInfoKHR scr;
  scr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  scr.pNext = NULL;
  scr.flags = 0;
  scr.surface = surface;
  scr.minImageCount = *image_count;
  scr.imageFormat = surface_format;
  scr.imageExtent = extent;
  scr.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // only one supported ?
  scr.imageArrayLayers = 1 ;  // non-stereoscopic
  scr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  scr.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;  // VK_SHARING_MODE_CONCURRENT
  scr.queueFamilyIndexCount = 0; // nonzero only if CONCURRENT
  scr.pQueueFamilyIndices = NULL;
  scr.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  scr.compositeAlpha =  VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  scr.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  scr.clipped = VK_TRUE;
  scr.oldSwapchain = VK_NULL_HANDLE;


  vk(CreateSwapchainKHR(device, &scr, NULL, swapchain));
  vk(GetSwapchainImagesKHR(device, *swapchain, image_count, NULL));
  return 0;
}

int
AllocateMemory(
    VkPhysicalDevice physical_device,
    VkMemoryRequirements const& mem_reqs,
    VkMemoryPropertyFlags properties,
    VkDevice device,
    VkDeviceMemory *memory)
{
  uint32_t memory_type;
  SelectMemoryTypeWithProperties(
      physical_device, mem_reqs, properties, &memory_type);

  VkMemoryAllocateInfo mal;
  mal.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mal.pNext = NULL;
  mal.allocationSize = mem_reqs.size;
  mal.memoryTypeIndex = memory_type;

  vk(AllocateMemory(device, &mal, NULL, memory));
  return 0;
}

int
MapAndInitializeMemory(
    VkDevice device,
    VkDeviceMemory memory,
    void const *data,
    uint32_t size)
{
  void *mapped_memory;
  vk(MapMemory(device, memory, 0, size, 0, &mapped_memory));
  memcpy(mapped_memory, data, size);
  vkUnmapMemory(device, memory);
  return 0;
}

int
CreateBuffer(
    VkDevice device,
    uint32_t buffer_size,
    VkBufferUsageFlags usage,
    VkBuffer *buffer)
{
  VkBufferCreateInfo bcr;
  bcr.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bcr.pNext = NULL;
  bcr.flags = 0;
  bcr.size = buffer_size;
  bcr.usage = usage;
  bcr.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bcr.queueFamilyIndexCount = 0;
  bcr.pQueueFamilyIndices = NULL; // EXCLUSIVE

  vk(CreateBuffer(device, &bcr, NULL, buffer));
  return 0;
}

int
CreateAndAllocateBuffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t buffer_size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer *buffer,
    VkDeviceMemory *memory)
{
  re(CreateBuffer(device, buffer_size, usage, buffer));

  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(device, *buffer, &mem_reqs);

  re(AllocateMemory(
      physical_device, mem_reqs, properties, device, memory));

  vk(BindBufferMemory(device, *buffer, *memory, 0));
  return 0;
}

int
CreateAndAllocateVertexBuffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t buffer_size,
    VkBuffer *vertex_buffer,
    VkDeviceMemory *vertex_memory)
{
  re(CreateAndAllocateBuffer(
      physical_device, device, buffer_size,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      vertex_buffer, vertex_memory));
  return 0;
}

int
CreateAndAllocateUniformMat4Buffer(
  VkPhysicalDevice physical_device,
  VkDevice device,
  VkBuffer *uniform_buffer,
  VkDeviceMemory *uniform_memory)
{
  uint32_t buffer_size =  4 * 4 * sizeof(float);
  re(CreateAndAllocateBuffer(
      physical_device, device, buffer_size,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      uniform_buffer, uniform_memory));
  return 0;
}

int CreateAndAllocateUniformVec4Buffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory)
{
  uint32_t buffer_size =  4 * sizeof(float);
  re(CreateAndAllocateBuffer(
      physical_device, device, buffer_size,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      uniform_buffer, uniform_memory));
  return 0;
}

int
CreateImage2D(
    VkDevice device,
    VkExtent2D extent,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkImage *image)
{
  VkImageCreateInfo icr;
  icr.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  icr.pNext = NULL;
  // VkDedicatedAllocationImageCreateInfoNV, VkExternalFormatANDROID,
  // VkExternalMemoryImageCreateInfo, VkExternalMemoryImageCreateInfoNV, 
  // VkImageFormatListCreateInfoKHR, or VkImageSwapchainCreateInfoKHR
  icr.flags = 0;  // VkImageCreateFlagBits
  icr.imageType = VK_IMAGE_TYPE_2D;
  icr.format = format; // VK_FORMAT_R8G8B8A8_UNORM;
  icr.extent.width = extent.width;
  icr.extent.height = extent.height;
  icr.extent.depth = 1;
  icr.mipLevels = 1;
  icr.arrayLayers = 1;
  icr.samples = VK_SAMPLE_COUNT_1_BIT;
  icr.tiling = tiling;
  icr.usage = usage;
  icr.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  icr.queueFamilyIndexCount = 0;
  icr.pQueueFamilyIndices = NULL;  // unused becaue EXCLUSIVE
  icr.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

  vk(CreateImage(device, &icr, NULL, image));
  return 0;
}


int
CreateAndAllocateImage2D
(
  VkPhysicalDevice physical_device,
  VkDevice device,
  VkExtent2D extent,
  VkFormat format,
  VkImageTiling tiling,
  VkImageUsageFlags usage,
  VkMemoryPropertyFlags properties,
  VkImage *image, VkDeviceMemory *memory)
{
  re(CreateImage2D(device, extent, format, tiling, usage, image));
  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device, *image, &mem_reqs);
  re(AllocateMemory(physical_device, mem_reqs, properties, device, memory));
  vk(BindImageMemory(device, *image, *memory, 0));
  return 0;
}

int
CreateImage2DView
(
  VkDevice device,
  VkImage image,
  VkFormat format,
  VkImageAspectFlags aspect_mask,
  VkImageView *image_view)
{
  VkImageViewCreateInfo ivc;
  ivc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ivc.pNext = NULL; // VkImageViewUsageCreateInfo, VkImageUsageFlagBits
  ivc.flags = 0;
  ivc.image = image,
  ivc.viewType = VK_IMAGE_VIEW_TYPE_2D; // VkImageViewType
  ivc.format = format;
  ivc.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivc.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivc.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivc.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  // VK_IMAGE_ASPECT_COLOR_BIT,  DEPTH, STENCIL
  ivc.subresourceRange.aspectMask = aspect_mask;
  ivc.subresourceRange.baseMipLevel = 0;
  ivc.subresourceRange.levelCount = 1;
  ivc.subresourceRange.baseArrayLayer = 0;
  ivc.subresourceRange.layerCount = 1;

  vk(CreateImageView(device, &ivc, NULL, image_view));
  return 0;
}

int
CreateDepthBuffer
(
  VkPhysicalDevice physical_device,
  VkDevice device,
  VkExtent2D extent,
  VkImage *depth_image,
  VkDeviceMemory *depth_memory,
  VkImageView *depth_image_view)
{
  VkFormatProperties format_props;
  vkGetPhysicalDeviceFormatProperties(
      physical_device, VK_FORMAT_D16_UNORM, &format_props);
  
  std::cerr << "Format properties for VK_FORMAT_D16_UNORM" << std::endl
    << "  linearTilingFeatures = "
         << vkFormatFeatureFlagsString(
            format_props.linearTilingFeatures) << std::endl
    << "  optimralTilingFeatures = "
         << vkFormatFeatureFlagsString(
            format_props.optimalTilingFeatures) << std::endl
    << "  bufferFeatures = "
         << vkFormatFeatureFlagsString(
            format_props.bufferFeatures) << std::endl;
  if (!(format_props.optimalTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)) {
    return -1;
  }

  re(CreateAndAllocateImage2D(
      physical_device, device, extent, VK_FORMAT_D16_UNORM,
      VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      0, // No flags
      depth_image, depth_memory));

  re(CreateImage2DView(
      device, *depth_image, VK_FORMAT_D16_UNORM, VK_IMAGE_ASPECT_DEPTH_BIT,
      depth_image_view));
  return 0;
}

 
int
CreateSwapchainImageViews
(
  VkDevice device,
  VkSwapchainKHR swapchain,
  VkFormat surface_format,
  uint32_t image_count,
  std::unique_ptr<VkImage[]> *swapchain_images,
  std::unique_ptr<VkImageView[]> *swapchain_image_views)
{
  swapchain_images->reset(new VkImage[image_count]);
  vk(GetSwapchainImagesKHR(device, swapchain, &image_count, swapchain_images->get()));

  swapchain_image_views->reset(new VkImageView[image_count]);
  for (uint32_t i = 0; i < image_count; ++i) {
    re(CreateImage2DView(
        device, (*swapchain_images)[i], surface_format, VK_IMAGE_ASPECT_COLOR_BIT,
        &((*swapchain_image_views)[i])));
  }
  return 0;
}

int
CreateRenderPass(
    VkDevice device,
    VkFormat surface_format,
    VkRenderPass *render_pass)
{
  VkAttachmentDescription attachments[2];

  // Pixel buffer
  attachments[0].flags = 0;
  attachments[0].format = surface_format;
  attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
  attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference color_reference;
  color_reference.attachment = 0;
  color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // Depth buffer
  attachments[1].flags = 0;
  attachments[1].format = VK_FORMAT_D16_UNORM;
  attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
  attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depth_reference;
  depth_reference.attachment = 1;
  depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass;
  subpass.flags = 0;
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.inputAttachmentCount = 0;
  subpass.pInputAttachments = NULL;
  subpass.colorAttachmentCount = 1;
  subpass.pResolveAttachments = NULL;
  subpass.pColorAttachments = &color_reference;
  subpass.pDepthStencilAttachment = &depth_reference;
  subpass.preserveAttachmentCount = 0;
  subpass.pPreserveAttachments = NULL;

  VkRenderPassCreateInfo rpc;
  rpc.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  rpc.pNext = NULL;  // VkRenderPassInputAttachmentAspectCreateInfo or VkRenderPassMultiviewCreateInfo
  rpc.flags = 0;
  rpc.attachmentCount = 2;
  rpc.pAttachments = attachments;
  rpc.subpassCount = 1;
  rpc.pSubpasses = &subpass;
  rpc.dependencyCount = 0;
  rpc.pDependencies = NULL;

  vk(CreateRenderPass(device, &rpc, NULL, render_pass));
  return 0;
}

int
CreateShaderModule(
    VkDevice device,
    char const *spirv_filename,
    VkShaderModule *shader_module)
{    
  std::vector<char> spirv;
  std::ifstream file(spirv_filename, std::ios::binary);  
  if (!file.good()) {
    return -1;
  }
  file.seekg(0, std::ios_base::end);
  std::streampos size = file.tellg();
  file.seekg(0, std::ios_base::beg);
  spirv.resize(size);
  file.read(spirv.data(), size);

  VkShaderModuleCreateInfo smc;
  smc.sType =  VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  smc.pNext = NULL; // VkShaderModuleValidationCacheCreateInfoEXT
  smc.flags = 0;
  smc.codeSize = size;
  smc.pCode = reinterpret_cast<uint32_t const *>(spirv.data());

  vk(CreateShaderModule(device, &smc, NULL, shader_module));
  return 0;
}

int
CreateFramebuffers(
    VkDevice device,
    uint32_t image_count,
    VkExtent2D surface_extent,
    VkRenderPass render_pass,
    VkImageView depth_image_view,
    std::unique_ptr<VkImageView[]> const& swapchain_image_views,
    std::unique_ptr<VkFramebuffer[]> *swapchain_framebuffers)
{
  swapchain_framebuffers->reset(new VkFramebuffer[image_count]);

  for (uint32_t k = 0; k < image_count; ++k) {
    VkImageView attachments[2];
    attachments[0] = swapchain_image_views[k];
    attachments[1] = depth_image_view;

    VkFramebufferCreateInfo fbc;
    fbc.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbc.pNext = NULL;
    fbc.flags = 0;
    fbc.renderPass = render_pass;
    fbc.attachmentCount = 2;
    fbc.pAttachments = attachments;
    fbc.width = surface_extent.width;
    fbc.height = surface_extent.height;
    fbc.layers = 1;

    vk(CreateFramebuffer(device, &fbc, NULL, &((*swapchain_framebuffers)[k])));
  }
  return 0;
}

int
CreateSampler(
    VkDevice device,
    VkSampler *sampler)
{
  VkSamplerCreateInfo scr;
  scr.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  scr.pNext = NULL;
  scr.flags = 0;
  scr.magFilter = VK_FILTER_LINEAR;
  scr.minFilter = VK_FILTER_LINEAR;
  scr.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  scr.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  scr.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  scr.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  scr.mipLodBias = 0.0f;
  scr.anisotropyEnable = VK_FALSE;
  scr.maxAnisotropy = 0.0f;
  scr.compareEnable = VK_FALSE;
  scr.compareOp = VK_COMPARE_OP_NEVER;
  scr.minLod = 0.0f;
  scr.maxLod = 0.0f;
  scr.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
  scr.unnormalizedCoordinates = VK_FALSE;

  vk(CreateSampler(device, &scr, NULL, sampler));
  return 0;
}

int
AcquireSwapchainImage(
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkSemaphore present_complete,
    uint32_t *n_swapchain_image)
{
  vk(AcquireNextImageKHR(
      device, swapchain, 1000, present_complete, VK_NULL_HANDLE,
      n_swapchain_image));
  return 0;
}

void
BeginCommandBuffer(VkCommandBuffer command_buffer)
{
  VkCommandBufferBeginInfo cbb;
  cbb.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cbb.pNext = NULL;  // VkDeviceGroupCommandBufferBeginInfo
  cbb.flags = 0;  // VK_COMMAND_BUFFER_USAGE_*
  cbb.pInheritanceInfo = NULL;
  vkBeginCommandBuffer(command_buffer, &cbb);
}

void
ConfigureImageMemoryBarrier(
    VkImage image, VkImageAspectFlags aspect,
    VkAccessFlags src_access,  VkImageLayout old_layout,
    VkAccessFlags dst_access,  VkImageLayout new_layout,
    VkImageMemoryBarrier *imb) {
  imb->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  imb->pNext = NULL;
  imb->srcAccessMask = src_access;
  imb->dstAccessMask = dst_access;
  imb->oldLayout = old_layout;
  imb->newLayout = new_layout;
  imb->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  imb->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  imb->image = image;
  imb->subresourceRange.aspectMask = aspect;
  imb->subresourceRange.baseMipLevel = 0;
  imb->subresourceRange.levelCount = 1;
  imb->subresourceRange.baseArrayLayer = 0;
  imb->subresourceRange.layerCount = 1;
}


void
TopOfPipelineImageMemoryBarriers(
    VkCommandBuffer command_buffer,
    VkImage depth_image,
    VkImage texture_image,
    VkImage swapchain_image)
{
  VkImageMemoryBarrier imb[3];
  // depth buffer
  ConfigureImageMemoryBarrier(
      depth_image, VK_IMAGE_ASPECT_DEPTH_BIT,
      0,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      &imb[0]);

  // frame buffer
  ConfigureImageMemoryBarrier(
      swapchain_image, VK_IMAGE_ASPECT_COLOR_BIT,
      0,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      &imb[1]);
      
  // texture
  ConfigureImageMemoryBarrier(
      texture_image, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_ACCESS_HOST_WRITE_BIT, VK_IMAGE_LAYOUT_GENERAL, // VK_IMAGE_LAYOUT_PREINITIALIZED,
      VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_GENERAL,
      &imb[2]);

  vkCmdPipelineBarrier(
      command_buffer,
      VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
      0,  // VkDependencyFlags
      0, NULL,  // VkMemoryBarrier
      0, NULL,  // VkBufferMemoryBarrier
      3, imb);  // VkimageMemoryBarrier
}

void BeginRenderPass(
    VkCommandBuffer command_buffer,
    VkRenderPass render_pass,
    VkFramebuffer framebuffer,
    VkExtent2D surface_extent)
{
  VkClearValue clear_values[2];
  clear_values[0].color.float32[0] = 0.2f;
  clear_values[0].color.float32[1] = 0.2f;
  clear_values[0].color.float32[2] = 0.2f;
  clear_values[0].color.float32[3] = 0.2f;
  clear_values[1].depthStencil.depth = 1.0f;
  clear_values[1].depthStencil.stencil = 0.0f;

  VkRenderPassBeginInfo rpb;
  rpb.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpb.pNext = NULL;  // 
  rpb.renderPass = render_pass;
  rpb.framebuffer = framebuffer;
  rpb.renderArea.offset.x = 0;
  rpb.renderArea.offset.y= 0;
  rpb.renderArea.extent.width = surface_extent.width;
  rpb.renderArea.extent.height = surface_extent.height;
  rpb.clearValueCount = 2;
  rpb.pClearValues = clear_values;

  vkCmdBeginRenderPass(
      command_buffer, &rpb, VK_SUBPASS_CONTENTS_INLINE);
}

void
SetViewportAndScissor(
    VkCommandBuffer command_buffer,
    VkExtent2D surface_extent)
{
  VkViewport viewport;
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = surface_extent.width;
  viewport.height = surface_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor;
  scissor.extent.width = surface_extent.width;
  scissor.extent.height = surface_extent.height;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

void
BindGraphicsPipeline(
    VkCommandBuffer command_buffer,
    VkPipeline pipeline,
    VkPipelineLayout pipeline_layout,
    VkDescriptorSet const *descriptor_set)
{
  vkCmdBindPipeline(
      command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vkCmdBindDescriptorSets(
      command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout,
      0, 1, descriptor_set, 0, NULL);
}

void
DrawVertexBuffer(
    VkCommandBuffer command_buffer,
    VkBuffer vertex_buffer,
    uint32_t n_vertices)
{
  VkDeviceSize buffer_offset = 0;
  vkCmdBindVertexBuffers (command_buffer, 0, 1, &vertex_buffer, &buffer_offset);
  vkCmdDraw (command_buffer, n_vertices, 1, 0, 0);
}

void
EndRenderPass(
    VkCommandBuffer command_buffer)
{
  vkCmdEndRenderPass (command_buffer);
}      

void
BottomOfPipelineImageMemoryBarriers(
    VkCommandBuffer command_buffer,
    VkImage swapchain_image)
{
  VkImageMemoryBarrier imb;
  ConfigureImageMemoryBarrier(
      swapchain_image, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      VK_ACCESS_MEMORY_READ_BIT,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      &imb);
      
  vkCmdPipelineBarrier(
      command_buffer,
      VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      0,  // VkDependencyFlags
      0, NULL,  // VkMemoryBarrier
      0, NULL,  // VkBufferMemoryBarrier
      1, &imb);  // VkimageMemoryBarrier
}

int
QueueSubmit(
    VkQueue queue,
    VkCommandBuffer command_buffer,
    VkSemaphore present_complete,
    VkSemaphore render_complete)
{
  VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

  VkSubmitInfo sub;
  sub.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  sub.pNext = NULL;
  sub.waitSemaphoreCount = 1;
  sub.pWaitSemaphores = &present_complete;
  sub.pWaitDstStageMask = &wait_stage;
  sub.commandBufferCount = 1;
  sub.pCommandBuffers  = &command_buffer;
  sub.signalSemaphoreCount = 1;
  sub.pSignalSemaphores = &render_complete;

  vk(QueueSubmit (queue, 1, &sub, VK_NULL_HANDLE));
  return 0;
}

int
QueuePresent(
    VkQueue queue,
    VkSwapchainKHR swapchain,
    uint32_t n_swapchain_image,
    VkSemaphore render_complete)
{

  VkResult present_result = VK_INCOMPLETE;
  VkPresentInfoKHR pre;
  pre.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pre.pNext = NULL;
  pre.waitSemaphoreCount = 1;
  pre.pWaitSemaphores = &render_complete;
  pre.swapchainCount = 1;
  pre.pSwapchains = &swapchain;
  pre.pImageIndices = &n_swapchain_image;
  pre.pResults = &present_result;

  vk(QueuePresentKHR(queue, &pre));
  vk(QueueWaitIdle(queue));
  if (present_result != VK_SUCCESS) {
    std::cerr << __FILE__ << " : " << __LINE__ 
        << "  present_result: " << vkResultString(present_result) << std::endl;
    return -1;
  }
  return 0;
}

int
Draw(
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkCommandBuffer command_buffer,
    VkRenderPass render_pass,
    VkPipeline pipeline,
    VkPipelineLayout pipeline_layout,
    VkDescriptorSet const *descriptor_set,
    VkExtent2D surface_extent,
    VkBuffer vertex_buffer,
    uint32_t n_vertices,
    VkImage texture_image,
    VkImage depth_image,
    VkImage swapchain_image,
    VkFramebuffer framebuffer)
{

  BeginCommandBuffer(command_buffer);

  TopOfPipelineImageMemoryBarriers(
      command_buffer, depth_image, texture_image, swapchain_image);

  BeginRenderPass (command_buffer, render_pass, framebuffer, surface_extent);

  BindGraphicsPipeline (command_buffer, pipeline, pipeline_layout, descriptor_set);

  SetViewportAndScissor (command_buffer, surface_extent);

  DrawVertexBuffer (command_buffer, vertex_buffer, n_vertices);

  EndRenderPass (command_buffer);

  BottomOfPipelineImageMemoryBarriers (command_buffer, swapchain_image);

  vk(EndCommandBuffer (command_buffer));
  return 0;
}

void
GetImagePitch(
  VkDevice device,
  VkImage image,
  uint32_t *size, uint32_t *pitch)
{
  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device, image, &mem_reqs);

  *size = mem_reqs.size;

  VkImageSubresource isr;
  isr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  isr.mipLevel = 0;
  isr.arrayLayer = 0;

  VkSubresourceLayout layout;
  vkGetImageSubresourceLayout(device, image, &isr, &layout);

  *pitch = layout.rowPitch;
}


