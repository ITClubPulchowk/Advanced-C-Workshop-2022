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
float CircleArea(void *ptr) {
    Circle *c = (Circle *)ptr;
    return c->radius * c->radius * PI;
}


float RectangleArea(void *ptr) {
    Rectangle *r = (Rectangle *)ptr;
    return r->b * r->l;
}

typedef float (*AreaProc) (void *);

static const AreaProc Workers[] = {CircleArea, RectangleArea};

float Area(Shape *shape) {
    return Workers[shape->kind]((void *)shape); 
}

int main() {
  Circle c;
  c.shape.kind = CircleKind;
  c.radius = 4.71013741469;
  printf("%f\n", Area((Shape *)&c));

  Rectangle r; 
  r.shape.kind = RectangleKind;
  r.l = 13;
  r.b = 5;
  printf("%f\n", Area((Shape *)&r));
  return 0;
}