//
// Created by ZhangHao on 2023/8/3.
//
#include "Shape.h"
IMPL_CLASS(Shape, CObject)

M_NEW(Shape, float x, float y) {
    this->Origin.x = x;
    this->Origin.y = y;
    return this;
}

MF_IMPL(Shape)
float, GetArea) {
    return 0;
}

Destructor(Shape) {

}
