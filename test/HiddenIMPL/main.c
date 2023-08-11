#include "Shape.h"
#include "Triangle.h"

int main() {
    Vec2 a = {0, 0};
    Vec2 b = {4, 2};
    Vec2 c = {1, 3};
    Triangle *tri = NEW(Triangle, a, b, c);
    printf("Area of triangle is : %f \n", MCall(tri, Shape , GetArea));
    DELETE(tri);
}