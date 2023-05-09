#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct iterator_t     iterator_t;

struct iterator_t {
    iterator_t*        (*next)(iterator_t *thiz);
    iterator_t*        (*prev)(iterator_t *thiz);
    int                (*eq)(GIterator *thiz, GIterator *that);
    int                (*nq)(GIterator *thiz, GIterator *that);

    int                (*lt)(GIterator *thiz, GIterator *that);
    int                (*le)(GIterator *thiz, GIterator *that);// less and equal

    int                (*gt)(GIterator *thiz, GIterator *that);
    int                (*ge)(GIterator *thiz, GIterator *that);// greater and equal
    void*              (*data)(iterator_t *thiz);
};




#ifdef  __cplusplus
}
#endif


#endif // ITERATOR_H_INCLUDED