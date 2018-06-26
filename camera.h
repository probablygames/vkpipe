#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct CameraState {
  glm::vec3 pos, dir, up;
  float wx, wy;
};

int CreateCamera(
    CameraState *camera_state,
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory);

void MoveCamera(
    bool w, bool a, bool s, bool d,
    bool z, bool c,
    float dx, float dy,
    CameraState *camera);

int UpdateCameraUniform(
    CameraState *camera,
    VkDevice device,
    VkDeviceMemory uniform_memory);



#endif  // _CAMERA_H_
