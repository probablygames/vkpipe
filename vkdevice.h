#ifndef _VKDEVICE_H_
#define _VKDEVICE_H_

#include <stdint.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <memory>

struct VulkanDevice {
  VkInstance instance;
  VkDebugReportCallbackEXT debug_report_callback;

  VkPhysicalDevice physical_device;
  uint32_t queue_family;

  VkPhysicalDeviceFeatures device_features;

  VkFormat surface_format;
  VkExtent2D surface_extent;

  VkSurfaceKHR surface;
  GLFWwindow *window;

  VkDevice device;

  VkCommandPool command_pool;
  VkQueue queue;

  VkCommandBuffer command_buffer;
  VkSemaphore render_complete, present_complete;

  VkImage depth_image;
  VkDeviceMemory depth_memory;
  VkImageView depth_image_view;

  VkSwapchainKHR swapchain;
  uint32_t swapchain_image_count;

  std::unique_ptr<VkImage[]> swapchain_images;
  std::unique_ptr<VkImageView[]> swapchain_image_views;

  VkRenderPass render_pass;

  std::unique_ptr<VkFramebuffer[]> swapchain_framebuffers;

  VkPipelineCache pipeline_cache;

  int InitializeVulkanDevice(
      uint32_t window_width, uint32_t window_height);
};

#endif // _VKDEVICE_H_
