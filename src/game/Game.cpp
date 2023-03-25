#include "Game.h"
#include "Window.h"
#include "Callbacks.h"
#include "ImageLoader.h"


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
    auto& image_loader = ImageLoader::get_loader();
    if(image_loader.has_image(ImageId("tileset"))) {
        std::cout << "has image" << std::endl;
        auto texture = new TextureComponent(player, ImageId("tileset"));
        std::cout << "texture: " << texture->image_id.id << ": " << ImageLoader::get_loader().get_image(texture->image_id).texture_id << std::endl;
        player->addComponent(texture);
        auto trsf_ctrl = new TransformComponent(player, texture->rect.top_left);
        player->addComponent(trsf_ctrl);
    } else {
        std::cout << "no image" << std::endl;
        auto color = new ColorComponent(player);
        color->r = 1.0F;
        color->g = 0.0F;
        color->b = 1.0F;
        color->rect.top_left.x = 0.5F;
        player->addComponent(color);

        auto trsf_ctrl = new TransformComponent(player, color->rect.top_left);
        player->addComponent(trsf_ctrl);
    }

    // todo add some templating or whatever - it is not obvious
    // that player sam some component to be controlled
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
