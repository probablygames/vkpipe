#include <vulkan/vulkan.h>
#include <string>

#define c(x) case x: return #x
char const *vkResultString(VkResult result) {
  switch (result) {
    c(VK_SUCCESS);
    c(VK_NOT_READY);
    c(VK_TIMEOUT);
    c(VK_EVENT_SET);
    c(VK_EVENT_RESET);
    c(VK_INCOMPLETE);
    c(VK_ERROR_OUT_OF_HOST_MEMORY);
    c(VK_ERROR_OUT_OF_DEVICE_MEMORY);
    c(VK_ERROR_INITIALIZATION_FAILED);
    c(VK_ERROR_DEVICE_LOST);
    c(VK_ERROR_MEMORY_MAP_FAILED);
    c(VK_ERROR_LAYER_NOT_PRESENT);
    c(VK_ERROR_EXTENSION_NOT_PRESENT);
    c(VK_ERROR_FEATURE_NOT_PRESENT);
    c(VK_ERROR_INCOMPATIBLE_DRIVER);
    c(VK_ERROR_TOO_MANY_OBJECTS);
    c(VK_ERROR_FORMAT_NOT_SUPPORTED);
    c(VK_ERROR_FRAGMENTED_POOL);
    c(VK_ERROR_SURFACE_LOST_KHR);
    c(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
    c(VK_SUBOPTIMAL_KHR);
    c(VK_ERROR_OUT_OF_DATE_KHR);
    c(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
    c(VK_ERROR_VALIDATION_FAILED_EXT);
    c(VK_ERROR_INVALID_SHADER_NV);
    c(VK_ERROR_OUT_OF_POOL_MEMORY_KHR);
    c(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR);
    default: return "Invalid VkResult value";
  }
}

char const *vkPhysicalDeviceTypeString(VkPhysicalDeviceType device_type) {
  switch(device_type) {
    c(VK_PHYSICAL_DEVICE_TYPE_OTHER);
    c(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    c(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    c(VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU );
    c(VK_PHYSICAL_DEVICE_TYPE_CPU);
    default: return "Invalid PhysicalDeviceType value";
  }
}

char const *vkMemoryPropertyFlagBitsString(VkMemoryPropertyFlagBits bit) {
  switch(bit) {
    c(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    c(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    c(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    c(VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
    c(VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT);
    // c(VK_MEMORY_PROPERTY_PROTECTED_BIT);
    default: return "Invalid VkMemoryPropertyFlagBits value";
  }
}

char const *vkMemoryHeapFlagBitsString(VkMemoryHeapFlagBits bit) {
  switch(bit) {
    c(VK_MEMORY_HEAP_DEVICE_LOCAL_BIT);
    // c(VK_MEMORY_HEAP_MULTI_INSTANCE_BIT);
    default: return "Invalid VkMemoryHeapFlagBits value";
  }
}

char const *vkQueueFlagBitsString(VkQueueFlagBits bit) {
  switch(bit) {
    c(VK_QUEUE_GRAPHICS_BIT);
    c(VK_QUEUE_COMPUTE_BIT);
    c(VK_QUEUE_TRANSFER_BIT);
    c(VK_QUEUE_SPARSE_BINDING_BIT);
    // c(VK_QUEUE_PROTECTED_BIT);
    default: return "Invalid VkQueueFlagBits value";
  }
}

char const *vkFormatString(VkFormat format) {
  switch(format) {
    c(VK_FORMAT_UNDEFINED);
    c(VK_FORMAT_R4G4_UNORM_PACK8);
    c(VK_FORMAT_R4G4B4A4_UNORM_PACK16);
    c(VK_FORMAT_B4G4R4A4_UNORM_PACK16);
    c(VK_FORMAT_R5G6B5_UNORM_PACK16);
    c(VK_FORMAT_B5G6R5_UNORM_PACK16);
    c(VK_FORMAT_R5G5B5A1_UNORM_PACK16);
    c(VK_FORMAT_B5G5R5A1_UNORM_PACK16);
    c(VK_FORMAT_A1R5G5B5_UNORM_PACK16);
    c(VK_FORMAT_R8_UNORM);
    c(VK_FORMAT_R8_SNORM);
    c(VK_FORMAT_R8_USCALED);
    c(VK_FORMAT_R8_SSCALED);
    c(VK_FORMAT_R8_UINT);
    c(VK_FORMAT_R8_SINT);
    c(VK_FORMAT_R8_SRGB);
    c(VK_FORMAT_R8G8_UNORM);
    c(VK_FORMAT_R8G8_SNORM);
    c(VK_FORMAT_R8G8_USCALED);
    c(VK_FORMAT_R8G8_SSCALED);
    c(VK_FORMAT_R8G8_UINT);
    c(VK_FORMAT_R8G8_SINT);
    c(VK_FORMAT_R8G8_SRGB);
    c(VK_FORMAT_R8G8B8_UNORM);
    c(VK_FORMAT_R8G8B8_SNORM);
    c(VK_FORMAT_R8G8B8_USCALED);
    c(VK_FORMAT_R8G8B8_SSCALED);
    c(VK_FORMAT_R8G8B8_UINT);
    c(VK_FORMAT_R8G8B8_SINT);
    c(VK_FORMAT_R8G8B8_SRGB);
    c(VK_FORMAT_B8G8R8_UNORM);
    c(VK_FORMAT_B8G8R8_SNORM);
    c(VK_FORMAT_B8G8R8_USCALED);
    c(VK_FORMAT_B8G8R8_SSCALED);
    c(VK_FORMAT_B8G8R8_UINT);
    c(VK_FORMAT_B8G8R8_SINT);
    c(VK_FORMAT_B8G8R8_SRGB);
    c(VK_FORMAT_R8G8B8A8_UNORM);
    c(VK_FORMAT_R8G8B8A8_SNORM);
    c(VK_FORMAT_R8G8B8A8_USCALED);
    c(VK_FORMAT_R8G8B8A8_SSCALED);
    c(VK_FORMAT_R8G8B8A8_UINT);
    c(VK_FORMAT_R8G8B8A8_SINT);
    c(VK_FORMAT_R8G8B8A8_SRGB);
    c(VK_FORMAT_B8G8R8A8_UNORM);
    c(VK_FORMAT_B8G8R8A8_SNORM);
    c(VK_FORMAT_B8G8R8A8_USCALED);
    c(VK_FORMAT_B8G8R8A8_SSCALED);
    c(VK_FORMAT_B8G8R8A8_UINT);
    c(VK_FORMAT_B8G8R8A8_SINT);
    c(VK_FORMAT_B8G8R8A8_SRGB);
    c(VK_FORMAT_A8B8G8R8_UNORM_PACK32);
    c(VK_FORMAT_A8B8G8R8_SNORM_PACK32);
    c(VK_FORMAT_A8B8G8R8_USCALED_PACK32);
    c(VK_FORMAT_A8B8G8R8_SSCALED_PACK32);
    c(VK_FORMAT_A8B8G8R8_UINT_PACK32);
    c(VK_FORMAT_A8B8G8R8_SINT_PACK32);
    c(VK_FORMAT_A8B8G8R8_SRGB_PACK32);
    c(VK_FORMAT_A2R10G10B10_UNORM_PACK32);
    c(VK_FORMAT_A2R10G10B10_SNORM_PACK32);
    c(VK_FORMAT_A2R10G10B10_USCALED_PACK32);
    c(VK_FORMAT_A2R10G10B10_SSCALED_PACK32);
    c(VK_FORMAT_A2R10G10B10_UINT_PACK32);
    c(VK_FORMAT_A2R10G10B10_SINT_PACK32);
    c(VK_FORMAT_A2B10G10R10_UNORM_PACK32);
    c(VK_FORMAT_A2B10G10R10_SNORM_PACK32);
    c(VK_FORMAT_A2B10G10R10_USCALED_PACK32);
    c(VK_FORMAT_A2B10G10R10_SSCALED_PACK32);
    c(VK_FORMAT_A2B10G10R10_UINT_PACK32);
    c(VK_FORMAT_A2B10G10R10_SINT_PACK32);
    c(VK_FORMAT_R16_UNORM);
    c(VK_FORMAT_R16_SNORM);
    c(VK_FORMAT_R16_USCALED);
    c(VK_FORMAT_R16_SSCALED);
    c(VK_FORMAT_R16_UINT);
    c(VK_FORMAT_R16_SINT);
    c(VK_FORMAT_R16_SFLOAT);
    c(VK_FORMAT_R16G16_UNORM);
    c(VK_FORMAT_R16G16_SNORM);
    c(VK_FORMAT_R16G16_USCALED);
    c(VK_FORMAT_R16G16_SSCALED);
    c(VK_FORMAT_R16G16_UINT);
    c(VK_FORMAT_R16G16_SINT);
    c(VK_FORMAT_R16G16_SFLOAT);
    c(VK_FORMAT_R16G16B16_UNORM);
    c(VK_FORMAT_R16G16B16_SNORM);
    c(VK_FORMAT_R16G16B16_USCALED);
    c(VK_FORMAT_R16G16B16_SSCALED);
    c(VK_FORMAT_R16G16B16_UINT);
    c(VK_FORMAT_R16G16B16_SINT);
    c(VK_FORMAT_R16G16B16_SFLOAT);
    c(VK_FORMAT_R16G16B16A16_UNORM);
    c(VK_FORMAT_R16G16B16A16_SNORM);
    c(VK_FORMAT_R16G16B16A16_USCALED);
    c(VK_FORMAT_R16G16B16A16_SSCALED);
    c(VK_FORMAT_R16G16B16A16_UINT);
    c(VK_FORMAT_R16G16B16A16_SINT);
    c(VK_FORMAT_R16G16B16A16_SFLOAT);
    c(VK_FORMAT_R32_UINT);
    c(VK_FORMAT_R32_SINT);
    c(VK_FORMAT_R32_SFLOAT);
    c(VK_FORMAT_R32G32_UINT);
    c(VK_FORMAT_R32G32_SINT);
    c(VK_FORMAT_R32G32_SFLOAT);
    c(VK_FORMAT_R32G32B32_UINT);
    c(VK_FORMAT_R32G32B32_SINT);
    c(VK_FORMAT_R32G32B32_SFLOAT);
    c(VK_FORMAT_R32G32B32A32_UINT);
    c(VK_FORMAT_R32G32B32A32_SINT);
    c(VK_FORMAT_R32G32B32A32_SFLOAT);
    c(VK_FORMAT_R64_UINT);
    c(VK_FORMAT_R64_SINT);
    c(VK_FORMAT_R64_SFLOAT);
    c(VK_FORMAT_R64G64_UINT);
    c(VK_FORMAT_R64G64_SINT);
    c(VK_FORMAT_R64G64_SFLOAT);
    c(VK_FORMAT_R64G64B64_UINT);
    c(VK_FORMAT_R64G64B64_SINT);
    c(VK_FORMAT_R64G64B64_SFLOAT);
    c(VK_FORMAT_R64G64B64A64_UINT);
    c(VK_FORMAT_R64G64B64A64_SINT);
    c(VK_FORMAT_R64G64B64A64_SFLOAT);
    c(VK_FORMAT_B10G11R11_UFLOAT_PACK32);
    c(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32);
    c(VK_FORMAT_D16_UNORM);
    c(VK_FORMAT_X8_D24_UNORM_PACK32);
    c(VK_FORMAT_D32_SFLOAT);
    c(VK_FORMAT_S8_UINT);
    c(VK_FORMAT_D16_UNORM_S8_UINT);
    c(VK_FORMAT_D24_UNORM_S8_UINT);
    c(VK_FORMAT_D32_SFLOAT_S8_UINT);
    c(VK_FORMAT_BC1_RGB_UNORM_BLOCK);
    c(VK_FORMAT_BC1_RGB_SRGB_BLOCK);
    c(VK_FORMAT_BC1_RGBA_UNORM_BLOCK);
    c(VK_FORMAT_BC1_RGBA_SRGB_BLOCK);
    c(VK_FORMAT_BC2_UNORM_BLOCK);
    c(VK_FORMAT_BC2_SRGB_BLOCK);
    c(VK_FORMAT_BC3_UNORM_BLOCK);
    c(VK_FORMAT_BC3_SRGB_BLOCK);
    c(VK_FORMAT_BC4_UNORM_BLOCK);
    c(VK_FORMAT_BC4_SNORM_BLOCK);
    c(VK_FORMAT_BC5_UNORM_BLOCK);
    c(VK_FORMAT_BC5_SNORM_BLOCK);
    c(VK_FORMAT_BC6H_UFLOAT_BLOCK);
    c(VK_FORMAT_BC6H_SFLOAT_BLOCK);
    c(VK_FORMAT_BC7_UNORM_BLOCK);
    c(VK_FORMAT_BC7_SRGB_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK);
    c(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK);
    c(VK_FORMAT_EAC_R11_UNORM_BLOCK);
    c(VK_FORMAT_EAC_R11_SNORM_BLOCK);
    c(VK_FORMAT_EAC_R11G11_UNORM_BLOCK);
    c(VK_FORMAT_EAC_R11G11_SNORM_BLOCK);
    c(VK_FORMAT_ASTC_4x4_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_4x4_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_5x4_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_5x4_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_5x5_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_5x5_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_6x5_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_6x5_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_6x6_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_6x6_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_8x5_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_8x5_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_8x6_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_8x6_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_8x8_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_8x8_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_10x5_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_10x5_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_10x6_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_10x6_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_10x8_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_10x8_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_10x10_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_10x10_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_12x10_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_12x10_SRGB_BLOCK);
    c(VK_FORMAT_ASTC_12x12_UNORM_BLOCK);
    c(VK_FORMAT_ASTC_12x12_SRGB_BLOCK);
    default: return "Invalid VkFormat value";
  }
}

char const *vkColorSpaceKHRString(VkColorSpaceKHR color_space) {
  switch(color_space) {
    c(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    default: return "Invalid VkColorSpaceKHR value";
  }
}

char const *vkPresentModeKHRString(VkPresentModeKHR mode) {
  switch(mode) {
    c(VK_PRESENT_MODE_IMMEDIATE_KHR);
    c(VK_PRESENT_MODE_MAILBOX_KHR);
    c(VK_PRESENT_MODE_FIFO_KHR);
    c(VK_PRESENT_MODE_FIFO_RELAXED_KHR);
    default: return "Invalid VkPresentModeKHR value";
  }
}

char const *vkSurfaceTransformFlagBitsKHRString(VkSurfaceTransformFlagBitsKHR bit) {
  switch(bit) {
    c(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR);
    c(VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR);
    default: return "Invalid VkSurfaceTransformFlagBitsKHR value";
  }
}

char const *vkCompositeAlphaFlagBitsKHRString(VkCompositeAlphaFlagBitsKHR bit) {
  switch(bit) {
    c(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    c(VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR);
    c(VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR);
    c(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR);
    default: return "Invalid VkCompositeAlphaFlagBitsKHR value";
  }
}

char const *vkImageUsageFlagBitsString(VkImageUsageFlagBits bit) {
  switch(bit) {
    c(VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
    c(VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    c(VK_IMAGE_USAGE_SAMPLED_BIT);
    c(VK_IMAGE_USAGE_STORAGE_BIT);
    c(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    c(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    c(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT);
    c(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    default: return "Invalid VkImageUsageFlagBits value";
  }
}

char const * vkFormatFeatureFlagBitsString(VkFormatFeatureFlagBits bit) {
  switch(bit) {
    c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    c(VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT);
    c(VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT);
    c(VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
    c(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT);
    c(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT);
    c(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
    c(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);
    c(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT);
    c(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    c(VK_FORMAT_FEATURE_BLIT_SRC_BIT);
    c(VK_FORMAT_FEATURE_BLIT_DST_BIT);
    c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
    // c(VK_FORMAT_FEATURE_TRANSFER_SRC_BIT);
    // c(VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
    // c(VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT);
    // c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT);
    // c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT);
    // c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT);
    // c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT);
    // c(VK_FORMAT_FEATURE_DISJOINT_BIT);
    // c(VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT);
    /// c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG);
    // c(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT);
    default: return "Invalid VkFormatFeatureFlagBits value";
  }
}
#undef c

template<typename FlagsT, typename BitsT>
std::string FlagsString(
    BitsT const bits[], uint32_t nbits, FlagsT flags,
    char const* (*string_fn)(BitsT)) {
  std::string flags_string("");
  for (uint32_t b = 0; b < nbits; ++b) {
    if (!(flags & bits[b])) {
      continue;
    }
    if (flags_string.size() > 0) {
      flags_string += ",";
    }
    flags_string += string_fn(bits[b]);
  }
  return flags_string;
}
    

std::string vkMemoryPropertyFlagsString(VkMemoryPropertyFlags flags) {
  VkMemoryPropertyFlagBits bits[] = {
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
      VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
      // VK_MEMORY_PROPERTY_PROTECTED_BIT,
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkMemoryPropertyFlagBitsString);
}

std::string vkMemoryHeapFlagsString(VkMemoryHeapFlags flags) {
  VkMemoryHeapFlagBits bits[] = {
    VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
    // VK_MEMORY_HEAP_MULTI_INSTANCE_BIT
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkMemoryHeapFlagBitsString);
}

std::string vkQueueFlagsString(VkQueueFlags flags) {
  VkQueueFlagBits bits[] = {
    VK_QUEUE_GRAPHICS_BIT,
    VK_QUEUE_COMPUTE_BIT,
    VK_QUEUE_TRANSFER_BIT,
    VK_QUEUE_SPARSE_BINDING_BIT,
    // VK_QUEUE_PROTECTED_BIT,
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkQueueFlagBitsString);
}

std::string vkSurfaceTransformFlagsKHRString(VkSurfaceTransformFlagsKHR flags) {
  VkSurfaceTransformFlagBitsKHR bits[] = {
    VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
    VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
    VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
    VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
    VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
    VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
    VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
    VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkSurfaceTransformFlagBitsKHRString);
}

std::string vkCompositeAlphaFlagsKHRString(VkCompositeAlphaFlagsKHR flags) {
  VkCompositeAlphaFlagBitsKHR bits[] = {
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkCompositeAlphaFlagBitsKHRString);
}

std::string vkImageUsageFlagsString(VkImageUsageFlags flags) {
  VkImageUsageFlagBits bits[] = {
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_USAGE_STORAGE_BIT,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkImageUsageFlagBitsString);
}

std::string vkFormatFeatureFlagsString(VkFormatFeatureFlags flags) {
  VkFormatFeatureFlagBits bits[] = {
    VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
    VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
    VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
    VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
    VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
    VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
    VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
    VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_FORMAT_FEATURE_BLIT_SRC_BIT,
    VK_FORMAT_FEATURE_BLIT_DST_BIT,
    VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
    // VK_FORMAT_FEATURE_TRANSFER_SRC_BIT,
    // VK_FORMAT_FEATURE_TRANSFER_DST_BIT,
    // VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,
    // VK_FORMAT_FEATURE_DISJOINT_BIT,
    // VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG,
    // VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT,
  };
  uint32_t const nbits = sizeof(bits) / sizeof(bits[0]);
  return FlagsString(bits, nbits, flags, vkFormatFeatureFlagBitsString);
}

std::string vkPhysicalDeviceFeaturesString(VkPhysicalDeviceFeatures const& device_features) {
  std::string features_string("");
  uint32_t num_features = 0;
#define f(feature) \
  if (device_features.feature) { \
    if (num_features++ > 0) \
      features_string += ","; \
    features_string += #feature; \
  }

  f(robustBufferAccess);
  f(fullDrawIndexUint32);
  f(imageCubeArray);
  f(independentBlend);
  f(geometryShader);
  f(tessellationShader);
  f(sampleRateShading);
  f(dualSrcBlend);
  f(logicOp);
  f(multiDrawIndirect);
  f(drawIndirectFirstInstance);
  f(depthClamp);
  f(depthBiasClamp);
  f(fillModeNonSolid);
  f(depthBounds);
  f(wideLines);
  f(largePoints);
  f(alphaToOne);
  f(multiViewport);
  f(samplerAnisotropy);
  f(textureCompressionETC2);
  f(textureCompressionASTC_LDR);
  f(textureCompressionBC);
  f(occlusionQueryPrecise);
  f(pipelineStatisticsQuery);
  f(vertexPipelineStoresAndAtomics);
  f(fragmentStoresAndAtomics);
  f(shaderTessellationAndGeometryPointSize);
  f(shaderImageGatherExtended);
  f(shaderStorageImageExtendedFormats);
  f(shaderStorageImageMultisample);
  f(shaderStorageImageReadWithoutFormat);
  f(shaderStorageImageWriteWithoutFormat);
  f(shaderUniformBufferArrayDynamicIndexing);
  f(shaderSampledImageArrayDynamicIndexing);
  f(shaderStorageBufferArrayDynamicIndexing);
  f(shaderStorageImageArrayDynamicIndexing);
  f(shaderClipDistance);
  f(shaderCullDistance);
  f(shaderFloat64);
  f(shaderInt64);
  f(shaderInt16);
  f(shaderResourceResidency);
  f(shaderResourceMinLod);
  f(sparseBinding);
  f(sparseResidencyBuffer);
  f(sparseResidencyImage2D);
  f(sparseResidencyImage3D);
  f(sparseResidency2Samples);
  f(sparseResidency4Samples);
  f(sparseResidency8Samples);
  f(sparseResidency16Samples);
  f(sparseResidencyAliased);
  f(variableMultisampleRate);
  f(inheritedQueries);
#undef f
  return features_string;
}

