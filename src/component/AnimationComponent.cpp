#include "AnimationComponent.h"
#include "GraphicsComponent.h"

AnimationComponent::AnimationComponent(Entity* e, TextureComponent* textureCmp, int interval) : Component(e), textureCmp(textureCmp), interval(interval) {
    this->active = false;
}

void AnimationComponent::onTimerTick(const int recentTick) {
    if(textureCmp) {
        // std::cout << "AnimationComponent::onTimerTick " << recentTick << "for" << textureCmp->textureID.value << std::endl;
        textureCmp->tilesetIndex = indices[recentTick];
    }
}

void AnimationComponent::setActive(bool activeValue) {
    if(this->active == false && activeValue == true) {
        /* Activating */
        int indicesCount = static_cast<int>(indices.size());
        TimersRegister::getTimersRegister().getTimer(interval, 0, indicesCount).addListener(this);
    }
    else if(this->active == true && activeValue == false) {
        /* Disactivating */
        int indicesCount = static_cast<int>(indices.size());
        TimersRegister::getTimersRegister().getTimer(interval, 0, indicesCount).removeListener(this);
    }
    this->active = activeValue;
}