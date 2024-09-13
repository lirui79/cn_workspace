
#ifndef OBJECT_H_INCLUDED_
#define OBJECT_H_INCLUDED_

#include "class.h"


#ifdef __cplusplus
extern "C"{
#endif


class(ObjectClass) {
    char cname[256];
};

class_new(ObjectClass);

class_del(ObjectClass);

class(Object) {
    ObjectClass  *_objectClass;
};

class_new(Object);

class_del(Object);


void Object_set(Object *obj, ObjectClass *objClass);

void Object_clear(Object *obj);

#define declare_new2(TYPE, CLASS)                       \
TYPE* TYPE##_new() {                                    \
    CLASS *cthiz = NULL;                                \
    TYPE  *cthis = NULL;                                \
    cthis = (struct TYPE*)malloc(sizeof(struct TYPE));  \
    if(!cthis) {                                        \
        return NULL;                                    \
    }                                                   \
    TYPE##_construct(cthis);                            \
    cthiz = CLASS##_new();                              \
    if (!cthiz) {                                       \
        TYPE##_del(cthis);                            \
        return NULL;                                    \
    }                                                   \
    Object_set((Object *)cthis, (ObjectClass *)cthiz);                       \
    return cthis;                                       \
}


#define declare_del2(TYPE, CLASS)                 \
void TYPE##_del(TYPE* cthis)                      \
{                                                 \
   CLASS *cls = class_ptr2(CLASS, cthis);         \
   if (cls != NULL) {                             \
       CLASS##_del(cls);                          \
       Object_clear(class_ptr(Object, cthis));    \
   }                                              \
   if(TYPE##_destruct(cthis)) {                   \
      free(cthis);                                \
   }                                              \
}



#define class_ptr2(CLASS, PTR)              (CLASS*)(((Object*)PTR)->_objectClass)

#ifdef __cplusplus
}
#endif


#endif //OBJECT_H_INCLUDED_