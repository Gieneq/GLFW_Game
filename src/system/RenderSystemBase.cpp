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

Rect4F RenderSystemBase::getProjectionRect(const Rect4F& worldRect) const {
    // get_translated(camera->position.getXY().get_negated()).get_scaled(camera->zoom);
    // auto projRect = Rect4F{eyeRect.top_left.x, -eyeRect.top_left.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
    //.get_scaled(Size2F{1.0F/aspect_ratio, 1.0F});
    auto eyeRect = worldRect.getTranslated(camera->position.getXY().getNegated()).getScaled(camera->zoom.w, camera->zoom.h);
    auto projRect = Rect4F{eyeRect.topLeft.x, -eyeRect.topLeft.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
        .getScaled(1.0F/aspect_ratio, 1.0F);
    return projRect;
}

//todo - figure out somehow
// Rect4F RenderSystemBase::getProjectionRect(const Rect3F& worldRect) const {
//     auto eyeRect = worldRect.getTranslated(camera->position.getXY().getNegated());
//     eyeRect.topLeft.x += camera->position.z;
    
    
//     .getScaled(camera->zoom.w, camera->zoom.h);
//     auto projRect = Rect4F{eyeRect.topLeft.x, -eyeRect.topLeft.y - eyeRect.size.h, eyeRect.size.w, eyeRect.size.h}
//         .getScaled(1.0F/aspect_ratio, 1.0F);
//     return projRect;
// }

bool RenderSystemBase::isProjectedRectVisible(const Rect4F& projectionRect) const {
    if(projectionRect.right() < -1.0F || projectionRect.left() > 1.0F || projectionRect.bottom() < -1.0F || projectionRect.top() > 1.0F) {
        return false;
    }
    return true;
}




void RenderSystemBase::renderTexturedRect4F(const Rect4F& worldRect, TextureData* textureData, int tilesetIndex) {
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

    /* Remove bleeding effect */
    u1 += 0.5F / static_cast<float>(textureData->getImageWidth());
    u2 -= 0.5F / static_cast<float>(textureData->getImageWidth());
    v1 += 0.5F / static_cast<float>(textureData->getImageHeight());
    v2 -= 0.5F / static_cast<float>(textureData->getImageHeight());

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

void RenderSystemBase::renderFilledRect4F(const Rect4F& worldRect, float r, float g, float b) {
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

void RenderSystemBase::renderTranslucentFilledRect4F(const Rect4F& worldRect, float r, float g, float b, float fillingAlpha) {
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

void RenderSystemBase::renderFilledCuboid5F(const Cuboid5F& worldCuboid, float r, float g, float b) {
    renderFilledCuboid6F(Cuboid6F(worldCuboid), r, g, b);
}

void RenderSystemBase::renderFilledCuboid6F(const Cuboid6F& worldCuboid, float r, float g, float b) {
    /* Form 2 sides: 
    front - flatten cube shifted in Y by depth and z,
    bottom - width remains the same, height is depth, align bottom to bottom, shift by z
    */

    /* Front */
    const auto frontRect = Rect4F{
        worldCuboid.x(),
        worldCuboid.y() - worldCuboid.depth() - worldCuboid.z(),
        worldCuboid.width(),
        worldCuboid.height()
    };
    const auto frontBrightness = Settings::Rendering::Brightness::FRONT;
    renderFilledRect4F(frontRect, 
        r * frontBrightness, 
        g * frontBrightness, 
        b * frontBrightness
    );

    /* Bottom */
    const auto bottomRect = Rect4F{
        worldCuboid.x(),
        worldCuboid.bottom() - worldCuboid.depth() - worldCuboid.z(),
        worldCuboid.width(),
        worldCuboid.depth()
    };
    const auto bottomBrightness = Settings::Rendering::Brightness::BOTTOM;
    renderFilledRect4F(bottomRect, 
        r * bottomBrightness, 
        g * bottomBrightness, 
        b * bottomBrightness
    );
}

void RenderSystemBase::renderTranslucentFilledCuboid5F(const Cuboid5F& worldCuboid, float r, float g, float b, float fillingAlpha) {
    renderTranslucentFilledCuboid6F(Cuboid6F(worldCuboid), r, g, b, fillingAlpha);
}

void RenderSystemBase::renderTranslucentFilledCuboid6F(const Cuboid6F& worldCuboid, float r, float g, float b, float fillingAlpha) {
    /* Form 4 sides: 
    back - flatten cube shifted in Y by z,
    top - width remains the same, height is depth, align top to top, shift by z,
    front - flatten cube shifted in Y by depth and z,
    bottom - width remains the same, height is depth, align bottom to bottom, shift by z
    */

    if(worldCuboid.depth() > 0.0F) {
        /* Back */
        const auto backRect = Rect4F{
            worldCuboid.x(),
            worldCuboid.y() - worldCuboid.z(),
            worldCuboid.width(),
            worldCuboid.height()
        };
        const auto backBrightness = Settings::Rendering::Brightness::BACK;
        renderTranslucentFilledRect4F(backRect, 
            r * backBrightness, 
            g * backBrightness, 
            b * backBrightness,
            fillingAlpha
        );

        /* Top */
        const auto topRect = Rect4F{
            worldCuboid.x(),
            worldCuboid.y() - worldCuboid.depth() - worldCuboid.z(),
            worldCuboid.width(),
            worldCuboid.depth()
        };
        const auto topBrightness = Settings::Rendering::Brightness::TOP;
        renderTranslucentFilledRect4F(topRect, 
            r * topBrightness, 
            g * topBrightness, 
            b * topBrightness,
            fillingAlpha
        );
    }

    /* Front */
    const auto frontRect = Rect4F{
        worldCuboid.x(),
        worldCuboid.y() - worldCuboid.depth() - worldCuboid.z(),
        worldCuboid.width(),
        worldCuboid.height()
    };
    const auto frontBrightness = Settings::Rendering::Brightness::FRONT;
    renderTranslucentFilledRect4F(frontRect, 
        r * frontBrightness, 
        g * frontBrightness, 
        b * frontBrightness,
        fillingAlpha
    );

    /* Bottom */
    const auto bottomRect = Rect4F{
        worldCuboid.x(),
        worldCuboid.bottom() - worldCuboid.depth() - worldCuboid.z(),
        worldCuboid.width(),
        worldCuboid.depth()
    };
    const auto bottomBrightness = Settings::Rendering::Brightness::BOTTOM;
    renderTranslucentFilledRect4F(bottomRect, 
        r * bottomBrightness, 
        g * bottomBrightness, 
        b * bottomBrightness,
        fillingAlpha
    );
}





