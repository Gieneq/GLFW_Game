#pragma once
#include "Component.h"
#include <stdexcept>
#include "Timers.h"
#include <vector>

class Entity;
class TextureComponent;
class AnimationComponent : public Component, public TimerListener {
public:
    AnimationComponent(Entity* e, TextureComponent* textureCmp, int interval);
    virtual ~AnimationComponent() = default;
    virtual void onTimerTick(const int recentTick) override;

    void appendIndex(int index) {
        indices.push_back(index);
    }

    void setActive(bool activeValue);

    TextureComponent * textureCmp{nullptr};

    AnimationComponent* clone(Entity* new_parent) override {
        auto animCmp = new AnimationComponent(new_parent, textureCmp, interval);
        animCmp->indices = indices;
        return animCmp;
    }

private:    
    bool active{false};
    int interval{1000};
    std::vector<int> indices;
};