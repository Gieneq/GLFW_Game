#pragma once
#include <iostream>

struct Vect2F {
    Vect2F(float x, float y) : x{x}, y{y} {}
    Vect2F() : x{0.0F}, y{0.0F} {}
    Vect2F get_negated() const;
    float x{0.0F};
    float y{0.0F};
};

struct Size2F {
    Size2F(float w, float h) : w{w}, h{h} {}
    Size2F() : w{0.0F}, h{0.0F} {}
    float w{0.0F};
    float h{0.0F};
};

struct Rect2F {
    Rect2F() : top_left{}, size{} {}
    Rect2F(float x, float y, float w, float h) : top_left{x,y}, size{w,h} {}

    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    Rect2F get_translated(Vect2F translation) const;
    Rect2F get_scaled(Size2F scale) const;
    static Rect2F from_sides(float left, float right, float top, float bottom);

    Vect2F top_left;
    Size2F size;

    friend std::ostream& operator<<(std::ostream& os, const Rect2F& rect) {
        os << "[" << rect.top_left.x << ", " << rect.top_left.y << ", " << rect.size.w << ", " << rect.size.h << "]";
        return os;
    }
};

template<typename T>
T constrain(T value, T min, T max) {
    value = value < min ? min : value;
    value = value > max ? max : value;
    return value;
}