#include "RenderSystem.h"
#include "GLCommon.h"
#include "Window.h"
#include "Loader.h"
#include "GraphicsComponent.h"

void RenderSystem::init() {
    setViewportDimensions(Window::width(), Window::height());
}

void RenderSystem::setViewportDimensions(int width, int height) {
    viewport_width = width;
    viewport_height = height;
    aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
}

void RenderSystem::attachCamera(Camera *cam) {
    camera = cam;
}


void RenderSystem::renderTexturedBox(const TextureData& textureData, const Rect2F& worldRect, int tilesetIndex) {
    auto eyeRect = worldRect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
    auto projRect = Rect2F{eyeRect.top_left.x, -eyeRect.top_left.y, eyeRect.size.w, eyeRect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});

    glColor3f(1.0F, 1.0F, 1.0F);

    glBindTexture(GL_TEXTURE_2D, textureData.id.value);
    glEnable(GL_TEXTURE_2D);
        
    int u_idx = tilesetIndex % textureData.div_width;
    int v_idx = tilesetIndex / textureData.div_width;

    float u1 = static_cast<float>(u_idx) / static_cast<float>(textureData.div_width);
    float u2 = static_cast<float>(u_idx + 1) / static_cast<float>(textureData.div_width);
    float v1 = static_cast<float>(v_idx) / static_cast<float>(textureData.div_height);
    float v2 = static_cast<float>(v_idx + 1) / static_cast<float>(textureData.div_height);

    glBegin(GL_QUADS);
    glTexCoord2f(u1, v1);
    glVertex2f(projRect.left(), projRect.bottom());
    glTexCoord2f(u1, v2);
    glVertex2f(projRect.left(), projRect.top());
    glTexCoord2f(u2, v2);
    glVertex2f(projRect.right(), projRect.top());
    glTexCoord2f(u2, v1);
    glVertex2f(projRect.right(), projRect.bottom());
    glEnd();


    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderSystem::renderFilledBox(Rect2F worldRect, float r, float g, float b) {
    auto eyeRect = worldRect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
    auto projRect = Rect2F{eyeRect.top_left.x, -eyeRect.top_left.y, eyeRect.size.w, eyeRect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(projRect.left(), projRect.bottom());
    glVertex2f(projRect.left(), projRect.top());
    glVertex2f(projRect.right(), projRect.top());
    glVertex2f(projRect.right(), projRect.bottom());
    glEnd();
}


void RenderSystem::render(Entity* entity) {
    /* Textured quad rendering */
    auto texture_component = entity->getComponent<TextureComponent>();
    if(texture_component) {

        /* Check if has valid texture */
        auto textureData = Loader::getLoader().getTextureDataByID(texture_component->textureID);
        if(!textureData) {
            /* Corrupted */
            renderFilledBox(texture_component->getWorldRect(), 1.0F, 0.0F, 0.0F);
        }
        else {
            renderTexturedBox(*textureData, texture_component->getWorldRect(), texture_component->tilesetIndex);
        }
    }

    /* Colored quad rendering */
    auto color_component = entity->getComponent<ColorComponent>();
    if(color_component) {
        renderFilledBox(color_component->getWorldRect(), color_component->r, color_component->g, color_component->b);
    }
}














    // /* Simple color quad rendering */
    // auto color_component = entity->getComponent<ColorComponent>();
    // if(color_component) {
    //     glColor3f(color_component->r, color_component->g, color_component->b);

    //     auto world_rect = color_component->worldRect;
    //     auto eye_rect = world_rect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
    //     auto proj_rect = Rect2F{eye_rect.top_left.x, -eye_rect.top_left.y, eye_rect.size.w, eye_rect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});


    //     glBegin(GL_QUADS);
    //     glVertex2f(proj_rect.left(), proj_rect.bottom());
    //     glVertex2f(proj_rect.left(), proj_rect.top());
    //     glVertex2f(proj_rect.right(), proj_rect.top());
    //     glVertex2f(proj_rect.right(), proj_rect.bottom());
    //     glEnd();
    // }

    // auto texture_component = entity->getComponent<TextureComponent>();
    // if(texture_component) {
    //     auto world_rect = texture_component->worldRect;
    //     auto eye_rect = world_rect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
    //     auto proj_rect = Rect2F{eye_rect.top_left.x, -eye_rect.top_left.y, eye_rect.size.w, eye_rect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});

    //     // auto& loader = Loader::get_loader();
    //     // texture_id = texture_component->texture_id;

    //     // auto texture_id = loader.get_image(texture_component->).texture_id;
    //     glColor3f(1.0F, 1.0F, 1.0F);

    //     glBindTexture(GL_TEXTURE_2D, texture_component->textureID.value);
    //     glEnable(GL_TEXTURE_2D);
        
    //     float corner = 1.0f/64;

    //     glBegin(GL_QUADS);
    //     glTexCoord2f(0.0F, 0.0F);
    //     glVertex2f(proj_rect.left(), proj_rect.bottom());
    //     glTexCoord2f(0.0F, corner);
    //     glVertex2f(proj_rect.left(), proj_rect.top());
    //     glTexCoord2f(corner, corner);
    //     glVertex2f(proj_rect.right(), proj_rect.top());
    //     glTexCoord2f(corner, 0.0F);
    //     glVertex2f(proj_rect.right(), proj_rect.bottom());
    //     glEnd();


    // glDisable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, 0);
    // }

// glActiveTexture(GL_TEXTURE0);
// glBindTexture(GL_TEXTURE_2D, textureID);
// glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);
