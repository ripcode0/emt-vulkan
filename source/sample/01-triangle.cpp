#include "01-triangle.h"
#include <vk_context.h>

namespace emt
{
    traiangle_dynamic_rendering::traiangle_dynamic_rendering(vk_context* context)
        : scene(context)
    {
    }

    void traiangle_dynamic_rendering::init_frame()
    {
        printf("the scene initialiezed\n");
    }

    void traiangle_dynamic_rendering::update_frame(float dt)
    {
    }

    void traiangle_dynamic_rendering::render_frame()
    {
        const VkCommandBuffer cmd = m_context->get_command_buffer();

        VkClearAttachment color_att{};
        color_att.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_att.clearValue = {{0.2,0.4,0.2,1.0f}};
        color_att.colorAttachment = 0;

        VkClearRect rc{};
        rc.rect.extent = {m_context->m_cx, m_context->m_cy};
        rc.rect.offset = {0,0};
        rc.layerCount = 1;
        rc.baseArrayLayer = 0;

        vkCmdClearAttachments(cmd, 1, &color_att, 1, &rc);
        

    }

    void traiangle_dynamic_rendering::release_frame()
    {
    }

} // namespace emt
