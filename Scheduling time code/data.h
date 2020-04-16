#pragma once

struct Data {
    int id;
    float x, y;
};


struct Data Data0(int id) {
    struct Data d;
    d.id = id;
    d.x = 0;
    d.y = 0;
    return d;
};

struct Data Data(int id,float x) {
    struct Data d;
    d.id = id;
    d.x = x;
    d.y = 0;
    return d;
};

struct Data Data2(int id,float x, float y) {
    struct Data d;
    d.id = id;
    d.x = x;
    d.y = y;
    return d;
};
