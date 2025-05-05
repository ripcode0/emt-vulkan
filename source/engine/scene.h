#pragma once

typedef struct VkDevice_T* VkDevice;

namespace emt
{
class vk_context;
class scene{ 
public:
    scene(vk_context* context) : m_context(context){}

    virtual void init_frame() = 0;
    virtual void update_frame(float delta) = 0;
    virtual void render_frame() = 0;
    virtual void release_frame() = 0;

    vk_context* m_context{};
};
} // namespace emt
