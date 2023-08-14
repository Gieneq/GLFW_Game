#include "RenderSystemBase.h"
#include "GLCommon.h"
#include "Window.h"
#include "Loader.h"
#include "GraphicsComponent.h"
#include "CollisionComponents.h"
#include "Entity.h"
#include <algorithm>

#define DEBUG_TEXTURE_BORDERS

void RenderSystemBase::init() {
    setViewportDimensions(Window::width(), Window::height());
}

void RenderSystemBase::loopStart() {

}

void RenderSystemBase::loopEnd() {

}

void RenderSystemBase::setViewportDimensions(int width, int height) {
    viewport_width = width;
    viewport_height = height;
    aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
}

void RenderSystemBase::attachCamera(Camera *cam) {
    camera = cam;
}

Rect2F RenderSystemBase::getProjectionRect(const Rect2F& worldRect) const {
    // get_translated(camera->position.getXY().get_negated()).get_scaled(camera->zoom);
    // auto projRect = Rect2F{eyeRect.top_left.x, -eyeRect.top_left.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
    //.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});
    auto eyeRect = worldRect.getTranslated(camera->position.getXY().getNegated()).getScaled(camera->zoom.w, camera->zoom.h);
    auto projRect = Rect2F{eyeRect.topLeft.x, -eyeRect.topLeft.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
        .getScaled(1.0F/aspect_ratio, 1.0F);
    return projRect;
}

//todo - figure out somehow
// Rect2F RenderSystemBase::getProjectionRect(const Rect3F& worldRect) const {
//     auto eyeRect = worldRect.getTranslated(camera->position.getXY().getNegated());
//     eyeRect.topLeft.x += camera->position.z;
    
    
//     .getScaled(camera->zoom.w, camera->zoom.h);
//     auto projRect = Rect2F{eyeRect.topLeft.x, -eyeRect.topLeft.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
//         .getScaled(1.0F/aspect_ratio, 1.0F);
//     return projRect;
// }

bool RenderSystemBase::isProjectedRectVisible(const Rect2F& projectionRect) const {
    if(projectionRect.right() < -1.0F || projectionRect.left() > 1.0F || projectionRect.bottom() < -1.0F || projectionRect.top() > 1.0F) {
        return false;
    }
    return true;
}




void RenderSystemBase::renderTexturedBox(const Rect2F& worldRect, TextureData* textureData, int tilesetIndex) {
    auto projRect = getProjectionRect(worldRect);
    if(!isProjectedRectVisible(projRect)) {
        return;
    }

    glColor3f(1.0F, 1.0F, 1.0F);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, textureData->getTextureID().getID());
    glEnable(GL_TEXTURE_2D);
        
    int u_idx = tilesetIndex % textureData->getColumns();
    int v_idx = tilesetIndex / textureData->getColumns();

    float u1 = static_cast<float>((u_idx + 0) * textureData->getTileWidth()) / static_cast<float>(textureData->getImageWidth());
    float u2 = static_cast<float>((u_idx + 1) * textureData->getTileWidth()) / static_cast<float>(textureData->getImageWidth());
    float v1 = static_cast<float>((v_idx + 0) * textureData->getTileHeight()) / static_cast<float>(textureData->getImageHeight());
    float v2 = static_cast<float>((v_idx + 1) * textureData->getTileHeight()) / static_cast<float>(textureData->getImageHeight());

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

    if(debugView) {
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
}

void RenderSystemBase::renderFilledBox(const Rect2F& worldRect, float r, float g, float b) {
        auto projRect = getProjectionRect(worldRect);
    if(!isProjectedRectVisible(projRect)) {
        return;
    }
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(projRect.left(), projRect.bottom());
    glVertex2f(projRect.left(), projRect.top());
    glVertex2f(projRect.right(), projRect.top());
    glVertex2f(projRect.right(), projRect.bottom());
    glEnd();
}

void RenderSystemBase::renderTranslucentFilledBox(const Rect2F& worldRect, float r, float g, float b, float fillingAlpha) {
    auto projRect = getProjectionRect(worldRect);
    if(!isProjectedRectVisible(projRect)) {
        return;
    }

    float vertices[] = {
        projRect.left(), projRect.bottom(), // Bottom-left vertex
        projRect.left(), projRect.top(), // Top-left vertex
        projRect.right(), projRect.top(), // Top-right vertex
        projRect.right(), projRect.bottom()  // Bottom-right vertex
    };

    /* Draw filling */
    glColor4f(r, g, b, fillingAlpha);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    glVertex2f(projRect.left(), projRect.bottom());
    glVertex2f(projRect.left(), projRect.top());
    glVertex2f(projRect.right(), projRect.top());
    glVertex2f(projRect.right(), projRect.bottom());
    glEnd();
    glDisable(GL_BLEND);

    /* Draw borders */
    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Define the vertex array data
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    // Draw the rectangle using GL_LINE_LOOP to form the borders
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    // Disable vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
}





