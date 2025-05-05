#pragma once

#include <scene.h>
#include <vk_config.h>

namespace emt
{
class traiangle_dynamic_rendering : public scene
{
public:
    traiangle_dynamic_rendering(vk_context* context);

    void init_frame() override;
    void update_frame(float dt) override;
    void render_frame() override;
    void release_frame() override;
};
} // namespace emt
