#pragma once
#include <iostream>
#include <array>

/********************************************
 *     Naming convention for Cuboid6F       *
 *                                          *
 * --------------------------------> x      *
 *  |              TOP                      *
 *  |     --------------------              *
 *  |    |  FRONT             |             *
 *  |    |      |-            |             *
 *  | L  |       \            | R           *
 *  |    |        \           |             *
 *  |    |         \          |             *
 *  |    |            BACK    |             *
 *  |     --------------------              *
 *  |           BOTTOM                      *
 * \ /                                      *
 *  y                             (z) UP    *
 *                                          *
 ********************************************
 *                                          *
 *   ORIGIN: top, left, bottom              *
 *                                          *
 ********************************************
 *         Intersection conditions          *
 *                                          *
 * inclusive [left, right) exclusive        *
 * inclusive [top, bottom) exclusive        *
 * inclusive [back, front) exclusive        *
 *                                          *
 *******************************************/

class Vect2F {
public:
    Vect2F() = default;

    Vect2F(float x, float y) : x{x}, y{y} {}

    void negate();

    Vect2F getNegated() const;

    void translate(const Vect2F& translation);

    void translate(const float x, const float y);

    Vect2F getTranslated(const Vect2F& translation) const;

    void normalize();

    Vect2F getNormalized() const;

    void scale(const float xScale, const float yScale);

    Vect2F getScaled(const float xScale, const float yScale) const;

    inline float length() const {
        return sqrt(x*x + y*y);
    }

    inline float lengthSquared() const {
        return x*x + y*y;
    }

    inline float dot(const Vect2F& other) const {
        return x*other.x + y*other.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vect2F& vect) {
        os << "[" << vect.x << ", " << vect.y << "]";
        return os;
    }
    
    float x{0.0F};

    float y{0.0F};
};

class Vect3F {
public:
    Vect3F() = default;

    Vect3F(float x, float y, float z) : x{x}, y{y}, z{z} {}

    Vect3F(const Vect2F& xy, float z) : x{xy.x}, y{xy.y}, z{z} {}

    void negate();

    Vect3F getNegated() const;

    void translate(const Vect3F& translation);

    void translate(const float x, const float y, const float z);

    Vect3F getTranslated(const Vect3F& translation) const;

    Vect3F getTranslated(const float x, const float y, const float z) const;

    void normalize();

    Vect3F getNormalized() const;

    void scale(const float xScale, const float yScale, const float zScale);

    Vect3F getScaled(const float xScale, const float yScale, const float zScale) const;

    inline float length() const {
        return sqrt(x*x + y*y + z*z);
    }

    inline float lengthSquared() const {
        return x*x + y*y + z*z;
    }

    inline float dot(const Vect3F& other) const {
        return x*other.x + y*other.y + z*other.z;
    }

    inline Vect3F cross(const Vect3F& other) const {
        return Vect3F{y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x};
    }

    Vect2F getXY() const;

    friend std::ostream& operator<<(std::ostream& os, const Vect3F& vect) {
        os << "[" << vect.x << ", " << vect.y << ", " << vect.z << "]";
        return os;
    }

    float x{0.0F};

    float y{0.0F};

    float z{0.0F};
};

class Size2F {
public:
    Size2F() = default;

    Size2F(float w, float h) : w{w}, h{h} {}

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

class Size3F {
public:
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

class Rect4F {
public:
    Rect4F() : topLeftCorner{}, size{} {}

    Rect4F(Vect2F tl, Size2F s) : topLeftCorner{tl}, size{s} {}

    Rect4F(float x, float y, float w, float h) : topLeftCorner{x, y}, size{w, h} {}

    static Rect4F fromSides(float left, float right, float top, float bottom);

    inline float left() const {
        return topLeftCorner.x;
    }

    inline float right() const {
        return topLeftCorner.x + size.w;
    }

    inline float top() const {
        return topLeftCorner.y;
    }

    inline float bottom() const {
        return topLeftCorner.y + size.h;
    }

    std::array<Vect2F, 2> getPointsInDirection(const Vect2F& direction) const;

    Rect4F getTranslated(Vect2F translation) const;

    Rect4F getScaled(const float xScale, const float yScale) const;

    inline float& x() { return topLeftCorner.x; }

    inline float& y() { return topLeftCorner.y; }

    inline float& w() { return size.w; }

    inline float& h() { return size.h; }
    
    inline const float& x() const { return topLeftCorner.x; }

    inline const float& y() const { return topLeftCorner.y; }

    inline const float& w() const { return size.w; }

    inline const float& h() const { return size.h; }

    inline float width() const { return size.w; }

    inline float height() const { return size.h; }

    inline bool hasPointInside(const Vect2F& point) const {
        return point.x >= left() && point.x < right() && point.y >= top() && point.y < bottom();
    }

    inline bool checkIntersection(const Rect4F& other) const {
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top());
    }

    static bool checkIntersection(const Rect4F& rect1, const Rect4F& rect2) {
        return rect1.checkIntersection(rect2);
    }

    inline Vect2F topLeft() const {
        return Vect2F{right(), top()};
    }

    inline Vect2F topRight() const {
        return Vect2F{right(), top()};
    }

    inline Vect2F bottomLeft() const {
        return Vect2F{left(), bottom()};
    }

    inline Vect2F bottomRight() const {
        return Vect2F{right(), bottom()};
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect4F& rect) {
        os << "[" << rect.topLeftCorner.x << ", " << rect.topLeftCorner.y << ", " << rect.size.w << ", " << rect.size.h << "]";
        return os;
    }

private:
    Vect2F topLeftCorner;

    Size2F size;
};


class Cuboid6F {
public:
    Cuboid6F() : topLeft{}, size{} {}
    Cuboid6F(float x, float y, float z, float w, float h, float d) : 
        topLeft{x, y, z}, size{w, h, d} {}

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
    
    static Cuboid6F fromSides(float left, float right, float top, float bottom, float back, float front);

    inline float& x() { return topLeft.x; }

    inline float x() const { return topLeft.x; }

    inline float& y() { return topLeft.y; }

    inline float y() const { return topLeft.y; }

    inline float& z() { return topLeft.z; }

    inline float z() const { return topLeft.z; }

    inline float& w() { return size.w; }

    inline float& h() { return size.h; }

    inline float& d() { return size.d; }

    inline float width() const {
        return size.w;
    }

    inline float height() const {
        return size.h;
    }

    inline float depth() const {
        return size.d;
    }

    inline Rect4F getFlatten() const {
        return Rect4F{topLeft.x, topLeft.y, size.w, size.h};
    }

    inline Cuboid6F getTranslated(const Vect3F& translation) const {
        return Cuboid6F{topLeft.x + translation.x, topLeft.y + translation.y, topLeft.z + translation.z, size.w, size.h, size.d};
    }

    inline Vect2F getXY() const {
        return Vect2F{topLeft.x, topLeft.y};
    }

    inline Size2F getWH() const {
        return Size2F{size.w, size.h};
    }

    inline bool checkIntersection(const Cuboid6F& other) const {
        /* Be careful - z used to be treated differently. Now it is all the same. */
        return !(left() >= other.right() || right() <= other.left() || top() >= other.bottom() || bottom() <= other.top() 
            || back() >= other.front() || front() <= other.back());
    }

    static bool checkIntersection(const Cuboid6F& rect1, const Cuboid6F& rect2) {
        return rect1.checkIntersection(rect2);
    }

    friend std::ostream& operator<<(std::ostream& os, const Cuboid6F& rect) {
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
