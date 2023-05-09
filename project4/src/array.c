#include  "array.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct array_im_t  array_im_t;

struct array_im_t {
    array_t            thiz;
    void*              first;
    void*              last;
    unsigned int       typesize;
};


static void array_im_free(array_t *thiz) {  // free thiz
    array_im_t *ithiz = (array_im_t*)thiz;
    free(ithiz->first);
    ithiz->first = NULL;
    ithiz->last  = NULL;
    free(ithiz);
}

static unsigned int  array_im_size(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*) thiz;
    unsigned int size = ithiz->last - ithiz->first;
    size = size / ithiz->typesize;
    return size;
}

static unsigned int  array_im_typesize(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    return ithiz->typesize;
}

static void* array_im_back(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    void* val = ithiz->last - ithiz->typesize;
    return val;
}

static void* array_im_front(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    return ithiz->first;
}

static void* array_im_at(array_t *thiz, unsigned int index) {
    array_im_t *ithiz = (array_im_t*)thiz;
    void* val = ithiz->last;
    if (index >= thiz->size(thiz))
        return val;
    val =(ithiz->first + index * ithiz->typesize);
    return val;
}

static void* array_im_data(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*) thiz;
    return ithiz->first;
}



static void* array_im_begin(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;    
    return ithiz->first;
}

static void* array_im_end(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    return ithiz->last;
}

static void* array_im_rbegin(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    void* val = ithiz->last - ithiz->typesize;
    return val;
}

static void* array_im_rend(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    void* val = ithiz->first - ithiz->typesize;
    return val;
}



static void array_im_reverse(array_t *thiz) {
    array_im_t *ithiz = (array_im_t*)thiz;
    void* first = ithiz->first, *last = ithiz->last - ithiz->typesize;
    void* ptr  = malloc(ithiz->typesize);
    for (; first < last; first = first + ithiz->typesize, last = last - ithiz->typesize) {
        memcpy(ptr,  first, ithiz->typesize);
        memcpy(first, last,  ithiz->typesize);
        memcpy(last,  ptr,  ithiz->typesize);
    }
    free(ptr);
}

static void array_im_fill(array_t *thiz, const void* val) {
    array_im_t *ithiz = (array_im_t*) thiz;
    void* ptr = NULL;
    for(ptr = ithiz->first; ptr < ithiz->last; ptr += ithiz->typesize) {
        memcpy(ptr, val, ithiz->typesize);
    }
}

static void array_im_assign(array_t *thiz, void* first, void* last) {
    array_im_t *ithiz = (array_im_t*) thiz;
    if (first == NULL || last == NULL || first >= last) {
        return;
    }

    unsigned int size = ithiz->last - ithiz->first;
    unsigned int newsize = last - first;
    if (newsize > size)
        newsize = size;
    memcpy(ithiz->first, first, newsize);
}

static void  array_im_swap(array_t *thiz, array_t *that) {
    void* ptr = NULL;
    array_im_t *ithiz = (array_im_t*) thiz, *ithat = (array_im_t*)that;
    if (that == NULL) {
        return;
    }
    ptr = ithiz->first;
    ithiz->first = ithat->first;
    ithat->first = ptr;

    ptr = ithiz->last;
    ithiz->last = ithat->last;
    ithat->last = ptr;

    unsigned int typesize = ithiz->typesize;
    ithiz->typesize = ithat->typesize;
    ithat->typesize = typesize;
}


array_t* array_alloc(unsigned int cnt, unsigned int typesize) { //n - count   c - ElementSize
    array_im_t *ithiz = NULL;
    array_t  *thiz  = NULL;
    if (cnt <= 0 || typesize <= 0) {
        return thiz;
    }
    ithiz = malloc(sizeof(array_im_t));
    ithiz->first = malloc(cnt * typesize);
    ithiz->last  = ithiz->first + cnt * typesize;
    ithiz->typesize = typesize;// unit typesize > 0

    thiz     = &(ithiz->thiz);
    thiz->free  = array_im_free;  // free thiz
    thiz->back  = array_im_back;
    thiz->front  = array_im_front;
    thiz->begin  = array_im_begin;
    thiz->end    = array_im_end;
    thiz->rbegin = array_im_rbegin;
    thiz->rend   = array_im_rend;
    thiz->reverse  = array_im_reverse;
    thiz->at  = array_im_at;
    thiz->fill  = array_im_fill;
    thiz->size  = array_im_size;
    thiz->typesize  = array_im_typesize;
    thiz->data  = array_im_data;
    thiz->assign  = array_im_assign;
    thiz->swap  = array_im_swap;
    return thiz;
}
