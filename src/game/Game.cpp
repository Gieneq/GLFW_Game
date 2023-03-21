#include "Game.h"


void Game::init() {
    Core::init();
    Entity *player = new Entity();
    player->addComponent(new ColorComponent(player));
    entities.push_back(player);
}


bool Game::input() {

}

void Game::update(float dt) {

}

void Game::render() {
    for(auto entity: entities) {
        render_system.render(entity);
    }
}
