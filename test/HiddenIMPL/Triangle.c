//
// Created by ZhangHao on 2023/8/11.
//
#include "Triangle.h"

IMPL_CLASS(Triangle, Shape)

M_NEW(Triangle, Vec2 v0, Vec2 v1, Vec2 v2) {
    CtorSuper(Shape, NEW, v0.x, v0.y);
    this->P1 = v1;
    this->P2 = v2;
    return this;
}

OVERRIDE_METHOD(Shape, Triangle, float, GetArea) {
    Triangle *self = ((Triangle *)this);
    Vec2 l01 = {self->P1.x - this->Origin.x, self->P1.y - this->Origin.y};
    Vec2 l02 = {self->P2.x - this->Origin.x, self->P2.y - this->Origin.y};
    float A = l01.x * l02.y - l01.y * l02.x;
    return A > 0 ? A / 2 : - A / 2;
}

Destructor(Triangle) {

}