#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct avlnode_t       avlnode_t;

struct avlnode_t {
    avlnode_t* (*left)(avlnode_t *thiz);
    avlnode_t* (*right)(avlnode_t *thiz);
    avlnode_t* (*parent)(avlnode_t *thiz);
    avlnode_t* (*rotate)(avlnode_t *thiz, int dir); // dir: 0 - left rotate   1 - right rotate  2 - left right rotate  3 - right left rotate
    int        (*height)(avlnode_t *thiz);
    int        (*degree)(avlnode_t *thiz);
    void*      (*data)(avlnode_t *thiz);
    void*      (*key)(avlnode_t *thiz);

};

typedef struct avltree_t       avltree_t;

struct avltree_t {
    void              (*free)(avltree_t    *thiz);

    void              (*clear)(avltree_t  *thiz);

    unsigned int      (*typesize)(avltree_t *thiz);

    unsigned int      (*keysize)(avltree_t *thiz);

    unsigned int      (*empty)(avltree_t *thiz);

    unsigned int      (*size)(avltree_t *thiz);

    unsigned int      (*height)(avltree_t *thiz);

    unsigned int      (*degree)(avltree_t *thiz);


    avlnode_t*        (*begin)(avltree_t *thiz);

    avlnode_t*        (*end)(avltree_t *thiz);

    avlnode_t*        (*min)(avltree_t *thiz);

    avlnode_t*        (*max)(avltree_t *thiz);

    avlnode_t*        (*find)(avltree_t *thiz, const void* key);


    avlnode_t*        (*remove)(avltree_t *thiz, const void* val);

    avlnode_t*        (*insert)(avltree_t *thiz, const void* val);

    void              (*traverse)(avltree_t *thiz);

    void              (*swap)(avltree_t *thiz, avltree_t *that);

};
    
avltree_t* avltree_alloc(unsigned int cnt, unsigned int typesize);


#ifdef  __cplusplus
}
#endif


#endif // AVLTREE_H_INCLUDED