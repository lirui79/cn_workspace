#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct array_t     array_t;

struct array_t {
    void            (*free)(array_t *thiz);  // free thiz

    unsigned int    (*size)(array_t *thiz);

    unsigned int    (*typesize)(array_t *thiz);

    void*           (*back)(array_t *thiz);

    void*           (*front)(array_t *thiz);

    void*           (*at)(array_t *thiz, unsigned int index);

    void*           (*data)(array_t *thiz);

    void*           (*begin)(array_t *thiz);

    void*           (*end)(array_t *thiz);

    void*           (*rbegin)(array_t *thiz);

    void*           (*rend)(array_t *thiz);

    void            (*reverse)(array_t *thiz);

    void            (*fill)(array_t *thiz, const void* val);

    void            (*assign)(array_t *thiz, void* first, void* last);

    void            (*swap)(array_t *thiz, array_t *that);
};


array_t* array_alloc(unsigned int cnt, unsigned int typesize);


#ifdef  __cplusplus
}
#endif


#endif // ARRAY_H_INCLUDED