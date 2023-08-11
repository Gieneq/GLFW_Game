#pragma once
#include <iostream>

struct Vect2F {
    Vect2F() = default;
    Vect2F(float x, float y) : x{x}, y{y} {}
    Vect2F get_negated() const;
    Vect2F getTranslated(const Vect2F& translation) const;
    float x{0.0F};
    float y{0.0F};

    friend std::ostream& operator<<(std::ostream& os, const Vect2F& vect) {
        os << "[" << vect.x << ", " << vect.y << "]";
        return os;
    }
};

struct Vect3F {
    Vect3F() = default;
    Vect3F(float x, float y, float z) : x{x}, y{y}, z{z} {}
    Vect3F(const Vect2F& xy, float z) : x{xy.x}, y{xy.y}, z{z} {}
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};

    friend std::ostream& operator<<(std::ostream& os, const Vect3F& vect) {
        os << "[" << vect.x << ", " << vect.y << ", " << vect.z << "]";
        return os;
    }
};

struct Size2F {
    Size2F() = default;
    Size2F(float w, float h) : w{w}, h{h} {}
    float w{0.0F};
    float h{0.0F};

    friend std::ostream& operator<<(std::ostream& os, const Size2F& size) {
        os << "[" << size.w << ", " << size.h << "]";
        return os;
    }
};

struct Size3F {
    Size3F() = default;
    Size3F(float w, float h, float l) : w{w}, h{h}, l{l} {}
    float w{0.0F};
    float h{0.0F};
    float l{0.0F};

    friend std::ostream& operator<<(std::ostream& os, const Size3F& size) {
        os << "[" << size.w << ", " << size.h << ", " << size.l << "]";
        return os;
    }
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

    void alignToLeftOf(const Rect2F& other) {
        top_left.x = other.left() - size.w;
    }

    void alignToRightOf(const Rect2F& other) {
        top_left.x = other.right();
    }

    void alignToTopOf(const Rect2F& other) {
        top_left.y = other.top() - size.h;
    }

    void alignToBottomOf(const Rect2F& other) {
        top_left.y = other.bottom();
    }

    bool checkIntersection(const Rect2F& other) const {
        // return !(left() > other.right() || right() < other.left() || top() > other.bottom() || bottom() < other.top());
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top());
    }

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