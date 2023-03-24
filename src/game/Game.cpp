#include "Game.h"
#include "../core/Window.h"
#include "../core/Callbacks.h"


void Game::init() {
    Core::init();
    render_system.init();
    render_system.attach_camera(&camera);

    user_input_system.init();
    Window::add_keyboard_listener(&user_input_system);

    transform_system.init();

    int size = 100;

    for(int y = -size; y < size+1; y++) {
        for(int x = -size; x < size+1; x++) {
            Entity *junk = new Entity();
            auto color_comp = new ColorComponent(junk);
            color_comp->rect.top_left.x = x;
            color_comp->rect.top_left.y = y;
            color_comp->r = 0.5F + (y % 2) / 2.0F;
            color_comp->g = 0.5F + (x % 2) / 2.0F;
            junk->addComponent(color_comp);
            entities.push_back(junk);
        }
    }

    
    Entity *player = new Entity();
    auto color = new ColorComponent(player);
    auto trsf_ctrl = new TransformComponent(player, color->rect.top_left);
    color->r = 1.0F;
    color->b = 0.0F;
    color->rect.top_left.x = 0.5F;
    player->addComponent(color);
    player->addComponent(trsf_ctrl);

    auto wsad_ctrl = new WSADControllableComponent(player);
    player->addComponent(wsad_ctrl);

    entities.push_back(player);

    camera.focus_on(player);
    
    user_input_system.attach_controllabe(player);
}


bool Game::input() {
    // user_input_system.process_keyboard();
}

void Game::update(float dt) {
    for(auto entity: entities) {
        transform_system.update(entity, dt);
    }
    
    camera.update(dt);
}

void Game::render() {
    for(auto entity: entities) {
        render_system.render(entity);
    }
}
