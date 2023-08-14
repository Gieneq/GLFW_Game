#pragma once
#include <iostream>

struct Vect2F {
    Vect2F() = default;
    Vect2F(float x, float y) : x{x}, y{y} {}

    Vect2F getNegated() const;
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

    Vect3F getNegated() const;
    Vect3F getTranslated(const Vect3F& translation) const;
    void normalize();

    float x{0.0F};
    float y{0.0F};
    float z{0.0F};

    Vect2F getXY() const;

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
    Size3F(float w, float h, float d) : w{w}, h{h}, d{d} {}
    Size3F(Size2F wh, float d) : w{wh.w}, h{wh.h}, d{d} {}
    float w{0.0F};
    float h{0.0F};
    float d{0.0F};

    friend std::ostream& operator<<(std::ostream& os, const Size3F& size) {
        os << "[" << size.w << ", " << size.h << ", " << size.d << "]";
        return os;
    }
};

struct Rect2F {
    Rect2F() : topLeft{}, size{} {}
    Rect2F(Vect2F tl, Size2F s) : topLeft{tl}, size{s} {}
    Rect2F(float x, float y, float w, float h) : topLeft{x, y}, size{w, h} {}
    static Rect2F fromSides(float left, float right, float top, float bottom);

    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    Rect2F getTranslated(Vect2F translation) const;
    Rect2F getScaled(const float xScale, const float yScale) const;

    Vect2F topLeft;
    Size2F size;

    inline float& x() { return topLeft.x; }
    inline float& y() { return topLeft.y; }
    inline float& w() { return size.w; }
    inline float& h() { return size.h; }

    void alignToLeftOf(const Rect2F& other) {
        topLeft.x = other.left() - size.w;
    }

    void alignToRightOf(const Rect2F& other) {
        topLeft.x = other.right();
    }

    void alignToTopOf(const Rect2F& other) {
        topLeft.y = other.top() - size.h;
    }

    void alignToBottomOf(const Rect2F& other) {
        topLeft.y = other.bottom();
    }

    bool checkIntersection(const Rect2F& other) const {
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top());
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect2F& rect) {
        os << "[" << rect.topLeft.x << ", " << rect.topLeft.y << ", " << rect.size.w << ", " << rect.size.h << "]";
        return os;
    }
};

class Rect3F {
public:
    Rect3F() : topLeft{}, size{} {}
    Rect3F(float x, float y, float z, float w, float h, float d) : topLeft{x, y, z}, size{w, h, d} {}

    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    float front() const;
    float back() const;
    Rect3F getTranslated(Vect3F translation) const;
    static Rect3F fromSides(float left, float right, float top, float bottom, float front, float back);

    Vect3F topLeft;
    Size3F size;

    inline float& x() { return topLeft.x; }
    inline float& y() { return topLeft.y; }
    inline float& z() { return topLeft.z; }
    inline float& w() { return size.w; }
    inline float& h() { return size.h; }
    inline float& d() { return size.d; }

    inline Rect2F getFlatten() const {
        return Rect2F{topLeft.x, topLeft.y, size.w, size.h};
    }

    inline Vect2F getXY() const {
        return Vect2F{topLeft.x, topLeft.y};
    }

    inline Size2F getWH() const {
        return Size2F{size.w, size.h};
    }

    void alignToLeftOf(const Rect3F& other) {
        topLeft.x = other.left() - size.w;
    }

    void alignToRightOf(const Rect3F& other) {
        topLeft.x = other.right();
    }

    void alignToTopOf(const Rect3F& other) {
        topLeft.y = other.top() - size.h;
    }

    void alignToBottomOf(const Rect3F& other) {
        topLeft.y = other.bottom();
    }

    void alignToFrontOf(const Rect3F& other) {
        topLeft.z = other.front() - size.d;
    }

    void alignToBackOf(const Rect3F& other) {
        topLeft.z = other.back();
    }

    bool checkIntersection(const Rect3F& other) const {
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top() || front() >= other.back() || back() <= other.front());
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect3F& rect) {
        os << "[" << rect.topLeft.x << ", " << rect.topLeft.y << ", " << rect.topLeft.z << ", " << rect.size.w << ", " << rect.size.h << ", " << rect.size.d << "]";
        return os;
    }
};

template<typename T>
T constrain(T value, T min, T max) {
    value = value < min ? min : value;
    value = value > max ? max : value;
    return value;
}