#include <stdio.h>

typedef enum ShapeKind {
    CircleKind,
    RectangleKind
} ShapeKind;

typedef struct Shape {
    ShapeKind kind;
} Shape;

typedef struct Circle {
    struct Shape shape;
    float radius;
} Circle;

typedef struct Rectangle {
    struct Shape shape;
    float l, b;
} Rectangle;


#define PI 3.14157
float CircleArea(Circle *c) {
    return c->radius * c->radius * PI;
}


float RectangleArea(Rectangle *r) {
    return r->b * r->l;
}


float Area(Shape *shape) {
    switch (shape->kind) {
        case CircleKind: return CircleArea((Circle *)shape); 
        case RectangleKind: return RectangleArea((Rectangle *)shape); 
        // others
    }

    return -1.0f;
}

int main() {
    Circle c;
    c.shape.kind = CircleKind;
    c.radius = 1;
    printf("%f\n", Area(&c));
    return 0;
}