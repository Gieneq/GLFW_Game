#pragma once
#include <iostream>


struct Vect2F {
    Vect2F() = default;

    Vect2F(float x, float y) : x{x}, y{y} {}

    Vect2F getNegated() const;

    Vect2F getTranslated(const Vect2F& translation) const;

    friend std::ostream& operator<<(std::ostream& os, const Vect2F& vect) {
        os << "[" << vect.x << ", " << vect.y << "]";
        return os;
    }
    
    float x{0.0F};

    float y{0.0F};
};

struct Vect3F {
    Vect3F() = default;

    Vect3F(float x, float y, float z) : x{x}, y{y}, z{z} {}

    Vect3F(const Vect2F& xy, float z) : x{xy.x}, y{xy.y}, z{z} {}

    Vect3F getNegated() const;

    Vect3F getTranslated(const Vect3F& translation) const;

    void normalize();

    Vect2F getXY() const;

    friend std::ostream& operator<<(std::ostream& os, const Vect3F& vect) {
        os << "[" << vect.x << ", " << vect.y << ", " << vect.z << "]";
        return os;
    }

    float x{0.0F};

    float y{0.0F};

    float z{0.0F};
};

struct Size2F {
    Size2F() = default;

    Size2F(float w, float h) : w{w}, h{h} {}

    Size2F getExpanded(const float x, const float y) const;

    inline Vect2F toVect2F() const {
        return Vect2F{w, h};
    }

    friend std::ostream& operator<<(std::ostream& os, const Size2F& size) {
        os << "[" << size.w << ", " << size.h << "]";
        return os;
    }

    float w{0.0F};

    float h{0.0F};
};

struct Size3F {
    Size3F() = default;

    Size3F(float w, float h, float d) : w{w}, h{h}, d{d} {}

    Size3F(Size2F wh, float d) : w{wh.w}, h{wh.h}, d{d} {}

    inline Vect3F toVect3F() const {
        return Vect3F{w, h, d};
    }

    friend std::ostream& operator<<(std::ostream& os, const Size3F& size) {
        os << "[" << size.w << ", " << size.h << ", " << size.d << "]";
        return os;
    }

    float w{0.0F};

    float h{0.0F};

    float d{0.0F};
};

struct Rect4F {
    Rect4F() : topLeft{}, size{} {}

    Rect4F(Vect2F tl, Size2F s) : topLeft{tl}, size{s} {}

    Rect4F(float x, float y, float w, float h) : topLeft{x, y}, size{w, h} {}

    static Rect4F fromSides(float left, float right, float top, float bottom);

    inline float left() const {
        return topLeft.x;
    }

    inline float right() const {
        return topLeft.x + size.w;
    }

    inline float top() const {
        return topLeft.y;
    }

    inline float bottom() const {
        return topLeft.y + size.h;
    }

    Rect4F getTranslated(Vect2F translation) const;

    Rect4F getScaled(const float xScale, const float yScale) const;

    inline float& x() { return topLeft.x; }
    inline float& y() { return topLeft.y; }
    inline float& w() { return size.w; }
    inline float& h() { return size.h; }

    inline void alignToLeftOf(const Rect4F& other) {
        topLeft.x = other.left() - size.w;
    }

    inline void alignToRightOf(const Rect4F& other) {
        topLeft.x = other.right();
    }

    inline void alignToTopOf(const Rect4F& other) {
        topLeft.y = other.top() - size.h;
    }

    inline void alignToBottomOf(const Rect4F& other) {
        topLeft.y = other.bottom();
    }

    inline bool checkIntersection(const Rect4F& other) const {
        /* Keep in mind direction of X and Y */
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top());
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect4F& rect) {
        os << "[" << rect.topLeft.x << ", " << rect.topLeft.y << ", " << rect.size.w << ", " << rect.size.h << "]";
        return os;
    }

    Vect2F topLeft;

    Size2F size;
};

/**
 * 2D rectangle with depth
*/
class Rect6F;
class Rect5F {
public:
    Rect5F() : topLeft{}, size{} {}
    Rect5F(float x, float y, float w, float h, float d) : topLeft{x, y}, size{w, h, d} {}

    inline void alignToLeftOf(const Rect5F& other) {
        topLeft.x = other.left() - size.w;
    }

    inline void alignToRightOf(const Rect5F& other) {
        topLeft.x = other.right();
    }

    inline void alignToTopOf(const Rect5F& other) {
        topLeft.y = other.top() - size.h;
    }

    inline void alignToBottomOf(const Rect5F& other) {
        topLeft.y = other.bottom();
    }

    inline float left() const {
        return topLeft.x;
    }

    inline float right() const {
        return topLeft.x + size.w;
    }

    inline float top() const {
        return topLeft.y;
    }

    inline float bottom() const {
        return topLeft.y + size.h;
    }

    /* Front is in distance from topLeft. In this 
    * case topLeft.z is 0 so front is equal to depth */
    inline float front() const {
        return size.d;
    }

    static Rect5F fromSides(float left, float right, float top, float bottom, float front);

    Rect5F getTranslated(Vect2F translation) const;

    inline float& x() { return topLeft.x; }

    inline float& y() { return topLeft.y; }

    inline float& w() { return size.w; }

    inline float& h() { return size.h; }

    inline float& d() { return size.d; }

    inline Rect4F getFlatten() const {
        return Rect4F{topLeft.x, topLeft.y, size.w, size.h};
    }

    inline Size2F getWH() const {
        return Size2F{size.w, size.h};
    }

    inline bool checkIntersection(const Rect5F& other) const {
        /* Keep in mind direction of X and Y. In this case depth won't change anything */
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top());
    }
        
    bool checkIntersection(const Rect6F* other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Rect5F& rect) {
        os << "[" << rect.topLeft.x << ", " << rect.topLeft.y << ", " << rect.size.w << ", " << rect.size.h << ", " << rect.size.d << "]";
        return os;
    }

    Vect2F topLeft;

    Size3F size;
};

class Rect6F {
public:
    Rect6F() : topLeft{}, size{} {}
    Rect6F(float x, float y, float z, float w, float h, float d) : topLeft{x, y, z}, size{w, h, d} {}

    inline float left() const {
        return topLeft.x;
    }

    inline float right() const {
        return topLeft.x + size.w;
    }

    inline float top() const {
        return topLeft.y;
    }

    inline float bottom() const {
        return topLeft.y + size.h;
    }

    /** Front is in distance from topLeft */
    inline float front() const {
        return topLeft.z + size.d;;
    }

    /** Back is where there topLeft is */
    inline float back() const {
        return topLeft.z;
    }
    
    Rect6F getTranslated(Vect3F translation) const;

    static Rect6F fromSides(float left, float right, float top, float bottom, float back, float front);

    inline float& x() { return topLeft.x; }

    inline float& y() { return topLeft.y; }

    inline float& z() { return topLeft.z; }

    inline float& w() { return size.w; }

    inline float& h() { return size.h; }

    inline float& d() { return size.d; }

    inline Rect4F getFlatten() const {
        return Rect4F{topLeft.x, topLeft.y, size.w, size.h};
    }

    inline Rect5F getFlattenWithDepth() const {
        return Rect5F{topLeft.x, topLeft.y, size.w, size.h, size.d};
    }

    inline Vect2F getXY() const {
        return Vect2F{topLeft.x, topLeft.y};
    }

    inline Size2F getWH() const {
        return Size2F{size.w, size.h};
    }

    inline void alignToLeftOf(const Rect6F& other) {
        topLeft.x = other.left() - size.w;
    }

    inline void alignToRightOf(const Rect6F& other) {
        topLeft.x = other.right();
    }

    inline void alignToTopOf(const Rect6F& other) {
        topLeft.y = other.top() - size.h;
    }

    inline void alignToBottomOf(const Rect6F& other) {
        topLeft.y = other.bottom();
    }

    inline void alignToFrontOf(const Rect6F& other) {
        topLeft.z = other.front();
    }

    inline void alignToBackOf(const Rect6F& other) {
        topLeft.z = other.back() - size.d;
    }

    inline bool checkIntersection(const Rect6F& other) const {
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top() || front() <= other.back() || back() >= other.front());
    }

    inline bool checkIntersection(const Rect5F& other) const {
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top() || back() >= other.front());
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect6F& rect) {
        os << "[" << rect.topLeft.x << ", " << rect.topLeft.y << ", " << rect.topLeft.z << ", " << rect.size.w << ", " << rect.size.h << ", " << rect.size.d << "]";
        return os;
    }

    Vect3F topLeft;

    Size3F size;
};

template<typename T>
T constrain(T value, T min, T max) {
    value = value < min ? min : value;
    value = value > max ? max : value;
    return value;
}
