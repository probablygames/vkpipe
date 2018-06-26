#ifndef _VULKAN_PIPELINE_NOLIGHT_H_
#define _VULKAN_PIPELINE_NOLIGHT_H_

#include <vulkan/vulkan.h>

#include "vkdevice.h"
#include "vkpipe.h"

namespace VulkanPipeline
{

struct NoLight
{
  VulkanShaderModules shaders;
  VkDescriptorPool descriptor_pool;
  VkDescriptorSetLayout descriptor_set_layout;
  VkDescriptorSet descriptor_set;
  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;

  int CreateVulkanPipeline(
      VulkanDevice const *vulkan_device,
      uint32_t subpass = 0);

  int UpdateDescriptorSet(
      VulkanDevice const *vulkan_device,
      VkBuffer uniform_buffer,
      VkImageView texture_image_view,
      VkSampler sampler);
};

};

#endif  // _VULKAN_PIPELINE_NOLIGHT_H_
