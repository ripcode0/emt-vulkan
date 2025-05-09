#include "vk_context.h"
#include <vector>
#include <iostream>
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include <vk_config.h>
#include <vk_init.h>
#include <vk_debug.h>
#include <span>

namespace emt
{
    vk_context::vk_context(uint32_t cx, uint32_t cy, HWND hwnd)
        : m_cx(cx), m_cy(cy), m_hwnd(hwnd)
    {
        create_instance();

        create_debug_callback();

        create_surface();

        create_device();

        //test


        create_command_pool();

        create_swapchain();

        create_render_pass();

        create_command_buffers();

        create_sync_objects();

        // init sub vulkan tools, vma ..etc
        vk::initialize(this);

    }

    vk_context::~vk_context()
    {
        //vkDestroyInstance(m_instance, nullptr);
        vkWaitForFences(m_device, 2, m_fence, VK_TRUE, VK_DEFAULT_FENCE_TIMEOUT);
        
        
    }

    void vk_context::create_instance()
    {
        uint32_t version = VK_HEADER_VERSION_COMPLETE;

        printf("vulkan sdk version %d.%d.%d\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

        std::vector<const char*> instance_layers = {
            "VK_LAYER_KHRONOS_validation"
        };
        std::vector<const char*> instance_extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
        #if defined(_WIN32)
            "VK_KHR_win32_surface",
        #endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };
  
        VkApplicationInfo info{};
        info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        info.pApplicationName = "emt-vulkan";
        info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        info.pEngineName = "vulkan=engine";
        info.engineVersion = VK_MAKE_VERSION(1,0,0);
        info.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &info;
        create_info.enabledLayerCount = instance_layers.size();
        create_info.ppEnabledLayerNames = instance_layers.data();
        create_info.enabledExtensionCount = instance_extensions.size();
        create_info.ppEnabledExtensionNames = instance_extensions.data();

        VK(vkCreateInstance(&create_info, nullptr, &m_instance));
        
        int a = 0;
    }

    void vk_context::create_debug_callback(){

        // VkDebugReportCallbackCreateInfoEXT is deprecated
        // VkDebugUtilsMessengerCreateInfoEXT is newer

        VkDebugUtilsMessengerCreateInfoEXT debug_info{};
        debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_info.pfnUserCallback = vk_debug::debug_utils_message_callback;

        //auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

        //__vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
        //VK_PFUNC(m_instance, PFN_vkCreateDebugReportCallbackEXT, vkCreateDebugUtilsMessengerEXT);
        vk_load_instance_func(m_instance, vkCreateDebugUtilsMessengerEXT);

        VK(vkCreateDebugUtilsMessengerEXT(m_instance, &debug_info, nullptr, &m_debug_messenger));
    }

    void vk_context::create_surface()
    {   
        VkWin32SurfaceCreateInfoKHR surface_info{};
        surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surface_info.hinstance = GetModuleHandle(nullptr);
        surface_info.hwnd = m_hwnd;

        VK(vkCreateWin32SurfaceKHR(m_instance, &surface_info, nullptr, &m_surface));
    }

    void vk_context::create_device()
    {
        const std::vector<const char*> device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            //"VK_KHR_dynamic_rendering"
        };

        get_physical_device_from_instance(m_instance, m_surface, &m_physical_device);

        // check the 1.3 dynamic rendering
        VkPhysicalDeviceDynamicRenderingFeatures dynamic_rendering_feats{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES};
        dynamic_rendering_feats.dynamicRendering = TRUE;
        
        
        VkPhysicalDeviceFeatures2 feats{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
        };
        feats.features = {};
        feats.pNext = &dynamic_rendering_feats;

        float queue_priority = 1.0f;

        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = m_physical_device.family_queue_index;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &queue_priority;

        VkDeviceCreateInfo device_info{};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.pNext = &feats;
        device_info.queueCreateInfoCount = 1;
        device_info.pQueueCreateInfos = &queue_info;
        device_info.enabledExtensionCount = device_extensions.size();
        device_info.ppEnabledExtensionNames = device_extensions.data();

        VK(vkCreateDevice(m_physical_device, &device_info, nullptr, &m_device));
      
        
        vkGetDeviceQueue(m_device, m_physical_device.family_queue_index, 0, &m_graphics_queue);


        
    }

    void vk_context::get_physical_device_from_instance(
        const VkInstance instance, const VkSurfaceKHR surface, vk_physical_device* gpu_device)
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
        std::vector<VkPhysicalDevice> phy_devices(device_count);
        vkEnumeratePhysicalDevices(instance, &device_count, phy_devices.data());

        for(const auto& device : phy_devices){
            uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

            for(uint32_t i = 0; i < queue_family_count; ++i){
                VkBool32 present_support = false;
                VkBool32 is_discrete = false;

                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

                VkPhysicalDeviceProperties props{};
                vkGetPhysicalDeviceProperties(device, &props);

                if(props.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    is_discrete = true;
                
                VkQueueFlags flags = queue_families[i].queueFlags;    
                if( (flags & VK_QUEUE_GRAPHICS_BIT) &&
                    (flags & VK_QUEUE_TRANSFER_BIT) &&
                    (flags & VK_QUEUE_COMPUTE_BIT) && present_support && is_discrete)
                {
                    
                    printf("found the complete queue index %s : %d\n", props.deviceName, i);
                    gpu_device->handle = device;
                    gpu_device->family_queue_index = i;
                    gpu_device->flags = flags;
                }
            }
        }

        
        vkGetPhysicalDeviceMemoryProperties(m_physical_device.handle, &m_physical_device.memory_props);
    }

    void vk_context::create_command_pool()
    {
        VkCommandPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.queueFamilyIndex = m_physical_device.family_queue_index;
        info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VK(vkCreateCommandPool(m_device, &info, nullptr, &m_command_pool));

    }

    void vk_context::create_swapchain()
    {
        VkSurfaceCapabilitiesKHR caps{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, m_surface, &caps);

        VkExtent2D extent {
          m_cx, m_cy  
        };

        uint32_t image_count = caps.minImageCount + 1;
        if(caps.maxImageCount > 0 && image_count > caps.maxImageCount){
            image_count = caps.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchain_info{};
        swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_info.surface = m_surface;
        swapchain_info.minImageCount = image_count;
        swapchain_info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
        swapchain_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        swapchain_info.imageExtent = extent;
        swapchain_info.imageArrayLayers = 1;
        swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_info.preTransform = caps.currentTransform;
        swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
        swapchain_info.clipped = VK_TRUE;
        swapchain_info.oldSwapchain = VK_NULL_HANDLE;

        VK(vkCreateSwapchainKHR(m_device, &swapchain_info, nullptr, &m_swapchain));

        printf("[emt] swapchain is created\n");
        printf("[emt] image count : %d\n", (int)image_count);

        
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
        m_swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

        printf("[emt] VkImage : %d\n", (int)m_swapchain_images.size());

        m_swapchain_image_views.resize(image_count);

        for(uint32_t i =0; i < image_count; ++i){
            VkImageViewCreateInfo view_info{};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = m_swapchain_images[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = VK_FORMAT_B8G8R8A8_SRGB;
            view_info.components = { VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY
            };
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;

            VK(vkCreateImageView(m_device, &view_info, nullptr, &m_swapchain_image_views[i]));
        }
    }

    void vk_context::create_command_buffers()
    {
        //m_command_buffers.resize(m_swapchain_image_views.size());
        m_command_buffers.resize(MAX_SYNC_FRAMES);

        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = (uint32_t)m_command_buffers.size();

        VK(vkAllocateCommandBuffers(m_device, &alloc_info, m_command_buffers.data()));
    }

    void vk_context::create_render_pass()
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = VK_FORMAT_B8G8R8A8_SRGB;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_ref{};
        color_ref.attachment = 0;
        color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_ref;

        VkRenderPassCreateInfo renderpass_info{};
        renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_info.attachmentCount = 1;
        renderpass_info.pAttachments = &color_attachment;
        renderpass_info.subpassCount = 1;
        renderpass_info.pSubpasses = &subpass;

        VK(vkCreateRenderPass(m_device, &renderpass_info, nullptr, &m_render_pass));
    }

    void vk_context::create_sync_objects()
    {
        VkSemaphoreCreateInfo semaphore_info{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        VkFenceCreateInfo fence_info(VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(uint32_t i = 0; i < MAX_SYNC_FRAMES; ++i){
            VK(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_present_semaphore[i]));
            VK(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_semaphore[i]));
            VK(vkCreateFence(m_device, &fence_info, nullptr, &m_fence[i]));
        }


    }

    void vk_context::record_cmd_buffer(VkCommandBuffer cmd, uint32_t image_index)
    {
        VkCommandBufferBeginInfo cmd_begin_info{};
        cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VK(vkBeginCommandBuffer(cmd, &cmd_begin_info));

        VkRenderingAttachmentInfo color_attach{};
        color_attach.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        color_attach.imageView = m_swapchain_image_views[image_index];
        color_attach.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        color_attach.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attach.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attach.clearValue = { {0.2f, 0.2f, 0.2, 1.f}};

        VkRenderingInfo rendering_info{};
        rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        rendering_info.renderArea.extent = {m_cx, m_cy};


    }

    void vk_context::record_cmd_buffer_13(VkCommandBuffer cmd)
    {
        VkCommandBufferBeginInfo cmd_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        //cmd_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(cmd, &cmd_info);

        

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        //barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.image = m_swapchain_images[m_image_index];
        barrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        vkCmdPipelineBarrier(
            cmd,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            0, nullptr, 0, nullptr,
            1, &barrier
        );

        VkRenderingAttachmentInfo color_info{};
        color_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        color_info.imageView = m_swapchain_image_views[m_image_index];
        color_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        color_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_info.clearValue = {{0.3f, 0.2f, 0.2f, 1.f}};

        VkRenderingInfo render_info{};
        render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        render_info.renderArea.extent = {m_cx, m_cy};
        render_info.layerCount = 1;
        render_info.colorAttachmentCount = 1;
        render_info.pColorAttachments = &color_info;

        vkCmdBeginRendering(cmd, &render_info);

        VkClearAttachment clear_attachment{};
        clear_attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clear_attachment.colorAttachment = 0;
        clear_attachment.clearValue.color = {{1.0f,0.f,0.f,1.f}};

        VkClearRect rc{};
        rc.rect.offset = {0,0};
        rc.rect.extent = {m_cx, m_cy};
        rc.baseArrayLayer = 0;
        rc.layerCount = 1;

        vkCmdClearAttachments(cmd, 1, &clear_attachment, 1,&rc);
        

        vkCmdEndRendering(cmd);

        //VkImageMemoryBarrier barrier{};
        //barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        //barrier.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = 0;
        barrier.image = m_swapchain_images[m_image_index];
        barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

        vkCmdPipelineBarrier(cmd, 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, nullptr, 0, nullptr, 1, &barrier);
        
        vkEndCommandBuffer(cmd);
    }

    void vk_context::begin_frame()
    {
        vkWaitForFences(m_device, 1, &m_fence[m_current_frame], VK_TRUE, UINT64_MAX);
        VK(vkResetFences(m_device, 1, &m_fence[m_current_frame]));

        
        VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, 
            m_present_semaphore[m_current_frame], VK_NULL_HANDLE ,&m_image_index);
        
        if(res == VK_ERROR_OUT_OF_DATE_KHR){

            int a =0;
        }
        VkCommandBuffer cmd = get_command_buffer();

        VkCommandBufferBeginInfo cmd_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        //cmd_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(cmd, &cmd_info);

        

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        //barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.image = m_swapchain_images[m_image_index];
        barrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        vkCmdPipelineBarrier(
            cmd,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            0, nullptr, 0, nullptr,
            1, &barrier
        );

        VkRenderingAttachmentInfo color_info{};
        color_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        color_info.imageView = m_swapchain_image_views[m_image_index];
        color_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        color_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_info.clearValue = {{0.3f, 0.2f, 0.2f, 1.f}};

        VkRenderingInfo render_info{};
        render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        render_info.renderArea.extent = {m_cx, m_cy};
        render_info.layerCount = 1;
        render_info.colorAttachmentCount = 1;
        render_info.pColorAttachments = &color_info;

        //TODO : replace
        vkCmdBeginRendering(cmd, &render_info);

    }

    void vk_context::end_frame()
    {
        const VkCommandBuffer cmd = get_command_buffer();

        vkCmdEndRendering(cmd);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = 0;
        barrier.image = m_swapchain_images[m_image_index];
        barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

        vkCmdPipelineBarrier(cmd, 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, nullptr, 0, nullptr, 1, &barrier);
        
        vkEndCommandBuffer(cmd);
        // const VkCommandBuffer cmd = m_command_buffers[m_current_frame];
        // record_cmd_buffer_13(cmd);
        VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pWaitDstStageMask = &wait_stage_mask;
        submit_info.pCommandBuffers = &cmd;
        submit_info.commandBufferCount = 1;
        submit_info.pWaitSemaphores = &m_present_semaphore[m_current_frame];
        submit_info.waitSemaphoreCount = 1;

        submit_info.pSignalSemaphores = &m_render_semaphore[m_current_frame];
        submit_info.signalSemaphoreCount = 1;

        VK(vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_fence[m_current_frame]));

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount =1;
        present_info.pWaitSemaphores = &m_render_semaphore[m_current_frame];
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swapchain;
        present_info.pImageIndices = &m_image_index;

        vkQueuePresentKHR(m_graphics_queue, &present_info);


        m_current_frame = (m_current_frame + 1) % MAX_SYNC_FRAMES;

        //vkDeviceWaitIdle(m_device);
    }

    VkCommandBuffer vk_context::get_command_buffer()
    {
        return m_command_buffers[m_current_frame];
    }

    uint32_t vk_context::get_memory_type_index(uint32_t type_bit, VkMemoryPropertyFlags props)
    {
        for(uint32_t i = 0; i < m_physical_device.memory_props.memoryTypeCount; i++){
            if((type_bit & 1) == 1){
                if((m_physical_device.memory_props.memoryTypes[i].propertyFlags & props) == props){
                    return i;
                }
            }
            type_bit >>= 1;
        }
        throw "could not find a suitable memory type";
    }

} // namespace emt
