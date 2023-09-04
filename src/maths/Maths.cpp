#include "Maths.h"


/* Vect2F */

Vect2F Vect2F::getNegated() const {
    return Vect2F{-x, -y};
}

void Vect2F::negate() {
    x = -x;
    y = -y;
}

void Vect2F::translate(const Vect2F& translation) {
    this->translate(translation.x, translation.y);
}

void Vect2F::translate(const float x, const float y) {
    this->x += x;
    this->y += y;
}

Vect2F Vect2F::getTranslated(const Vect2F& translation) const {
    return Vect2F{x + translation.x, y + translation.y};
}

void Vect2F::normalize() {
    float length = this->length();
    x /= length;
    y /= length;
}

Vect2F Vect2F::getNormalized() const {
    float length = this->length();
    return Vect2F{x / length, y / length};
}

void Vect2F::scale(const float xScale, const float yScale) {
    x *= xScale;
    y *= yScale;
}

Vect2F Vect2F::getScaled(const float xScale, const float yScale) const {
    return Vect2F{x * xScale, y * yScale};
}


/* Vect3F */

void Vect3F::negate() {
    x = -x;
    y = -y;
    z = -z;
}

Vect3F Vect3F::getNegated() const {
    return Vect3F{-x, -y, -z};
}

void Vect3F::translate(const Vect3F& translation) {
    this->translate(translation.x, translation.y, translation.z);
}

void Vect3F::translate(const float x, const float y, const float z) {
    this->x += x;
    this->y += y;
    this->z += z;
}

Vect3F Vect3F::getTranslated(const Vect3F& translation) const {
    return Vect3F{x + translation.x, y + translation.y, z + translation.z};
}

Vect3F Vect3F::getTranslated(const float x, const float y, const float z) const {
    return Vect3F{x + this->x, y + this->y, z + this->z};
}

void Vect3F::normalize() {
    float length = this->length();
    x /= length;
    y /= length;
    z /= length;
}

Vect3F Vect3F::getNormalized() const {
    float length = this->length();
    return Vect3F{x / length, y / length, z / length};
}

void Vect3F::scale(const float xScale, const float yScale, const float zScale) {
    x *= xScale;
    y *= yScale;
    z *= zScale;
}

Vect3F Vect3F::getScaled(const float xScale, const float yScale, const float zScale) const {
    return Vect3F{x * xScale, y * yScale, z * zScale};
}

Vect2F Vect3F::getXY() const {
    return Vect2F{x, y};
}


/* Size2F */


/* Size3F */


/* Rect4F */

Rect4F Rect4F::fromSides(float left, float right, float top, float bottom) {
    return Rect4F{left, top, right-left, bottom-top};
}

Rect4F Rect4F::getTranslated(Vect2F translation) const {
    return {
        topLeftCorner.x + translation.x,
        topLeftCorner.y + translation.y,
        size.w,
        size.h
    };
}


Rect4F Rect4F::getScaled(const float xScale, const float yScale) const {
    return {
        topLeftCorner.x * xScale,
        topLeftCorner.y * yScale,
        size.w * xScale,
        size.h * yScale
    };
}

std::array<Vect2F, 2> Rect4F::getPointsInDirection(const Vect2F& direction) const {

    if(direction.x > 0) {
        return std::array<Vect2F, 2>{topRight(), bottomRight()};
    }
    else if(direction.x < 0) {
        return std::array<Vect2F, 2>{topLeft(), bottomLeft()};
    }
    else if(direction.y > 0) {
        return std::array<Vect2F, 2>{bottomRight(), bottomLeft()};
    }
    else if(direction.y < 0) {
        return std::array<Vect2F, 2>{topLeft(), topRight()};
    }

    throw std::runtime_error("Direction is zero");
}

/* Cuboid6F */

Cuboid6F::Cuboid6F(const Rect4F* rect) {
    topLeft = Vect3F{rect->x(), rect->y(), 0.0F};
    size = Size3F{rect->w(), rect->h(), 0.0F};
}

Cuboid6F Cuboid6F::fromSides(float left, float right, float top, float bottom, float back, float front) {
    return Cuboid6F{left, top, back, right-left, bottom-top, front-back};
}

void Cuboid6F::alignParentCuboidToOther(Cuboid6F& outer, const Cuboid6F inner, const Cuboid6F& other, const Vect2F& direction) {
    /* Align in X */
    if(direction.x > 0.0F) {
        outer.x() = other.x() - inner.width() - (inner.x() - outer.x());
    }
    else if(direction.x < 0.0F) {
        outer.x() = other.x() + other.width() - (inner.x() - outer.x());
    }

    /* Align in Y */
    if(direction.y > 0.0F) {
        outer.y() = other.y() - inner.height() - (inner.y() - outer.y());
    }
    else if(direction.y < 0.0F) {
        outer.y() = other.y() + other.height() - (inner.y() - outer.y());
    }
}

void Cuboid6F::placeParentCuboidOnOther(Cuboid6F& outer, const Cuboid6F inner, const Cuboid6F& other) {
    outer.z() = other.z() + other.depth() - (inner.z() - outer.z());
}