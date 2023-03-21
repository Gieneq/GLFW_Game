#pragma once
#include <iostream>
#include "Component.h"

class TransformComponent : public Component {
public:
    virtual ~TransformComponent() = default;

    void move();
};
