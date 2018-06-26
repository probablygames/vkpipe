#include "vkdevice.h"
#include "vkutils.h"
#include "vkmacros.h"

int
VulkanDevice::
InitializeVulkanDevice(
   uint32_t window_width, uint32_t window_height)
{
  re(CreateVulkanInstance (&instance));

  re(CreateDebugReportCallback(instance, &debug_report_callback));

  re(CreatePhysicalDevice(
      instance, &physical_device, &queue_family, &device_features));

  surface_format = VK_FORMAT_B8G8R8A8_UNORM;
  surface_extent.width = window_width;
  surface_extent.height = window_height;
  re(CreateWindowSurface(
      instance, physical_device, surface_format, surface_extent,
      &window, &surface));

  re(CreateDevice(physical_device, device_features, queue_family, &device));

  re(CreateCommandPoolAndQueue(device, queue_family, &command_pool, &queue));

  re(CreateCommandBuffer(device, command_pool, &command_buffer));

  re(CreateSemaphore(device, &render_complete));
  re(CreateSemaphore(device, &present_complete));

  re(CreateDepthBuffer(
      physical_device, device, surface_extent,
      &depth_image, &depth_memory, &depth_image_view));

  swapchain_image_count = 2;
  re(CreateSwapchain(
      physical_device, device, queue_family, surface, surface_format,
      &swapchain_image_count, &swapchain));

  re(CreateSwapchainImageViews(
      device, swapchain, surface_format, swapchain_image_count,
      &swapchain_images, &swapchain_image_views));

  tr(CreateRenderPass(device, surface_format, &render_pass));

  tr(CreateFramebuffers(
      device, swapchain_image_count, surface_extent, render_pass,
      depth_image_view, swapchain_image_views,
      &swapchain_framebuffers));

  tr(CreatePipelineCache (device, &pipeline_cache));

  return 0;
}
