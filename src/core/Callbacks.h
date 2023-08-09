#pragma once

class KeyboardEventListener {
public:
    virtual ~KeyboardEventListener() = default;
    
    virtual bool onKeyPressed(int key) = 0;
    virtual bool onKeyReleased(int key) = 0;
};