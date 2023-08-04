#include "RenderSystem.h"
#include "GLCommon.h"
#include "Window.h"
#include "Loader.h"

void RenderSystem::init() {
    set_viewport_size(Window::width(), Window::height());

    auto texture_id = Loader::get_loader().load_image_from_resources("tiles.png");
    auto texture_data = Loader::get_loader().get_texture_data(texture_id);
    std::string texture_name{"some_tiles"};
    if(Loader::get_loader().register_texture_name(texture_name, texture_id)) {
        std::cout << "Registered texture: " << texture_id << "as " << texture_name << std::endl;
    }
    else {
        std::cout << "Failed to register texture name: " << texture_id << std::endl;
    }
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
    /* Simple color quad rendering */
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

    /* Texxtured quad rendering */
    auto texture_component = entity->getComponent<TextureComponent>();
    if(texture_component) {
        auto world_rect = texture_component->rect;
        auto eye_rect = world_rect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
        auto proj_rect = Rect2F{eye_rect.top_left.x, -eye_rect.top_left.y, eye_rect.size.w, eye_rect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});

        // auto& loader = Loader::get_loader();
        // texture_id = texture_component->texture_id;

        // auto texture_id = loader.get_image(texture_component->).texture_id;
        glColor3f(1.0F, 1.0F, 1.0F);

        glBindTexture(GL_TEXTURE_2D, texture_component->texture_id.id);
        glEnable(GL_TEXTURE_2D);
        
        float corner = 1.0f/64;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0F, 0.0F);
        glVertex2f(proj_rect.left(), proj_rect.bottom());
        glTexCoord2f(0.0F, corner);
        glVertex2f(proj_rect.left(), proj_rect.top());
        glTexCoord2f(corner, corner);
        glVertex2f(proj_rect.right(), proj_rect.top());
        glTexCoord2f(corner, 0.0F);
        glVertex2f(proj_rect.right(), proj_rect.bottom());
        glEnd();


    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    }
}





// glActiveTexture(GL_TEXTURE0);
// glBindTexture(GL_TEXTURE_2D, textureID);
// glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);
