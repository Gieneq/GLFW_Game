#include "Maths.h"


/* Vect2F */

Vect2F Vect2F::getNegated() const {
    return Vect2F{-x, -y};
}


Vect2F Vect2F::getTranslated(const Vect2F& translation) const {
    return Vect2F{x + translation.x, y + translation.y};
}


Vect2F Vect3F::getXY() const {
    return Vect2F{x, y};
}

/* Vect3F */

Vect3F Vect3F::getNegated() const {
    return Vect3F{-x, -y, -z};
}

Vect3F Vect3F::getTranslated(const Vect3F& translation) const {
    return Vect3F{x + translation.x, y + translation.y, z + translation.z};
}

void Vect3F::normalize() {
    float length = sqrt(x*x + y*y + z*z);
    x /= length;
    y /= length;
    z /= length;
}

/* Size2F */


/* Size3F */


/* Rect2F */
float Rect2F::left() const {
    return topLeft.x;
}

float Rect2F::right() const {
    return topLeft.x + size.w;
}

float Rect2F::top() const {
    return topLeft.y;
}

float Rect2F::bottom() const {
    return topLeft.y + size.h;
}

Rect2F Rect2F::fromSides(float left, float right, float top, float bottom) {
    return Rect2F{left, top, right-left, bottom-top};
}

Rect2F Rect2F::getTranslated(Vect2F translation) const {
    return {
        topLeft.x + translation.x,
        topLeft.y + translation.y,
        size.w,
        size.h
    };
}


Rect2F Rect2F::getScaled(const float xScale, const float yScale) const {
    return {
        topLeft.x * xScale,
        topLeft.y * yScale,
        size.w * xScale,
        size.h * yScale
    };
}


/* Rect3F */

float Rect3F::left() const {
    return topLeft.x;
}

float Rect3F::right() const {
    return topLeft.x + size.w;
}

float Rect3F::top() const {
    return topLeft.y;
}

float Rect3F::bottom() const {
    return topLeft.y + size.h;
}

float Rect3F::front() const {
    return topLeft.z;
}

float Rect3F::back() const {
    return topLeft.z + size.d;
}


Rect3F Rect3F::getTranslated(Vect3F translation) const {
    return {
        topLeft.x + translation.x,
        topLeft.y + translation.y,
        topLeft.z + translation.z,
        size.w,
        size.h,
        size.d
    };
}

Rect3F Rect3F::fromSides(float left, float right, float top, float bottom, float front, float back) {
    return Rect3F{left, top, front, right-left, bottom-top, back-front};
}