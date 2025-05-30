#pragma once

#include <scene.h>
#include <vk_config.h>

namespace emt
{
struct vertex
{
    float pos[3];
    float color[3];
};

class traiangle_dynamic_rendering : public scene
{
public:
    traiangle_dynamic_rendering(vk_context* context);

    void init_frame() override;
    void update_frame(float dt) override;
    void render_frame() override;
    void release_frame() override;

    //vk_buffer m_vertex_buffer{};
    //vk_buffer m_index_buffer{};
    

    VkBuffer m_vertex_buffer{};
    VkBuffer m_index_buffer{};
    uint32_t m_indices{0};

    VkShaderModule m_vs_shader{};
    VkShaderModule m_ps_shader{};

    VkPipelineLayout m_pipeline_layout{};
    VkPipeline m_pipeline{};

private:
    void create_buffers();
    //void create_buffers1();
    void create_pipeline();
};
} // namespace emt
