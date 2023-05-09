#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif



typedef struct  vector_t     vector_t;

struct vector_t {
    void      (*clear)(vector_t *thiz);// but not free thiz

    void      (*free)(vector_t *thiz);  // free thiz


    unsigned int     (*typesize)(vector_t *thiz);

    unsigned int     (*empty)(vector_t *thiz);

    unsigned int     (*size)(vector_t *thiz);

    unsigned int     (*capacity)(vector_t *thiz);

    void      (*resize)(vector_t *thiz, unsigned int n, const void* val);

    void      (*reserve)(vector_t *thiz, unsigned int capacity);



    void*  (*back)(vector_t *thiz);

    void*  (*front)(vector_t *thiz);

    void*  (*at)(vector_t *thiz, unsigned int index);

    void*  (*data)(vector_t *thiz);



    void*  (*begin)(vector_t *thiz);

    void*  (*end)(vector_t *thiz);

    void*  (*rbegin)(vector_t *thiz);

    void*  (*rend)(vector_t *thiz);



    void      (*push_back)(vector_t *thiz, const void* val);

    void      (*push_front)(vector_t *thiz, const void* val);

    void      (*pop_back)(vector_t *thiz);//

    void      (*pop_front)(vector_t *thiz);//

    void*     (*erase)(vector_t *thiz, void* first, void* last);

    void*     (*remove)(vector_t *thiz, void* position);//


    void      (*assign)(vector_t *thiz, void* first, void* last);

    void      (*fill)(vector_t *thiz, void* position, unsigned int n, const void* val);

    void      (*insert)(vector_t *thiz, void* position, void* first, void* last);

    void      (*reverse)(vector_t *thiz);

    void      (*swap)(vector_t *thiz, vector_t *that);
};


vector_t* vector_alloc(unsigned int cnt, unsigned int typesize); //vector ElementSize


#ifdef  __cplusplus
}
#endif


#endif // VECTOR_H_INCLUDED
