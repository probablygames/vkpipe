#ifndef _VKPIPE_H_
#define _VKPIPE_H_

#include <vulkan/vulkan.h>

int AllocateDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptor_pool,
    VkDescriptorSetLayout const *descriptor_set_layout,
    VkDescriptorSet *descriptor_set);

int CreatePipelineLayout(
    VkDevice device,
    VkDescriptorSetLayout const *descriptor_set_layout,
    VkPipelineLayout *pipeline_layout);

struct VulkanShaderModules
{
  VkShaderModule vert;
  VkShaderModule tesc; 
  VkShaderModule tese; 
  VkShaderModule geom; 
  VkShaderModule frag; 

  VulkanShaderModules() : vert(0), tesc(0), tese(0), geom(0), frag(0) {}
};

void ConfigurePipelineVertexInputVec4Vec2 (
    VkVertexInputBindingDescription *vib,
    VkVertexInputAttributeDescription attributes[2],
    VkPipelineVertexInputStateCreateInfo *pvi);

void ConfigurePipelineShaderStages(
    VulkanShaderModules const *shader_modules,
    uint32_t *shader_stage_count,
    VkPipelineShaderStageCreateInfo *stages);

void ConfigurePipelineInputAssemblyState(
    VkPipelineInputAssemblyStateCreateInfo *pia);

void ConfigurePipelineRasterizationState(
    VkPipelineRasterizationStateCreateInfo *prs);

void ConfigurePipelineColorBlendAttachmentState(
    VkPipelineColorBlendAttachmentState *cba);

void ConfigurePipelineColorBlendState(
    VkPipelineColorBlendAttachmentState const *cba,
    VkPipelineColorBlendStateCreateInfo *pcb);

void ConfigurePipelineViewportState(
    VkPipelineViewportStateCreateInfo *pvs);

void ConfigurePipelineDynamicState(
    VkDynamicState dynamic_states[2],
    VkPipelineDynamicStateCreateInfo *pds);

void ConfigurePipelineDepthStencilState(
    VkPipelineDepthStencilStateCreateInfo *dss);

void ConfigurePipelineMultisampleState(
    VkPipelineMultisampleStateCreateInfo *pms);


struct VulkanPipelineConfig
{
  // All arguments to CreateGraphicsPipelines except for
  // the shader-dependent input binding and shader modules.
  // InputAssembly is maybe shader-dependent.
  VkPipelineInputAssemblyStateCreateInfo pia;
  VkPipelineViewportStateCreateInfo pvs;
  VkPipelineRasterizationStateCreateInfo prs;
  VkPipelineMultisampleStateCreateInfo pms;
  VkPipelineDepthStencilStateCreateInfo dss;
  VkPipelineColorBlendAttachmentState cba;
  VkPipelineColorBlendStateCreateInfo pcb;
  VkDynamicState dynamic_states[2];
  VkPipelineDynamicStateCreateInfo pds;
};

void ConfigureVulkanPipelineDefaults(
    VulkanPipelineConfig *config);

int CreateGraphicsPipeline(
    VkDevice device,
    VkPipelineCache pipeline_cache,
    VulkanShaderModules const *shaders,
    VkPipelineVertexInputStateCreateInfo const *vertex_input,
    VulkanPipelineConfig const *config,
    VkPipelineLayout pipeline_layout,
    VkRenderPass render_pass,
    uint32_t subpass,
    VkPipeline *pipeline);



#endif  // _VKPIPE_H_
