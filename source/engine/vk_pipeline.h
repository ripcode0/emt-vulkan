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

struct vk_input_layout
{
    vk_input_layout(){
        input_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        bindings.clear();
        attribs.clear();
    }

    void binding(const VkVertexInputBindingDescription& bind_desc){
        bindings.emplace_back(bind_desc);
    };

    void attribute(const VkVertexInputAttributeDescription& attr_desc){
        attribs.emplace_back(attr_desc);
    }

    operator VkPipelineVertexInputStateCreateInfo&()  {
        input_state_info.vertexBindingDescriptionCount = (uint32_t)bindings.size();
        input_state_info.pVertexBindingDescriptions = bindings.data();
        input_state_info.vertexAttributeDescriptionCount = (uint32_t)attribs.size();
        input_state_info.pVertexAttributeDescriptions = attribs.data();
        return input_state_info;
    };

private:
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attribs;
    VkPipelineVertexInputStateCreateInfo input_state_info{};
};


} // namespace emt
