#ifndef _VKSTRINGS_H_
#define _VKSTRINGS_H_

#include <vulkan/vulkan.h>
#include <string>

char const *vkResultString(VkResult result);
char const *vkPhysicalDeviceTypeString(VkPhysicalDeviceType device_type);
char const *vkMemoryPropertyFlagBitsString(VkMemoryPropertyFlagBits bit);
char const *vkMemoryHeapFlagBitsString(VkMemoryHeapFlagBits bit);
char const *vkQueueFlagBitsString(VkQueueFlagBits bit);
char const *vkFormatString(VkFormat format);
char const *vkColorSpaceKHRString(VkColorSpaceKHR color_space);
char const *vkPresentModeKHRString(VkPresentModeKHR mode);
char const *vkSurfaceTransformFlagBitsKHRString(VkSurfaceTransformFlagBitsKHR bit);
char const *vkCompositeAlphaFlagBitsKHRString(VkCompositeAlphaFlagBitsKHR bit);
char const *vkImageUsageFlagBitsString(VkImageUsageFlagBits bit);
char const *vkFormatFeatureFlagBitsString(VkFormatFeatureFlagBits bit);

std::string vkMemoryPropertyFlagsString(VkMemoryPropertyFlags flags);
std::string vkMemoryHeapFlagsString(VkMemoryHeapFlags flags);
std::string vkQueueFlagsString(VkQueueFlags flags);
std::string vkSurfaceTransformFlagsKHRString(VkSurfaceTransformFlagsKHR flags);
std::string vkCompositeAlphaFlagsKHRString(VkCompositeAlphaFlagsKHR flags);
std::string vkImageUsageFlagsString(VkImageUsageFlags flags);
std::string vkFormatFeatureFlagsString(VkFormatFeatureFlags flags);
std::string vkPhysicalDeviceFeaturesString(VkPhysicalDeviceFeatures const& device_features);


#endif  // _VKSTRINGS_H_
