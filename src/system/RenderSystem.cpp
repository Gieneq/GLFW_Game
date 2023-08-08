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
    auto projRect = Rect2F{eyeRect.top_left.x, -eyeRect.top_left.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});
    //reflect with y axis
    // projRect.top_left.y = -projRect.top_left.y - projRect.size.h;

    /** 
     * Filter notvisible objects.
     * Check if left < -1 or right > 1 or top < -1 or bottom > 1
     * */

    if(projRect.right() < -1.0F || projRect.left() > 1.0F || projRect.bottom() < -1.0F || projRect.top() > 1.0F) {
        //todo consider moving to world space
        return;
    }

    glColor3f(1.0F, 1.0F, 1.0F);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, textureData.id.value);
    glEnable(GL_TEXTURE_2D);
        
    int u_idx = tilesetIndex % textureData.tilesPerRow;
    int v_idx = tilesetIndex / textureData.tilesPerRow;
    // float u1 = static_cast<float>(u_idx) / static_cast<float>(textureData.tileWidth);
    // float u2 = static_cast<float>(u_idx + 1) / static_cast<float>(textureData.tileWidth);
    // float v1 = static_cast<float>(v_idx) / static_cast<float>(textureData.tileHeight);
    // float v2 = static_cast<float>(v_idx + 1) / static_cast<float>(textureData.tileHeight);
    float u1 = static_cast<float>((u_idx + 0) * textureData.tileWidth) / static_cast<float>(textureData.width);
    float u2 = static_cast<float>((u_idx + 1) * textureData.tileWidth) / static_cast<float>(textureData.width);
    float v1 = static_cast<float>((v_idx + 0) * textureData.tileHeight) / static_cast<float>(textureData.height);
    float v2 = static_cast<float>((v_idx + 1) * textureData.tileHeight) / static_cast<float>(textureData.height);

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
    glDisable(GL_BLEND);



    glColor3f(1.0F, 0.0F, 0.0F);
    float vertices[] = {
        projRect.left(), projRect.bottom(), // Bottom-left vertex
        projRect.left(), projRect.top(), // Top-left vertex
        projRect.right(), projRect.top(), // Top-right vertex
        projRect.right(), projRect.bottom()  // Bottom-right vertex
    };
    glEnableClientState(GL_VERTEX_ARRAY);

    // Define the vertex array data
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    // Draw the rectangle using GL_LINE_LOOP to form the borders
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    // Disable vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
}

void RenderSystem::renderFilledBox(Rect2F worldRect, float r, float g, float b) {
    //on input for example (0,0) x (1,1) in world space
    auto eyeRect = worldRect.get_translated(camera->position.get_negated()).get_scaled(camera->zoom);
    //here (0,0) x (1,1) in eye space if camera is pointing (0,0)
    //then get scaled with camera->zoom to (0,0) x (0.1, 0.1)
    auto projRect = Rect2F{eyeRect.top_left.x, -eyeRect.top_left.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});
    //herecanbe (0,0) x (0.05, 0.1) if aspect ratio is 2.0 - smaller width for long display
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
