#include "vk_command_list.h"

namespace emt
{
    vk_command_list::vk_command_list(const VkDevice device, const VkCommandPool pool)
        : m_device(device), m_command_pool(pool)
    {
    }

} // namespace emt
