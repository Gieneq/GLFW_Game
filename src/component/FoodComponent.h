#pragma once
#include <iostream>
#include "Component.h"

class FoodComponent : public Component {
public:
    virtual ~FoodComponent() = default;

    void eat();
};
