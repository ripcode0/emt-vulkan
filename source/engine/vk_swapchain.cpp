#include "vk_swapchain.h"
#include <vk_context.h>

namespace emt
{
    vk_swapchain::vk_swapchain(const vk_context *context, uint32_t cx, uint32_t cy)
        : m_context(context), m_device(context->m_device)
    {
        assert(m_device && "a");
    }

} // namespace emt
