//
// Created by Nanorth on 2023/8/2.
//

//#define MALLOC_DEBUG

#include "CClass.h"
#include <stdio.h>

Interface(Printable,
          void virtual(Print, const char *);
)

Interface(Testable,
          void virtual(Test, const char *);
)

CLASS(Base, CObject,
      int x;
)I(Printable)I(Testable)

    void MF(Base, VMethod);

CLASS_END(Base, CObject)

// declare and implement interface Printable::Print
IMPL(Base, Printable)
void, Print, const char *msg)
{
printf("Base implement print %d, with msg %s \n", this->x, msg);
}

// declare and implement member function Base::VMethod()
MF_IMPL(Base)
void, VMethod) {
printf("Base VMethod %d \n", this->x);
}

M_NEW(Base, int val) {
    this->x = val;
    printf("New Base %d \n", this->x);
    return this;
}
M_COPY(Base)
{
    this->x = other->x;
    printf("Copy Base %d \n", this->x);
    return this;
}
M_MOVE(Base)
    this = *other;
    *other = NULL;
    printf("Move Base %d \n", this->x);
    return this;
}
Destructor(Base) {
    printf("Base dealloc %d \n", this->x);
}

CLASS(Father, Base,
      int y;
)
    void MF(Father, TestMethod2);
CLASS_END(Father, Base)

M_NEW(Father, int val) {
    CtorSuper(Base, NEW, val + 1);
    this->y = val;
    printf("New Father %d \n", this->y);
    return this;
}
M_COPY(Father) {
    CtorSuper(Base, COPY, (Base *)other);
    this->y = other->y;
    printf("COPY Father %d \n", this->y);
    return this;
}
Destructor(Father) {
    printf("Father dealloc %d \n", this->y);
}

CLASS(Son, Father,
      int z;
)
    void MF(Son, TestMethod3);
CLASS_END(Son, Father)

MF_IMPL(Son)
void , TestMethod3) {
printf("Son TestMethod %d \n", this->z);
}

OVERRIDE_METHOD(Base, Son, void, VMethod) {
    printf("Son VMethod %d \n", ((Son *)(this))->z);
}

M_NEW(Son, int val, int par) {
    CtorSuper(Father, NEW, par);
    this->z = val;
    printf("New Son %d \n", this->z);
    return this;
}
Destructor(Son) {
    printf("Son dealloc %d \n", this->z);
}

#define SharedPtr(T) \
CLASS(SharedPtr##T, CObject,\
        T *ptr;\
        int *ref; \
)\
CLASS_END(SharedPtr##T, CObject)\
M_NEW(SharedPtr##T, T *Other) { \
    this->ref = m_malloc(sizeof(int), ""#T" *obj"); \
    *this->ref = 1; \
    this->ptr = Other; \
    return this; \
} \
M_COPY(SharedPtr##T) { \
    this->ref = other->ref; \
    this->ptr = other->ptr; \
    ++(*this->ref); \
    return this; \
}\
Destructor(SharedPtr##T) { \
    (*this->ref) --; \
    if ((*this->ref) <= 0) { \
        m_free(this->ref, ""#T" *obj"); \
        DELETE(this->ptr);\
    }\
}                    \
typedef SharedPtr##T * Shared##T;       \
void SharedPtr##T##CleanUp(SharedPtrBase **ref) {if (ref && (*ref)) {DELETE(*ref);}}

#define SHARED(T) __attribute((cleanup(SharedPtr##T##CleanUp))) SharedPtr##T*
#define Get(P) P->ptr
#define Ref(T, P) COPY(SharedPtr##T, P)
#define MAKE_SHARED(T, ...) SharedPtr##T##_Cons_NEW(SharedPtr##T##_Ctor(), Base_Cons_NEW(T##_Ctor() ,##__VA_ARGS__))

// declare class shared ptr
SharedPtr(Base)
// Describe body

IMPL_CLASS(SharedPtrBase , CObject)
IMPL_CLASS(Base, CObject)
IMPL_CLASS(Father , Base)
IMPL_CLASS(Son , Father)

int main() {
    SHARED(Base) res = MAKE_SHARED(Base, 10);
    SHARED(Base) ref2 = Ref(Base, res);
    printf("class is %s \n", ClassOf(Get(ref2))->_name_);
    return 0;
}