#include "vkutils.h"
#include "vkmacros.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int
CreateScene(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkExtent2D surface_extent,

    VkBuffer *vertex_buffer,
    VkDeviceMemory *vertex_memory,
    uint32_t *n_vertices,

    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory,

    VkImage *texture_image,
    VkDeviceMemory *texture_memory,
    VkImageView *texture_image_view)
{
  *n_vertices = 3;
  uint32_t vertex_buffer_size = 3 * 6 * sizeof(float); // x y z w u v;
  re(CreateAndAllocateVertexBuffer(
      physical_device, device, vertex_buffer_size,
      vertex_buffer, vertex_memory));

  float vertex_data[3 * 6] = {
      -1.0f,  1.0f, 0.5f, 1.0f, 0.0f, 1.0f,
       1.0f,  1.0f, 0.5f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f
  }; 

  re(MapAndInitializeMemory (
      device, *vertex_memory, vertex_data, sizeof(vertex_data)));

  tr(CreateAndAllocateUniform4x4Buffer(
      physical_device, device,
      uniform_buffer, uniform_memory));

  float fov = glm::radians(45.0f);
  if (surface_extent.width > surface_extent.height) {
    fov *= static_cast<float>(surface_extent.height) /
           static_cast<float>(surface_extent.width);
  }

  glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
  glm::mat4 view = glm::lookAt(
      glm::vec3(0, 0, 5),   // Camera is at (0,0,5), in World Space
      glm::vec3(0, 0, 0),   // and looks at the origin
      glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)

  glm::mat4 model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  glm::mat4 clip = glm::mat4(1.0f,  0.0f, 0.0f, 0.0f,
                             0.0f, -1.0f, 0.0f, 0.0f,
                             0.0f,  0.0f, 0.5f, 0.0f,
                             0.0f,  0.0f, 0.5f, 1.0f);

  glm::mat4 mvp = clip * projection * view * model;

  re(MapAndInitializeMemory (device, *uniform_memory, &mvp, sizeof(mvp)));

  VkFormat texture_format = VK_FORMAT_B8G8R8A8_UNORM; //VK_FORMAT_R8G8B8A8_UNORM;
  VkExtent2D tex_extent = surface_extent;
  tr(CreateAndAllocateImage2D(
      physical_device,
      device, tex_extent, texture_format,
      VK_IMAGE_TILING_LINEAR,
      VK_IMAGE_USAGE_SAMPLED_BIT,
      (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      texture_image, texture_memory));

  tr(CreateImage2DView(
      device, *texture_image, texture_format, VK_IMAGE_ASPECT_COLOR_BIT,
      texture_image_view));

  uint32_t texture_size, texture_pitch;
  GetImagePitch (device, *texture_image, &texture_size, &texture_pitch);
  std::unique_ptr<uint8_t []> texture(new uint8_t [texture_size]);

  for (uint32_t x = 0; x < tex_extent.width; ++x) {
    for (uint32_t y = 0; y < tex_extent.height; ++y) {
      uint8_t red = (255 * x) / tex_extent.width;
      uint8_t green = (255 * (tex_extent.width - x)) / tex_extent.width;
      uint8_t blue = (255 * y) / tex_extent.height;
      uint8_t alpha = (255 * (tex_extent.height - y)) / tex_extent.height;

      texture[0 + 4 * x + texture_pitch * y] = blue;
      texture[1 + 4 * x + texture_pitch * y] = green;
      texture[2 + 4 * x + texture_pitch * y] = red;
      texture[3 + 4 * x + texture_pitch * y] = alpha;
    }
  }

  re(MapAndInitializeMemory (
      device, *texture_memory, texture.get(), texture_size));
  return 0;
}
