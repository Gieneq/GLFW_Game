#pragma once

class KeyboardEvent {
public:
    virtual ~KeyboardEvent() = default;
    
    virtual bool on_key_press(int key) = 0;
    virtual bool on_key_release(int key) = 0;
};