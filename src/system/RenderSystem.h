#pragma once
#include <iostream>
#include <vector>

#ifndef __gl_h_
#include <glad/gl.h>
#endif


#include "SystemBase.h"
#include "../component/SpriteComponent.h"
#include "../entity/Entity.h"

class RenderSystem : public SystemBase {
public:
    int system_id;
    void init();
    void render(Entity* entity);
    // GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    // GLint mvp_location, vpos_location, vcol_location;
};

