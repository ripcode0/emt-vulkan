#include "vk_pipeline.h"

namespace emt
{
    void vk_pipeline::create_pipeline(
            const uint32_t shader_count, 
            const VkShaderModule* shader_modules,
            VkPipelineVertexInputStateCreateInfo& vertex_input_info,
            VkPipelineLayout piepline_layout,
            VkPipeline* pp_pipeline
            )
    {
        VkPipelineInputAssemblyStateCreateInfo input_assembly{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;
        

        const VkDynamicState dynamic_states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dynamic_state.dynamicStateCount = _countof(dynamic_states);
        dynamic_state.pDynamicStates = dynamic_states;

        VkPipelineViewportStateCreateInfo viewport_state{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo raster_state{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        raster_state.polygonMode = VK_POLYGON_MODE_FILL;
        raster_state.cullMode = VK_CULL_MODE_BACK_BIT;
        raster_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        raster_state.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo multisample_state{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorblend_attachment{};
        colorblend_attachment.colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        
        VkPipelineColorBlendStateCreateInfo colorblend_state{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        colorblend_state.attachmentCount = 1;
        colorblend_state.pAttachments = &colorblend_attachment;

        // dynamic rendering structure

        VkFormat color_format = VK_FORMAT_R8G8B8A8_UNORM;

        VkPipelineRenderingCreateInfo rendering_info{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
        rendering_info.colorAttachmentCount = 1;
        rendering_info.pColorAttachmentFormats = &color_format;

        VkPipelineShaderStageCreateInfo shader_stages[2] = {};
        shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages[0].module = shader_modules[0];
        shader_stages[0].pName = "main";

        shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages[1].module = shader_modules[1];
        shader_stages[1].pName = "main";


        VkGraphicsPipelineCreateInfo pipeline_info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};

        pipeline_info.pNext = &rendering_info;
        pipeline_info.stageCount = shader_count;
        pipeline_info.pStages = shader_stages;
        pipeline_info.pVertexInputState = &vertex_input_info;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &raster_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &colorblend_state;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = piepline_layout;
        pipeline_info.renderPass = VK_NULL_HANDLE;

        VkPipeline pipeline{};

        
        
    }

} // namespace emt
