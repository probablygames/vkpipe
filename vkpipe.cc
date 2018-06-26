
#include "vkpipe.h"
#include "vkutils.h"
#include "vkmacros.h"

int
AllocateDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptor_pool,
    VkDescriptorSetLayout const *descriptor_set_layout,
    VkDescriptorSet *descriptor_set)
{
  VkDescriptorSetAllocateInfo dsa;
  dsa.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  dsa.pNext = NULL;  //  VkDescriptorSetVariableDescriptorCountAllocateInfoEXT
  dsa.descriptorPool = descriptor_pool;
  dsa.descriptorSetCount = 1;
  dsa.pSetLayouts = descriptor_set_layout;

  vk(AllocateDescriptorSets (device, &dsa, descriptor_set));
  return 0;
}

int
CreatePipelineLayout(
    VkDevice device,
    VkDescriptorSetLayout const *descriptor_set_layout,
    VkPipelineLayout *pipeline_layout)
{
  VkPipelineLayoutCreateInfo plc;
  plc.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  plc.pNext = NULL;
  plc.flags = 0;
  plc.setLayoutCount = 1;
  plc.pSetLayouts = descriptor_set_layout;
  plc.pushConstantRangeCount = 0;
  plc.pPushConstantRanges = NULL;

  vk(CreatePipelineLayout( device, &plc, NULL, pipeline_layout));
  return 0;
}

void ConfigurePipelineVertexInputVec4Vec2 (
    VkVertexInputBindingDescription *vib,
    VkVertexInputAttributeDescription attributes[2],
    VkPipelineVertexInputStateCreateInfo *pvi)
{
  vib->binding = 0;
  vib->stride = 6 * sizeof(float); // x y z w u v; vertex_size_bytes;
  vib->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  attributes[0].location = 0;
  attributes[0].binding = 0;
  attributes[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attributes[0].offset = 0;

  attributes[1].location = 1;
  attributes[1].binding = 0;
  attributes[1].format = VK_FORMAT_R32G32_SFLOAT;
  attributes[1].offset = 4 * sizeof(float);

  pvi->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  pvi->pNext = NULL;  // VkPipelineVertexInputDivisorStateCreateInfoEXT
  pvi->flags = 0;
  pvi->vertexBindingDescriptionCount = 1;
  pvi->pVertexBindingDescriptions = vib;
  pvi->vertexAttributeDescriptionCount = 2;
  pvi->pVertexAttributeDescriptions = attributes;
}

void
ConfigurePipelineShaderStages(
    VulkanShaderModules const *shader_modules,
    uint32_t *shader_stage_count,
    VkPipelineShaderStageCreateInfo *stages)
{
  uint32_t k = 0;
  if (shader_modules->vert != VK_NULL_HANDLE) {
    stages[k].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[k].pNext = NULL;
    stages[k].flags = 0;
    stages[k].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[k].module = shader_modules->vert;
    stages[k].pName = "main";
    stages[k].pSpecializationInfo = NULL;
    ++k;
  }

  if (shader_modules->geom != VK_NULL_HANDLE) {
    stages[k].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[k].pNext = NULL;
    stages[k].flags = 0;
    stages[k].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
    stages[k].module = shader_modules->geom;
    stages[k].pName = "main";
    stages[k].pSpecializationInfo = NULL;
    ++k;
  }

  if (shader_modules->frag != VK_NULL_HANDLE) {
    stages[k].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[k].pNext = NULL;
    stages[k].flags = 0;
    stages[k].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[k].module = shader_modules->frag;
    stages[k].pName = "main";
    stages[k].pSpecializationInfo = NULL;
    ++k;
  }

  *shader_stage_count = k;
}

void
ConfigurePipelineInputAssemblyState(
    VkPipelineInputAssemblyStateCreateInfo *pia)
{
  pia->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  pia->pNext = NULL;
  pia->flags = 0;
  pia->topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pia->primitiveRestartEnable = VK_FALSE;
}

void
ConfigurePipelineRasterizationState(
    VkPipelineRasterizationStateCreateInfo *prs)
{
  prs->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  prs->pNext = NULL;
  prs->flags = 0;
  prs->depthClampEnable = VK_TRUE;
  prs->rasterizerDiscardEnable = VK_FALSE;
  prs->polygonMode = VK_POLYGON_MODE_FILL;
  prs->cullMode = VK_CULL_MODE_BACK_BIT;
  prs->frontFace = VK_FRONT_FACE_CLOCKWISE;
  prs->depthBiasEnable = VK_FALSE;
  prs->depthBiasConstantFactor = 0.0f;
  prs->depthBiasClamp = 0.0f;
  prs->depthBiasSlopeFactor = 0.0f;
  prs->lineWidth = 1.0f;
}

void
ConfigurePipelineColorBlendAttachmentState(
    VkPipelineColorBlendAttachmentState *cba)
{
  cba->blendEnable = VK_FALSE;
  cba->srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  cba->dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  cba->colorBlendOp = VK_BLEND_OP_ADD;
  cba->srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  cba->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  cba->alphaBlendOp = VK_BLEND_OP_ADD;
  cba->colorWriteMask = (
      VK_COLOR_COMPONENT_R_BIT |
      VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT |
      VK_COLOR_COMPONENT_A_BIT);
}

void
ConfigurePipelineColorBlendState(
    VkPipelineColorBlendAttachmentState const *cba,
    VkPipelineColorBlendStateCreateInfo *pcb)
{
  pcb->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  pcb->pNext = NULL;
  pcb->flags = 0;
  pcb->logicOpEnable = VK_FALSE;
  pcb->logicOp = VK_LOGIC_OP_CLEAR;
  pcb->attachmentCount = 1;
  pcb->pAttachments = cba;
  pcb->blendConstants[0] = 1.0f;
  pcb->blendConstants[1] = 1.0f;
  pcb->blendConstants[2] = 1.0f;
  pcb->blendConstants[3] = 1.0f;
}

void
ConfigurePipelineViewportState(
    VkPipelineViewportStateCreateInfo *pvs)
{
  pvs->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  pvs->pNext = NULL;
  pvs->flags = 0;
  pvs->viewportCount = 1;
  pvs->pViewports = NULL;
  pvs->scissorCount = 1;
  pvs->pScissors = NULL;
}

void
ConfigurePipelineDynamicState(
    VkDynamicState dynamic_states[2],
    VkPipelineDynamicStateCreateInfo *pds)
{
  dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
  dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

  pds->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  pds->pNext = NULL;
  pds->flags = 0;
  pds->dynamicStateCount = 2;
  pds->pDynamicStates = dynamic_states;
}

void
ConfigureStencilOpState (
    VkStencilOpState *sos)
{
  sos->failOp = VK_STENCIL_OP_KEEP;
  sos->passOp = VK_STENCIL_OP_KEEP;
  sos->depthFailOp = VK_STENCIL_OP_KEEP;
  sos->compareOp = VK_COMPARE_OP_ALWAYS;
  sos->compareMask = 0;
  sos->writeMask = 0;
  sos->reference = 0;
}

void
ConfigurePipelineDepthStencilState(
    VkPipelineDepthStencilStateCreateInfo *dss)
{
  dss->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  dss->pNext = NULL;
  dss->flags = 0;
  dss->depthTestEnable = VK_FALSE;
  dss->depthWriteEnable = VK_FALSE;
  dss->depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  dss->depthBoundsTestEnable = VK_FALSE;
  dss->stencilTestEnable = VK_FALSE;
  ConfigureStencilOpState(&dss->front);
  ConfigureStencilOpState(&dss->back);
  dss->minDepthBounds = 0.0f;
  dss->maxDepthBounds = 0.0f;
}

void
ConfigurePipelineMultisampleState(
    VkPipelineMultisampleStateCreateInfo *pms)
{
  pms->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  pms->pNext = NULL;
  pms->flags = 0;
  pms->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  pms->sampleShadingEnable = VK_FALSE;
  pms->minSampleShading = 0.0f;
  pms->pSampleMask = NULL;
  pms->alphaToCoverageEnable = VK_FALSE;
  pms->alphaToOneEnable = VK_FALSE;
}

void
ConfigureVulkanPipelineDefaults(
    VulkanPipelineConfig *config)
{
  ConfigurePipelineInputAssemblyState(&config->pia);
  ConfigurePipelineViewportState (&config->pvs);
  ConfigurePipelineRasterizationState (&config->prs);
  ConfigurePipelineMultisampleState (&config->pms);
  ConfigurePipelineDepthStencilState (&config->dss);
  ConfigurePipelineColorBlendAttachmentState (&config->cba);
  ConfigurePipelineColorBlendState (&config->cba, &config->pcb);
  ConfigurePipelineDynamicState (config->dynamic_states, &config->pds);
}

int
CreateGraphicsPipeline(
    VkDevice device,
    VkPipelineCache pipeline_cache,
    VulkanShaderModules const *shaders,
    VkPipelineVertexInputStateCreateInfo const *vertex_input,
    VulkanPipelineConfig const *config,
    VkPipelineLayout pipeline_layout,
    VkRenderPass render_pass,
    uint32_t subpass,
    VkPipeline *pipeline)
{
  uint32_t shader_count;
  VkPipelineShaderStageCreateInfo shader_stages[5];
  ConfigurePipelineShaderStages (shaders, &shader_count, shader_stages);

  VkGraphicsPipelineCreateInfo gpc[1];
  gpc->sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  gpc->pNext = NULL;
  gpc->flags = 0;
  gpc->stageCount = shader_count;
  gpc->pStages = shader_stages;
  gpc->pVertexInputState = vertex_input;
  gpc->pInputAssemblyState = &config->pia;
  gpc->pTessellationState = NULL;
  gpc->pViewportState = &config->pvs;
  gpc->pRasterizationState = &config->prs;
  gpc->pMultisampleState = &config->pms;
  gpc->pDepthStencilState = &config->dss;
  gpc->pColorBlendState = &config->pcb;
  gpc->pDynamicState = &config->pds;
  gpc->layout = pipeline_layout;
  gpc->renderPass = render_pass;
  gpc->subpass = subpass;
  gpc->basePipelineHandle = VK_NULL_HANDLE;
  gpc->basePipelineIndex = 0;

  vk(CreateGraphicsPipelines(device, pipeline_cache, 1, gpc, NULL, pipeline));
  return 0;
}
