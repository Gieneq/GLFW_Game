#include "Game.h"


void Game::init() {
    GameBase::init();
    
    createWorld();
    createPlayer();
}

bool Game::input() {
    return GameBase::input();
}

void Game::update(float dt) {
    GameBase::update(dt);
}

void Game::render() {
    GameBase::render();
}


bool Game::createWorld() {
    try
    {
        Loader::getLoader().load_map(world, "testmap");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    int size = 10;

    for(int y = -size; y < size+1; y++) {
        for(int x = -size; x < size+1; x++) {
            Entity *junk = new Entity();
            auto color_comp = new ColorComponent(junk);
            color_comp->rect.top_left.x = static_cast<float>(x);
            color_comp->rect.top_left.y = static_cast<float>(y);
            color_comp->r = 0.5F + (y % 2) / 2.0F;
            color_comp->g = 0.5F + (x % 2) / 2.0F;
            junk->addComponent(color_comp);
            entities.push_back(junk);
        }
    }

    return true;
}

bool Game::createPlayer() {

    /**
     * Create players look.
     * Load texture or replace with placeholde color.
     */
    auto playersTextureID = Loader::getLoader().get_texture_id("some_tiles");

    if(playersTextureID.hasID()) {
        std::cout << "has image" << std::endl;
        auto texture = new TextureComponent(&player, playersTextureID);
        // std::cout << "texture: " << texture->image_id.id << ": " << Loader::get_loader().get_image(texture->image_id).texture_id << std::endl;
        player.addComponent(texture);
        auto trsf_ctrl = new TransformComponent(&player, texture->rect.top_left);
        player.addComponent(trsf_ctrl);
    } else {
        std::cout << "no image" << std::endl;
        auto color = new ColorComponent(&player);
        color->r = 1.0F;
        color->g = 0.0F;
        color->b = 1.0F;
        color->rect.top_left.x = 0.5F;
        player.addComponent(color);

        auto trsf_ctrl = new TransformComponent(&player, color->rect.top_left);
        player.addComponent(trsf_ctrl);
    }

    // todo add some templating or whatever - it is not obvious
    // that player sam some component to be controlled
    auto wsad_ctrl = new WSADControllableComponent(&player);
    player.addComponent(wsad_ctrl);

    entities.push_back(&player);

    camera.focus_on(&player);
    
    user_input_system.attach_controllabe(&player);

    return true;
}
