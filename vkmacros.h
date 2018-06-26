#ifndef _VKMACROS_H_
#define _VKMACROS_H_

#include <iostream>

#include "vkstrings.h"

#define vk(cmd) \
do { \
  VkResult result = vk ## cmd ; \
  if (VK_SUCCESS != result) { \
    std::cerr << __FILE__ << " : " << __LINE__  \
        << "  \"" << #cmd << "\" " << vkResultString(result) << std::endl; \
    return -1; \
  } \
} while (0)


#define re(cmd) \
do { \
  int result = cmd ; \
  if (0 != result) { \
    std::cerr << __FILE__ << " : " << __LINE__  \
        << "  \"" << #cmd << "\" " << result << std::endl; \
    return result; \
  } \
} while(0)

#define tr(cmd) \
do { \
  std::cerr << __FILE__ << " : " << __LINE__  << "  \"" << #cmd << "\" "  << std::endl; \
  re(cmd); \
  std::cerr << __FILE__ << " : " << __LINE__  << " OK" << std::endl; \
} while(0)


#endif // _VKMACROS_H_
