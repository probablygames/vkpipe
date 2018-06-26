#include "facenormals.h"
#include "vkutils.h"
#include "vkpipe.h"
#include "vkmacros.h"

static int
CreateShaderModules(
    VkDevice device,
    VkShaderModule *vert,
    VkShaderModule *geom,
    VkShaderModule *frag)
{
  re(CreateShaderModule (device, "facenormals/facenormals.vert.spv", vert));
  re(CreateShaderModule (device, "facenormals/facenormals.geom.spv", geom));
  re(CreateShaderModule (device, "facenormals/facenormals.frag.spv", frag));

  return 0;
}


static int
CreateDescriptorPool (
    VkDevice device,
    VkDescriptorPool *descriptor_pool)
{
  VkDescriptorPoolSize desc_pool_sizes[3];
  desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  desc_pool_sizes[0].descriptorCount = 1;

  desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  desc_pool_sizes[1].descriptorCount = 1;

  desc_pool_sizes[2].type  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  desc_pool_sizes[2].descriptorCount = 1;

  VkDescriptorPoolCreateInfo dpl;
  dpl.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  dpl.pNext = NULL;
  dpl.flags = 0;
  dpl.maxSets = 1;
  dpl.poolSizeCount = 3;
  dpl.pPoolSizes = desc_pool_sizes;

  vk(CreateDescriptorPool (device, &dpl, NULL, descriptor_pool));

  return 0;
}

static int
CreateDescriptorSetLayout (
    VkDevice device,
    VkDescriptorSetLayout *descriptor_set_layout)
{
  VkDescriptorSetLayoutBinding layout_bindings[3];
  layout_bindings[0].binding = 0;
  layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  layout_bindings[0].descriptorCount = 1;
  layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  layout_bindings[0].pImmutableSamplers = NULL;

  layout_bindings[1].binding = 1;
  layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  layout_bindings[1].descriptorCount = 1;
  layout_bindings[1].stageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
  layout_bindings[1].pImmutableSamplers = NULL;

  layout_bindings[2].binding = 2;
  layout_bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  layout_bindings[2].descriptorCount = 1;
  layout_bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  layout_bindings[2].pImmutableSamplers = NULL;

  VkDescriptorSetLayoutCreateInfo dsl;
  dsl.sType= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  dsl.pNext = NULL;  // of VkDescriptorSetLayoutBindingFlagsCreateInfoEXT
  dsl.flags = 0;
  dsl.bindingCount = 3;
  dsl.pBindings = layout_bindings;

  vk(CreateDescriptorSetLayout(device, &dsl, NULL, descriptor_set_layout));
  return 0;
}

static int
UpdateDescriptorSet(
    VkDevice device,
    VkDescriptorSet descriptor_set,
    VkBuffer vert_uniform_buffer,
    VkBuffer geom_uniform_buffer,
    VkImageView texture_image_view,
    VkSampler sampler)
{
  VkWriteDescriptorSet write_descriptor[3];

  VkDescriptorBufferInfo vert_uniform;
  vert_uniform.buffer = vert_uniform_buffer;
  vert_uniform.offset = 0;
  vert_uniform.range = 4 * 4 * sizeof(float); // sizeof(mvp) // glm::mat4

  write_descriptor[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write_descriptor[0].pNext = NULL;
  write_descriptor[0].dstSet = descriptor_set;
  write_descriptor[0].dstBinding = 0;
  write_descriptor[0].dstArrayElement = 0;
  write_descriptor[0].descriptorCount = 1;
  write_descriptor[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  write_descriptor[0].pImageInfo = NULL;
  write_descriptor[0].pBufferInfo = &vert_uniform;
  write_descriptor[0].pTexelBufferView = NULL;


  VkDescriptorBufferInfo geom_uniform;
  geom_uniform.buffer = geom_uniform_buffer;
  geom_uniform.offset = 0;
  geom_uniform.range = 4 * sizeof(float);  // vec3 dir, float ambient

  write_descriptor[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write_descriptor[1].pNext = NULL;
  write_descriptor[1].dstSet = descriptor_set;
  write_descriptor[1].dstBinding = 1;
  write_descriptor[1].dstArrayElement = 0;
  write_descriptor[1].descriptorCount = 1;
  write_descriptor[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  write_descriptor[1].pImageInfo = NULL;
  write_descriptor[1].pBufferInfo = &geom_uniform;
  write_descriptor[1].pTexelBufferView = NULL;


  VkDescriptorImageInfo descriptor_image;
  descriptor_image.sampler = sampler;
  descriptor_image.imageView = texture_image_view;
  descriptor_image.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

  write_descriptor[2].sType =  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write_descriptor[2].pNext = NULL;
  write_descriptor[2].dstSet = descriptor_set;
  write_descriptor[2].dstBinding = 2;
  write_descriptor[2].dstArrayElement = 0;
  write_descriptor[2].descriptorCount = 1;
  write_descriptor[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  write_descriptor[2].pImageInfo = &descriptor_image;
  write_descriptor[2].pBufferInfo = NULL;
  write_descriptor[2].pTexelBufferView = NULL;

  vkUpdateDescriptorSets(device, 3, write_descriptor, 0, NULL);
  return 0;
}

namespace VulkanPipeline
{

int
FaceNormals::
CreateVulkanPipeline(
    VulkanDevice const *v, uint32_t subpass)
{
 re(CreateShaderModules (v->device, &shaders.vert, &shaders.geom, &shaders.frag));

  re(CreateDescriptorPool (v->device, &descriptor_pool));
  re(CreateDescriptorSetLayout (v->device, &descriptor_set_layout));
  re(AllocateDescriptorSets (
      v->device, descriptor_pool, &descriptor_set_layout,
      &descriptor_set));
  re(CreatePipelineLayout (
      v->device, &descriptor_set_layout,
      &pipeline_layout));

  VulkanPipelineConfig config;
  ConfigureVulkanPipelineDefaults (&config);

  VkVertexInputBindingDescription vib;
  VkVertexInputAttributeDescription attributes[2];
  VkPipelineVertexInputStateCreateInfo vertex_input;
  ConfigurePipelineVertexInputVec4Vec2 (&vib, attributes, &vertex_input);

  re(CreateGraphicsPipeline(
      v->device, v->pipeline_cache, &shaders, &vertex_input, &config,
      pipeline_layout, v->render_pass, subpass,
      &pipeline));

  return 0;
}


int
FaceNormals::
UpdateDescriptorSet(
    VulkanDevice const *v,
    VkBuffer vert_uniform_buffer,
    VkBuffer geom_uniform_buffer,
    VkImageView texture_image_view,
    VkSampler sampler)
{
  re(::UpdateDescriptorSet(
      v->device, descriptor_set,
      vert_uniform_buffer, geom_uniform_buffer,
      texture_image_view, sampler));
  
  return 0;
}

}  // namespace VulkanPipeline
