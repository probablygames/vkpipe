#ifndef _VKUTILS_H_
#define _VKUTILS_H_

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <memory>


int CreateVulkanInstance (VkInstance *instance);

int CreateDebugReportCallback(
    VkInstance instance,
    VkDebugReportCallbackEXT *callback);

int CreatePhysicalDevice(
    VkInstance instance,
    VkPhysicalDevice *physical_device,
    uint32_t *queue_family,
    VkPhysicalDeviceFeatures *device_features);

int SelectMemoryTypeWithProperties(
    VkPhysicalDevice physical_device,
    VkMemoryRequirements mem_reqs,
    VkMemoryPropertyFlags properties,
    uint32_t *memory_type);

int CreateWindowSurface(
    VkInstance instance,
    VkPhysicalDevice physical_device,
    VkFormat surface_format,
    VkExtent2D surface_extent,
    GLFWwindow **window,
    VkSurfaceKHR *surface);

int CreateDevice(
    VkPhysicalDevice physical_device,
    VkPhysicalDeviceFeatures device_features,
    uint32_t queue_family,
    VkDevice *device);

int CreateCommandPoolAndQueue(
    VkDevice device,
    uint32_t queue_family,
    VkCommandPool *command_pool,
    VkQueue *queue);

int CreateCommandBuffer(
    VkDevice device,
    VkCommandPool command_pool,
    VkCommandBuffer *command_buffer);

int CreateSemaphore(
    VkDevice device,
    VkSemaphore *semaphore);

int CreateSwapchain(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t queue_family,
    VkSurfaceKHR surface,
    VkFormat surface_format,
    uint32_t *image_count,
    VkSwapchainKHR *swapchain);

int AllocateMemory(
    VkPhysicalDevice physical_device,
    VkMemoryRequirements const& mem_reqs,
    VkMemoryPropertyFlags properties,
    VkDevice device,
    VkDeviceMemory *memory);

int MapAndInitializeMemory(
    VkDevice device,
    VkDeviceMemory memory,
    void const *data,
    uint32_t size);

int CreateBuffer(
    VkDevice device,
    uint32_t buffer_size,
    VkBufferUsageFlags usage,
    VkBuffer *buffer);

int CreateAndAllocateBuffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t buffer_size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer *buffer,
    VkDeviceMemory *memory);

int CreateAndAllocateVertexBuffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    uint32_t buffer_size,
    VkBuffer *vertex_buffer,
    VkDeviceMemory *vertex_memory);

int CreateAndAllocateUniformMat4Buffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory);

int CreateAndAllocateUniformVec4Buffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkBuffer *uniform_buffer,
    VkDeviceMemory *uniform_memory);

int CreateImage2D(
    VkDevice device,
    VkExtent2D extent,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkImage *image);

int CreateAndAllocateImage2D(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkExtent2D extent,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImage *image, VkDeviceMemory *memory);

int CreateImage2DView(
    VkDevice device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect_mask,
    VkImageView *image_view);

int CreateDepthBuffer(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkExtent2D extent,
    VkImage *depth_image,
    VkDeviceMemory *depth_memory,
    VkImageView *depth_image_view);

int CreateSwapchainImageViews (
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkFormat surface_format,
    uint32_t image_count,
    std::unique_ptr<VkImage[]> *swapchain_images,
    std::unique_ptr<VkImageView[]> *swapchain_image_views);

int CreateRenderPass(
    VkDevice device,
    VkFormat surface_format,
    VkRenderPass *render_pass);

int CreateShaderModule(
    VkDevice device,
    char const *spirv_filename,
    VkShaderModule *shader_module);

int CreateFramebuffers(
    VkDevice device,
    uint32_t image_count,
    VkExtent2D surface_extent,
    VkRenderPass render_pass,
    VkImageView depth_image_view,
    std::unique_ptr<VkImageView[]> const& swapchain_image_views,
    std::unique_ptr<VkFramebuffer[]> *swapchain_framebuffers);

int CreateSampler(
    VkDevice device,
    VkSampler *sampler);

int CreatePipelineCache(
    VkDevice device,
    VkPipelineCache *pipeline_cache);

int AcquireSwapchainImage(
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkSemaphore present_complete,
    uint32_t *n_swapchain_image);

void BeginCommandBuffer(VkCommandBuffer command_buffer);

void ConfigureImageMemoryBarrier(
    VkImage image, VkImageAspectFlags aspect,
    VkAccessFlags src_access,  VkImageLayout old_layout,
    VkAccessFlags dst_access,  VkImageLayout new_layout,
    VkImageMemoryBarrier *imb);

void TopOfPipelineImageMemoryBarriers(
    VkCommandBuffer command_buffer,
    VkImage depth_image,
    VkImage texture_image,
    VkImage swapchain_image);

void BeginRenderPass(
    VkCommandBuffer command_buffer,
    VkRenderPass render_pass,
    VkFramebuffer framebuffer,
    VkExtent2D surface_extent);

void SetViewportAndScissor(
    VkCommandBuffer command_buffer,
    VkExtent2D surface_extent);

void BindGraphicsPipeline(
    VkCommandBuffer command_buffer,
    VkPipeline pipeline,
    VkPipelineLayout pipeline_layout,
    VkDescriptorSet const *descriptor_set);

void DrawVertexBuffer(
    VkCommandBuffer command_buffer,
    VkBuffer vertex_buffer,
    uint32_t n_vertices);

void EndRenderPass(
    VkCommandBuffer command_buffer);

void BottomOfPipelineImageMemoryBarriers(
    VkCommandBuffer command_buffer,
    VkImage swapchain_image);

int QueueSubmit(
    VkQueue queue,
    VkCommandBuffer command_buffer,
    VkSemaphore present_complete,
    VkSemaphore render_complete);

int QueuePresent(
    VkQueue queue,
    VkSwapchainKHR swapchain,
    uint32_t n_swapchain_image,
    VkSemaphore render_complete);

void GetImagePitch(
    VkDevice device,
    VkImage image,
    uint32_t *size, uint32_t *pitch);

int Draw(
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkCommandBuffer command_buffer,
    VkRenderPass render_pass,
    VkPipeline pipeline,
    VkPipelineLayout pipeline_layout,
    VkDescriptorSet const *descriptor_set,
    VkExtent2D surface_extent,
    VkBuffer vertex_buffer,
    uint32_t n_vertices,
    VkImage texture_image,
    VkImage depth_image,
    VkImage swapchain_image,
    VkFramebuffer framebuffer);


#endif  // _VKUTILS_H_

