#pragma once
#include "Component.h"
#include <stdexcept>
#include <unordered_map>

enum class MovementControlKey {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Entity;
class ControllableComponent : public Component {
public:
    ControllableComponent(Entity* e, MovementComponent* move);
    virtual ~ControllableComponent() = default;

    std::unordered_map<int, bool> pressedKeysWSAD;
    MovementControlKey lastPressedKey{MovementControlKey::NONE};

    void onPressWSADKey(int key);
    void onReleaseWSADKey(int key);
    
    void onPressSpaceKey();
    void onReleaseSpaceKey();
    void onPressLShiftKey();
    void onReleaseLShiftKey();

    bool active{true};

    MovementComponent * controlledMovement{nullptr};

protected:
    void updateMovement();
};
