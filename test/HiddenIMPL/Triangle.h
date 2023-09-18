//
// Created by ZhangHao on 2023/8/11.
//

#ifndef OOPC_TRIANGLE_H
#define OOPC_TRIANGLE_H
#include "Shape.h"

CLASS(Triangle, Shape)
    Vec2 P1;
    Vec2 P2;
)
CLASS_END(Triangle, Shape)

M_NEW(Triangle, Vec2 v0, Vec2 v1, Vec2 v2);

#endif //OOPC_TRIANGLE_H
