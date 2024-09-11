
#ifndef CLASS_H_INCLUDED_
#define CLASS_H_INCLUDED_

#include <stddef.h>
#include <malloc.h>
//offsetof(type, member-designator)

// Interface Class
#define ILASS(TYPE)                     \
typedef struct TYPE TYPE;               \
void TYPE##_ctor(TYPE* cthis);          \
int TYPE##_dtor(TYPE* cthis);           \
struct TYPE

// Virtual Class
#define VLASS(TYPE)                     \
typedef struct TYPE TYPE;               \
void TYPE##_ctor(TYPE* cthis);          \
void TYPE##_delete(TYPE* cthis);        \
int TYPE##_dtor(TYPE* cthis);           \
struct TYPE

// Class
#define CLASS(TYPE)                     \
typedef struct TYPE TYPE;               \
TYPE* TYPE##_new();                     \
void TYPE##_ctor(TYPE* cthis);          \
void TYPE##_delete(TYPE* cthis);        \
int TYPE##_dtor(TYPE* cthis);           \
struct TYPE


#define CTOR(TYPE)                                      \
    TYPE* TYPE##_new() {                                \
    struct TYPE *cthis;                                 \
    cthis = (struct TYPE*)malloc(sizeof(struct TYPE));  \
    if(!cthis) {                                        \
        return NULL;                                    \
    }                                                   \
    TYPE##_ctor(cthis);                                 \
    return cthis;                                       \
}                                                       \
                                                        \
void TYPE##_ctor(TYPE* cthis) {


#define ETOR	}

#define DTOR(TYPE)                  \
void TYPE##_delete(TYPE* cthis)     \
{                                   \
   if(TYPE##_dtor(cthis)) {         \
      free(cthis);                  \
   }                                \
}                                   \
                                    \
int TYPE##_dtor(TYPE* cthis)        \
{


#define CTDE(TYPE)                  \
void TYPE##_ctor(TYPE* cthis) {


#define DTDE(TYPE)                  \
int TYPE##_dtor(TYPE* cthis) {


#define CALL_CTOR(TYPE, PTR, MEM)    TYPE##_ctor(&(PTR->MEM))

#define CALL_DTOR(TYPE, PTR, MEM)    TYPE##_dtor(&(PTR->MEM))

#define UPER_PTR(TYPE, PTR, MEM)     (TYPE*)(&(PTR->MEM))

#define FUNC_SET(PTR, FUNC1, FUNC2)  PTR->FUNC1 = FUNC2



#endif