
#ifndef CLASS_H_INCLUDED_
#define CLASS_H_INCLUDED_

#include <stddef.h>
#include <malloc.h>
//offsetof(type, member-designator)


#define class(TYPE)                     \
typedef struct TYPE TYPE;               \
void TYPE##_construct(TYPE* cthis);     \
int  TYPE##_destruct(TYPE* cthis);      \
struct TYPE


#define class_new(TYPE)              \
TYPE* TYPE##_new()

#define class_del(TYPE)              \
void  TYPE##_del()

#define class_func(TYPE, FUNC, ...)   (*FUNC)(TYPE *cthis, ##__VA_ARGS__)
#define func_ptr(FUNC, ...)   (*FUNC)(__VA_ARGS__)

#define declare_new(TYPE)                               \
TYPE* TYPE##_new() {                                    \
    struct TYPE *cthis;                                 \
    cthis = (struct TYPE*)malloc(sizeof(struct TYPE));  \
    if(!cthis) {                                        \
        return NULL;                                    \
    }                                                   \
    TYPE##_construct(cthis);                            \
    return cthis;                                       \
}


#define declare_del(TYPE)           \
void TYPE##_del(TYPE* cthis)        \
{                                   \
   if(TYPE##_destruct(cthis)) {     \
      free(cthis);                  \
   }                                \
}


#define declare_func(TYPE, FUNC, ...)  TYPE##_##FUNC(__VA_ARGS__)

#define declare_construct(TYPE)     \
void TYPE##_construct(TYPE* cthis)

#define declare_destruct(TYPE)      \
int  TYPE##_destruct(TYPE* cthis)


#define func_init(TYPE, FUNC)   cthis->FUNC = TYPE##_##FUNC

#define func_set(PTR, FUNC1, FUNC2)       PTR->FUNC1 = FUNC2

#define class_construct(TYPE, PTR, MEM)    TYPE##_construct(&(PTR->MEM))

#define class_destruct(TYPE, PTR, MEM)     TYPE##_destruct(&(PTR->MEM))

#define class_mem(TYPE, PTR, MEM)         (TYPE*)(&(PTR->MEM))

#define class_ptr(TYPE, PTR)              (TYPE*)(PTR)



#endif