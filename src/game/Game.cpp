#include "Game.h"


void Game::init() {
    Core::init();
    render_system.init();
    render_system.attach_camera(&camera);

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
    color->r = 1.0F;
    color->b = 0.0F;
    color->rect.top_left.x = 0.5F;
    player->addComponent(color);
    entities.push_back(player);
}


bool Game::input() {

}

void Game::update(float dt) {
    camera.position.x += dt * 0.5F;
}

void Game::render() {
    for(auto entity: entities) {
        render_system.render(entity);
    }
}
