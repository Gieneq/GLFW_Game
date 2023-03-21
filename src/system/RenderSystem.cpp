#include "RenderSystem.h"
#include <GLFW/glfw3.h>
#include "../core/Window.h"
// #include "linmath.h"

void RenderSystem::init() {
    set_viewport_size(Window::width(), Window::height());
}

void RenderSystem::set_viewport_size(int width, int height) {
    viewport_width = width;
    viewport_height = height;
    aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
}

void RenderSystem::attach_camera(Camera *cam) {
    camera = cam;
}

void RenderSystem::render(Entity* entity) {
    auto color_component = entity->getComponent<ColorComponent>();
    if(color_component) {
        glColor3f(color_component->r, color_component->g, color_component->b);

        auto world_rect = color_component->rect;
        auto eye_rect = world_rect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
        auto proj_rect = Rect2F{eye_rect.top_left.x, -eye_rect.top_left.y, eye_rect.size.w, eye_rect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});


        glBegin(GL_QUADS);
        glVertex2f(proj_rect.left(), proj_rect.bottom());
        glVertex2f(proj_rect.left(), proj_rect.top());
        glVertex2f(proj_rect.right(), proj_rect.top());
        glVertex2f(proj_rect.right(), proj_rect.bottom());
        glEnd();
    }
}