#pragma once

struct Vect2F {
    float x{0};
    float y{0};
};

struct Size2F {
    float w{0};
    float h{0};
};

struct Rect2F {
    Vect2F top_left;
    Size2F size;
};