#pragma once

class KeyboardEvent {
public:
    virtual ~KeyboardEvent() = default;
    
    virtual bool onKeyPressed(int key) = 0;
    virtual bool onKeyReleased(int key) = 0;
};