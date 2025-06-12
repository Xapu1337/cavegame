#ifndef OOGABOOGA_GFX_IMPL_VULKAN_H
#define OOGABOOGA_GFX_IMPL_VULKAN_H

#include <stdbool.h>
#include <stdint.h>

// Vulkan graphics implementation for Linux
// This header defines the Vulkan-specific interface that is used internally
// but should not be exposed to application code

// Forward declarations
typedef struct VkInstance_T* VkInstance;
typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkFramebuffer_T* VkFramebuffer;
typedef struct VkPipeline_T* VkPipeline;
typedef struct VkPipelineLayout_T* VkPipelineLayout;

// Vulkan state structure (internal only)
typedef struct Vulkan_State {
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physical_device;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    VkRenderPass render_pass;
    VkFramebuffer* framebuffers;
    VkPipeline graphics_pipeline;
    VkPipelineLayout pipeline_layout;
    
    uint32_t graphics_queue_family;
    uint32_t present_queue_family;
    uint32_t swapchain_image_count;
    uint32_t current_frame;
    
    bool initialized;
} Vulkan_State;

// Internal Vulkan functions (not exposed to app)
bool vulkan_init(void);
void vulkan_shutdown(void);
bool vulkan_create_swapchain(void);
void vulkan_destroy_swapchain(void);
bool vulkan_begin_frame(void);
void vulkan_end_frame(void);
void vulkan_present(bool vsync);

// Window integration (platform-specific)
bool vulkan_create_surface(void* windowHandle);
void vulkan_destroy_surface(void);

#endif // OOGABOOGA_GFX_IMPL_VULKAN_H
