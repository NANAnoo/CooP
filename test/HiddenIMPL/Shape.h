//
// Created by ZhangHao on 2023/8/3.
//

#ifndef OOPC_TEST_H
#define OOPC_TEST_H
#include "CClass.h"

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

CLASS(Shape, CObject,
      Vec2 Origin;
)
    float MF(Shape, GetArea);
CLASS_END(Shape, CObject)

M_NEW(Shape, float x, float y);

#endif //OOPC_TEST_H
