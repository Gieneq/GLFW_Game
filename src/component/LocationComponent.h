#pragma once
#include "Component.h"
#include "Maths.h"

class Floor;
class LocationComponent : public Component {
public:
    LocationComponent(Entity* e) : Component(e) {}
    virtual ~LocationComponent() = default;
    Rect2F worldRect{0.0F,0.0F,1.0F,1.0F};
    float relativeZ{0.0F}; // relative to floor
    float zLength{1.0F};
    Floor *containingFloor{nullptr};

    float getWorldZ() const;

    float getWorldBottomZ() const {
        return getWorldZ();
    }

    float getWorldTopZ() const {
        return getWorldZ() + zLength;
    }

    int getRecentElevation();

    bool changeElevation(int newElevation);

    LocationComponent* clone(Entity* new_parent) override {
        return new LocationComponent(new_parent);
    }
};
