#include "Maths.h"


Vect2F Vect2F::get_negated() const {
    return Vect2F{-x, -y};
}


Vect2F Vect2F::getTranslated(const Vect2F& translation) const {
    return Vect2F{x + translation.x, y + translation.y};
}


Vect2F Vect3F::getXY() const {
    return Vect2F{x, y};
}

float Rect2F::left() const {
    return top_left.x;
}

float Rect2F::right() const {
    return top_left.x + size.w;
}

float Rect2F::top() const {
    return top_left.y;
}

float Rect2F::bottom() const {
    return top_left.y + size.h;
}

Rect2F Rect2F::from_sides(float left, float right, float top, float bottom) {
    return Rect2F{left, top, right-left, bottom-top};
}

Rect2F Rect2F::get_translated(Vect2F translation) const {
    return {
        this->top_left.x + translation.x,
        this->top_left.y + translation.y,
        size.w,
        size.h
    };
}

Rect2F Rect2F::get_scaled(Size2F scale) const {
    return from_sides(
        left() * scale.w,
        right() * scale.w,
        top() * scale.h,
        bottom() * scale.h
    );
}


/* Rect3F */

float Rect3F::left() const {
    return top_left.x;
}

float Rect3F::right() const {
    return top_left.x + size.w;
}

float Rect3F::top() const {
    return top_left.y;
}

float Rect3F::bottom() const {
    return top_left.y + size.h;
}

float Rect3F::front() const {
    return top_left.z;
}

float Rect3F::back() const {
    return top_left.z + size.l;
}

