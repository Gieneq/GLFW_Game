#pragma once
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
#include "Entity.h"

class WSADControllableComponent : public Component {
public:
    WSADControllableComponent(Entity* e) : Component(e) {
        transform_cmpnt = e->getComponent<TransformComponent>();
    }
    virtual ~WSADControllableComponent() = default;

    void press_w();
    void press_a();
    void press_s();
    void press_d();
    void release_w();
    void release_a();
    void release_s();
    void release_d();

    TransformComponent * transform_cmpnt;

    WSADControllableComponent* clone(Entity* new_parent) override {
        return new WSADControllableComponent(new_parent);
    }
};
