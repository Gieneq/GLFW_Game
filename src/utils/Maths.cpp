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


/* Rect4F */

Rect4F Rect4F::fromSides(float left, float right, float top, float bottom) {
    return Rect4F{left, top, right-left, bottom-top};
}

Rect4F Rect4F::getTranslated(Vect2F translation) const {
    return {
        topLeft.x + translation.x,
        topLeft.y + translation.y,
        size.w,
        size.h
    };
}


Rect4F Rect4F::getScaled(const float xScale, const float yScale) const {
    return {
        topLeft.x * xScale,
        topLeft.y * yScale,
        size.w * xScale,
        size.h * yScale
    };
}

std::array<Vect2F, 2> Rect4F::getDirectedPoints(const Vect2F& direction) const {

    if(direction.x > 0) {
        return std::array<Vect2F, 2>{topRight(), bottomRight()};
    }
    else if(direction.x < 0) {
        return std::array<Vect2F, 2>{topLeft, bottomLeft()};
    }
    else if(direction.y > 0) {
        return std::array<Vect2F, 2>{bottomRight(), bottomLeft()};
    }
    else if(direction.y < 0) {
        return std::array<Vect2F, 2>{topLeft, topRight()};
    }

    throw std::runtime_error("Direction is zero");
}

/* Rect 5F */
    
bool Cuboid5F::checkIntersection(const Cuboid6F* other) const {
    return !(left() >= other->right() || right() <= other->left() || top() >= other->bottom() || bottom() <= other->top() || front() <= other->back());
}

Cuboid5F Cuboid5F::fromSides(float left, float right, float top, float bottom, float front) {
    return Cuboid5F{left, top, right-left, bottom-top, front};
}

/* Cuboid6F */

Cuboid6F Cuboid6F::fromSides(float left, float right, float top, float bottom, float back, float front) {
    return Cuboid6F{left, top, back, right-left, bottom-top, front-back};
}
