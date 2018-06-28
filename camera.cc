#include "camera.h"

#include "vkutils.h"
#include "vkmacros.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <math.h>
#include <sys/time.h>

#include <iostream>

void
InitializeCameraState(
    CameraState *camera)
{
  camera->pos = glm::vec3(0, 0, -1.1);  // Camera position in world space.
  camera->dir = glm::vec3(0, 0, 1);   // and looks at the origin
  camera->up = glm::vec3(0, 1, 0);    // Head is up (set to 0,-1,0 to look upside-down)
  camera->wx = 0.0f;
  camera->wy = 0.0f;
}

/*
  The "canonical" openGL camera matix 
    projection = glm::perspective(fov, aspect, near, far)
    view = glm::lookAt(cam_pos, look_at, cam_up)
    model = .... scene specific ...
    mvp = projection * view * model

    gl_Position = mvp * vec4(modelspace_vertex, 1)

  e.g. http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
*/

void
ComputeCameraMatrix(
    CameraState const *camera,
    glm::mat4 *camera_matrix)
{
  float fov = M_PI / 3.0;

  *camera_matrix = 
      glm::perspective(fov, 1920.0f / 1080.0f, 0.0001f, 10.0f) *
      glm::lookAt(
          camera->pos,
          glm::normalize(camera->pos + camera->dir),
          camera->up) *
      // Vulkan clip space has inverted Y and half Z.
      glm::mat4(1.0f,  0.0f, 0.0f, 0.0f,
                0.0f, -1.0f, 0.0f, 0.0f,
                0.0f,  0.0f, 0.5f, 0.0f,
                0.0f,  0.0f, 0.5f, 1.0f) *
      glm::rotate(camera->wx, glm::vec3(1, 1, 0)) *
      glm::rotate(camera->wy, glm::vec3(0, 1, 0));
      
}

void MoveCamera(
    bool w, bool a, bool s, bool d,
    bool z, bool c, float dx, float dy,
    CameraState *camera)
{
/*
  float speed = 0.01;
  glm::vec3 left = glm::normalize(glm::cross(camera->up, camera->dir));

  if (w) { camera->pos += speed * glm::normalize(camera->dir); }
  if (a) { camera->pos -= speed * glm::normalize(camera->dir); }
  if (s) { camera->pos += speed * glm::normalize(left); }
  if (d) { camera->pos -= speed * glm::normalize(left); }
  if (z) { camera->pos += speed * glm::normalize(camera->up); }
  if (c) { camera->pos -= speed * glm::normalize(camera->up); }
  float rotspeed = 0.001;
*/
  float rspeed = 5e-2;

  if (w) { camera->wy += rspeed ; }
  if (s) { camera->wy -= rspeed ; }
  if (a) { camera->wx += rspeed ; }
  if (d) { camera->wx -= rspeed ; }
}


int
CreateCamera(
    CameraState *camera_state,
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory)
{
  tr(CreateAndAllocateUniformMat4Buffer(
      physical_device, device,
      uniform_buffer, uniform_memory));

  InitializeCameraState(camera_state);
  tr(UpdateCameraUniform(camera_state, device, *uniform_memory));
    
  return 0;
}

int
UpdateCameraUniform(
    CameraState *camera_state,
    VkDevice device,
    VkDeviceMemory uniform_memory)
{
  glm::mat4 camera_matrix;
  ComputeCameraMatrix(camera_state, &camera_matrix);

  re(MapAndInitializeMemory (
      device, uniform_memory, &camera_matrix, sizeof(camera_matrix)));
  return 0;
}
