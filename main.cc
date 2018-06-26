
#include "vkutils.h"
#include "nolight.h"
#include "facenormals.h"
#include "camera.h"
#include "vkmacros.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

extern int CreateScene(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkExtent2D surface_extent,

    VkBuffer *vertex_buffer,
    VkDeviceMemory *vertex_memory,
    uint32_t *n_vertices,

    VkImage *texture_image,
    VkDeviceMemory *texture_memory,
    VkImageView *texture_image_view);

struct InputState
{
  struct MousePos {
      double x, y;
      MousePos(double x=0, double y=0) : x(x), y(y) {}
  };
  MousePos mouse;
};

int
HandleInput(
    GLFWwindow *window,
    CameraState *camera,
    VkDevice device,
    VkDeviceMemory vert_uniform_memory)
{
  InputState *input_state = reinterpret_cast<InputState*>(
      glfwGetWindowUserPointer (window));

  bool w = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
  bool a = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
  bool s = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
  bool d = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
  bool z = (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
  bool c = (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS);

  InputState::MousePos prev = input_state->mouse;
  InputState::MousePos curr;
  glfwGetCursorPos (window, &curr.x, &curr.y);
  InputState::MousePos delta (curr.x - prev.x, curr.y - prev.y);
  input_state->mouse = curr;

  MoveCamera(w, a, s, d, z, c, delta.x, delta.y, camera);
  re(UpdateCameraUniform (
        camera, device, vert_uniform_memory));
  return 0;
}

int
CreateLight(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory)
{
  re(CreateAndAllocateUniformVec4Buffer (
      physical_device, device,
      uniform_buffer, uniform_memory));
  struct Light {
    glm::vec3 dir;
    float ambient;
  };

  Light light;
  light.dir = glm::normalize(glm::vec3(0.0f, -4.0f, -1.0f));
  light.ambient = 0.01f;

  re(MapAndInitializeMemory(
      device, *uniform_memory, &light, sizeof(light)));
  return 0;
}

int
main (int argc, char **argv)
{
  VulkanDevice v;
  tr(v.InitializeVulkanDevice (1920, 1080));

  // VulkanPipeline::NoLight p;
  VulkanPipeline::FaceNormals p;

  tr(p.CreateVulkanPipeline (&v));

  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_memory;
  uint32_t n_vertices;

  VkImage texture_image;
  VkDeviceMemory texture_memory;
  VkImageView texture_image_view;
  VkSampler sampler;

  tr(CreateScene(
      v.physical_device, v.device, v.surface_extent,
      &vertex_buffer, &vertex_memory, &n_vertices,
      &texture_image, &texture_memory, &texture_image_view));

  tr(CreateSampler(v.device, &sampler));

  VkBuffer light_buffer;
  VkDeviceMemory light_memory;
  tr(CreateLight (v.physical_device, v.device, &light_buffer, &light_memory));

  CameraState camera_state;
  VkBuffer vert_uniform_buffer;
  VkDeviceMemory vert_uniform_memory;
  tr(CreateCamera(
      &camera_state, 
      v.physical_device, v.device,
      &vert_uniform_buffer, &vert_uniform_memory));

  // tr(p.UpdateDescriptorSet (&v, vert_uniform_buffer, texture_image_view, sampler));
  tr(p.UpdateDescriptorSet (
      &v, vert_uniform_buffer, light_buffer, texture_image_view, sampler));

  InputState input_state;
  glfwSetWindowUserPointer (v.window, &input_state);
  glfwGetCursorPos (v.window, &input_state.mouse.x, &input_state.mouse.y);
  //glfwSetInputMode (v.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  while (!glfwWindowShouldClose(v.window))
  {
    uint32_t n_swapchain_image;
    re(AcquireSwapchainImage(
        v.device, v.swapchain, v.present_complete, &n_swapchain_image));
    re(Draw(
        v.device, v.swapchain, v.command_buffer, v.render_pass,
        p.pipeline, p.pipeline_layout, &p.descriptor_set, v.surface_extent,
        vertex_buffer, n_vertices, texture_image,
        v.depth_image, v.swapchain_images[n_swapchain_image],
        v.swapchain_framebuffers[n_swapchain_image]));

    re(QueueSubmit (v.queue, v.command_buffer, v.present_complete, v.render_complete));
    re(QueuePresent (v.queue, v.swapchain, n_swapchain_image, v.render_complete));

    glfwPollEvents();
    re(HandleInput(v.window, &camera_state, v.device, vert_uniform_memory));
  }

  return 0;
}
