#pragma once

#include <vk_config.h>

namespace emt
{
struct vk_pipeline
{
    vk_pipeline();

    static void create_pipeline(
        const uint32_t shader_count, const VkShaderModule* shader_modules,
        VkPipelineVertexInputStateCreateInfo& vertex_input_info,
        VkPipelineLayout piepline_layout,
        VkPipeline* pp_pipeline
    );

    VkPipeline handle;
};

} // namespace emt
