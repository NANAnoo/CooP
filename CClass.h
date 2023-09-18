//
// Created by Nanorth on 2023/8/2.
//

#ifndef CCOT_CCLASS_H
#define CCOT_CCLASS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MALLOC_DEBUG

#ifdef MALLOC_DEBUG
static int m_count = 0, f_count = 0;
#endif
static void *m_malloc(size_t size, const char *type) {
#ifdef MALLOC_DEBUG
    m_count ++;
    printf("[%d]my malloc %s\n", m_count, type);
    fflush(stdout);
#endif
    return malloc(size);
}

static void m_free(void *ptr, const char *type) {
#ifdef MALLOC_DEBUG
    f_count ++;
    printf("[%d]my free %s\n", f_count, type);
    fflush(stdout);
#endif
    free(ptr);
}

#define Constructor(T, name, ...) T *T##_Cons_##name(T *this ,##__VA_ARGS__)
#define Destructor(T) void T##_Dtor(T *this)
#define CONSTRUCT(T, name, ...) T##_Cons_##name(T##_Ctor() ,##__VA_ARGS__)
#define DELETE(obj) Delete((CObject *)obj); obj = NULL

#define M_NEW(T, ...) \
Constructor(T, NEW ,##__VA_ARGS__)
#define NEW(T, ...) \
CONSTRUCT(T, NEW ,##__VA_ARGS__)

#define M_COPY(T) Constructor(T, COPY, T *other)
#define COPY(T, other) T##_Cons_COPY(T##_Ctor(), (other))

#define M_MOVE(T) T * T##_Cons_MOVE(T **other) { T *this = NULL;
#define MOVE(T, other) T##_Cons_MOVE(&other)

#define CtorSuper(T, name, ...) T##_Cons_##name((super), __VA_ARGS__)

typedef struct VTableRoot {

} VTableRoot;

#define VTable(Type, Super) \
struct VTable##Type {      \
     VTable##Super _root_;          \
     void (*_Dtor)(Type *);\
} *v_tbl;\

#define SIZE_OF_VTABLE(T) (sizeof(VTable##T) / sizeof(void(*)(void)))

typedef struct VTableCObject VTableCObject;
typedef struct Class Class;
typedef struct CObject {
    Class *isa;
} CObject;
struct Class {
    Class *isa;
    Class *_super_;
    const char *_name_;
    int v_Tbl_size;
};
struct CObjectClass {
    Class _meta_;
    VTable(CObject, Root);
};

// global c-object base
Class CObjectClassMeta;
VTableCObject VTableCObject_tbl;
struct CObjectClass CObjectClass;

static void CObject_Dtor(CObject *this) {
    if (this) m_free(this, this->isa->_name_);
}

static __attribute__((constructor)) void CONSTRUCT_C_OBJECT_BASE() {
    CObjectClassMeta.isa = NULL;
    CObjectClassMeta._super_ = (Class *)&CObjectClass;
    CObjectClassMeta._name_ = "CObjectClass";
    CObjectClassMeta.v_Tbl_size = 0;
    VTableCObject_tbl._Dtor = &CObject_Dtor;

    CObjectClass._meta_._super_ = &CObjectClassMeta;
    CObjectClass._meta_.isa = &CObjectClassMeta;
    CObjectClass._meta_._name_ = "CObject";
    CObjectClass._meta_.v_Tbl_size = sizeof(VTableCObject_tbl) / sizeof(void *);
    CObjectClass.v_tbl = &VTableCObject_tbl;
}

#define ClassOf(obj) ((CObject *)obj)->isa
#define SuperClassOf(obj) ((Class *)ClassOf(obj)->_super_)
#define MetaClassOf(obj) ((Class*)ClassOf(obj)->isa)
#define CLASS_VARIABLES_BEGIN(T, Super) struct T { Super _super_;
#define CLASS_VARIABLES_END };

#define CLASS(T, SUPER) CLASS_(T, SUPER,
#define CLASS_(T, SUPER, ...)\
    typedef struct T T;\
    struct T {\
        SUPER _super_;\
        __VA_ARGS__\
    };\
    typedef struct VTable##T {\
        VTable##SUPER _root_;\
        void (*_Dtor)(T *);

#define CLASS_END(T, SUPER) \
    } VTable##T;\
    struct T##Class {\
        Class _meta_;\
        VTable##T *v_tbl;\
    };\
    extern Class T##ClassMeta;\
    extern struct T##Class T##Class;\
    extern VTable##T VTable##T##_tbl;\
    T *T##_Ctor();\
    void T##_Dtor(T *);

#define IMPL_CLASS(T, SUPER)\
struct VTable##T VTable##T##_tbl = {{}, &T##_Dtor};\
__attribute((constructor)) void Default_Vtable##T##_Setup(){\
    VTable##T##_tbl._root_ = *((struct SUPER##Class*)(T##Class._meta_._super_))->v_tbl;\
}\
Class T##ClassMeta = {&SUPER##ClassMeta, (Class *) &(SUPER##Class), #T "Class"};\
struct T##Class T##Class = {\
    {\
        &T##ClassMeta,\
        (Class *) &SUPER##Class,\
        #T,\
        (sizeof(VTable##T) / sizeof(void (*)(void)))\
    },\
    &VTable##T##_tbl\
};\
T *T##_Ctor() {\
    T *res = m_malloc(sizeof(T), #T);\
    ((CObject *) res)->isa = (Class *) &T##Class;\
    return res;\
}

#define MF(T, name, ...) (*name)(T *this ,##__VA_ARGS__)

#define RAII(T, val, constructor, expr, ...) { \
     T val = {(Class *)&T##Class};                  \
     T##_Cons_##constructor(&val ,##__VA_ARGS__);\
     do {expr;}while(0);                                    \
     T##_Dtor(&val);                                        \
}

void static Delete(CObject *obj) {
    Class *s_cls = SuperClassOf(obj);
    void (**FP_ARR)(CObject *) = (void (**)(CObject *)) ((struct CObjectClass *) ClassOf(obj))->v_tbl;
    if (FP_ARR == NULL) return;
    while(s_cls)
    {
        int Dtor_Address = s_cls->v_Tbl_size;
        FP_ARR[Dtor_Address](obj);
        s_cls = Dtor_Address == 0 ? NULL : s_cls->_super_;
    }
}

#define M_METHOD_SEL(T, func) T##_##func

#define DEF_M_METHOD(T, ret, func, ...) \
ret M_METHOD_SEL(T, func)(T *this ,##__VA_ARGS__);

#define SET_M_METHOD(T, func) (VTable##T##_tbl.func = &M_METHOD_SEL(T, func))

#define MF_IMPL_(T, ret, func, ...) \
ret M_METHOD_SEL(T, func)(T *this ,##__VA_ARGS__); \
__attribute__((constructor)) static void CTOR_SET_METHOD_##T##func() \
{\
    SET_M_METHOD(T, func);\
}                              \
ret M_METHOD_SEL(T, func) (T *this ,##__VA_ARGS__)

#define MF_IMPL(T) MF_IMPL_(T,

#define OVERRIDE_METHOD(Super, T, ret, func, ...) \
ret M_METHOD_SEL(T, func)(Super * ,##__VA_ARGS__); \
__attribute__((constructor)) static void CTOR_SET_METHOD_##T##func() \
{\
    ((VTable##Super *)&VTable##T##_tbl)->func = &M_METHOD_SEL(T, func);\
}\
ret M_METHOD_SEL(T, func) (Super *this ,##__VA_ARGS__)


#define VTableOf(T, obj) ((struct T##Class *)((CObject *)(obj))->isa)->v_tbl
// [MCall] usage : MCall(object, Class, Function)
#define MCall(obj, T, func, ...) (VTableOf(T, obj)->func((T *)(obj) ,##__VA_ARGS__))

#define ICall(obj, T, I, func, ...) (VTableOf(T, obj)->I.func((void *)(obj) ,##__VA_ARGS__))

#define super (&this->_super_)

int static IsClass(CObject *obj, const char *T) {
    Class *cls = obj->isa;
    while (cls && !strcmp(cls->_name_, T)) {
        cls = cls->_super_;
    }
    return cls == NULL;
}

#define DYNAMIC_CAST(T, obj) \
!IsClass((CObject *)obj, #T) ? (T *)obj : NULL

#define Interface(Name, ...) \
struct Name {__VA_ARGS__};

#define virtual(Func, ...) (*Func)(void * ,##__VA_ARGS__);
#define I(I) struct I I;

#define IMPL_IMP(T, I, func, ...) \
    T##_IMPL_##I##_##func(Base *this ,##__VA_ARGS__)
#define IMPL_DEF(T, I, func, ...) \
    T##_IMPL_##I##_##func(Base *this ,##__VA_ARGS__);\
    __attribute((constructor)) void T##_IMPL_##I##_##func_REGISTER() {\
        void ** attr = (void*)(&VTable##T##_tbl.I.func);\
        *attr = (void *)(&T##_IMPL_##I##_##func);\
    }

#define IMPL_(T, I, ret, func, ...)\
ret IMPL_DEF(T, I, func,##__VA_ARGS__)\
ret IMPL_IMP(T, I, func,##__VA_ARGS__)\

#define IMPL(T, I) IMPL_(T, I,

#endif //CCOT_CCLASS_H
