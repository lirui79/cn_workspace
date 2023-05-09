#include  "vector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>



typedef struct vector_im_t  vector_im_t;

struct vector_im_t {
    vector_t       thiz;
    void*          first;
    void*          last;
    void*          end;
    unsigned int   typesize;// elementSize typesize > 0
};
// vector
// first                last             end
// |                     |               |
// V                     V               V
// +-------------------------------------+
// |    ... data ...     |               |
// +-------------------------------------+
// |<--------size()----->|
// |<---------------capacity()---------->|

static void vector_im_clear(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    ithiz->last = ithiz->first;
}

static void vector_im_free(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (ithiz->first == NULL) {
        free(ithiz);
        return;
    }
    free(ithiz->first);
    ithiz->first = NULL;
    ithiz->last  = NULL;
    ithiz->end   = NULL;
    free(ithiz);
}

static unsigned int  vector_im_typesize(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    return ithiz->typesize;
}

static unsigned int  vector_im_empty(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (ithiz->first == ithiz->last)
        return 1;
    return 0;
}

static unsigned int  vector_im_size(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    unsigned int size = ithiz->last - ithiz->first;
    size = (size / ithiz->typesize);
    return size;
}

static unsigned int vector_im_capacity(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    unsigned int capacity = ithiz->end - ithiz->first;
    capacity = (capacity / ithiz->typesize);
    return capacity;
}

static void vector_im_resize(vector_t *thiz, unsigned int n, const void* val) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (ithiz->first == NULL) {
        ithiz->first = malloc(n * ithiz->typesize);
        ithiz->end  = ithiz->first + n * ithiz->typesize;
    } else {
        if (n > thiz->capacity(thiz)) {
            free(ithiz->first);
            ithiz->first = malloc(n * ithiz->typesize);
            ithiz->end  = ithiz->first + n * ithiz->typesize;
        }
    }

    ithiz->last = ithiz->first;
    for (unsigned int i = 0; i < n; ++i, ithiz->last += ithiz->typesize) {
        memcpy(ithiz->last, val, ithiz->typesize);
    }
}

static void vector_im_reserve(vector_t *thiz, unsigned int capacity) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (capacity <= 0)
        return;
    capacity = capacity * ithiz->typesize;
    unsigned int size = ithiz->end - ithiz->first;
    if (size == capacity)
        return;
    void* ptr = malloc(capacity);
    size = ithiz->last - ithiz->first;
    if (size > capacity)
        size = capacity;
    if (size > 0)
        memcpy(ptr, ithiz->first, size);
    free(ithiz->first);
    ithiz->first = ptr;
    ithiz->last  = ptr + size;
    ithiz->end   = ptr + capacity;
}


static void* vector_im_back(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void* val = NULL;
    unsigned int size = ithiz->last - ithiz->first;
    if (size < ithiz->typesize)
        return ithiz->last;
    val = ithiz->last - ithiz->typesize;
    return val;
}

static void* vector_im_front(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (ithiz->first == ithiz->last)
        return ithiz->last;
    return ithiz->first;
}

static void*  vector_im_at(vector_t *thiz, unsigned int index) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void* val = ithiz->last;
    if (index >= thiz->size(thiz))
        return val;
    val = (ithiz->first + index * ithiz->typesize);
    return val;
}

static void*  vector_im_data(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    return ithiz->first;
}



static void* vector_im_begin(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    return ithiz->first;
}

static void* vector_im_end(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    return ithiz->last;
}

static void* vector_im_rbegin(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void *val = ithiz->last - ithiz->typesize;
    return val;
}

static void* vector_im_rend(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void *val = ithiz->first - ithiz->typesize;
    return val;
}



static void vector_im_push_back(vector_t *thiz, const void* val) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    thiz->fill(thiz, ithiz->last, 1, val);
}

static void vector_im_push_front(vector_t *thiz, const void* val) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    thiz->fill(thiz, ithiz->first, 1, val);
}

static void vector_im_pop_back(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    unsigned int size = ithiz->last - ithiz->first;
    if (size < ithiz->typesize)
        return;
    ithiz->last = ithiz->last - ithiz->typesize;
}

static void vector_im_pop_front(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void* first;
    unsigned int size = ithiz->last - ithiz->first;
    if (size < ithiz->typesize)
        return;
    for (first = ithiz->first;first < ithiz->last; first = first + ithiz->typesize)
        memcpy(first, first + ithiz->typesize, ithiz->typesize);
    ithiz->last = ithiz->last - ithiz->typesize;
}

static void* vector_im_erase(vector_t *thiz, void* first, void* last) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (first == NULL || last == NULL || first >= last)
        return first;
    if (first < ithiz->first)
        first = ithiz->first;
    if (last > ithiz->last)
        last = ithiz->last;
    for (;last < ithiz->last; first = first + ithiz->typesize, last = last + ithiz->typesize)
        memcpy(first, last, ithiz->typesize);
    ithiz->last = first;
    return first;
}

static void* vector_im_remove(vector_t *thiz, void* position) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    return thiz->erase(thiz, position, position + ithiz->typesize);
}

static void vector_im_assign(vector_t *thiz, void* first, void* last) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (first == NULL || last == NULL || first >= last)
        return;
    unsigned int size = (last - first);

    if (ithiz->first == NULL) {
        ithiz->first = malloc(size);
        ithiz->end  = ithiz->first + size;
    } else {
        if (size > (ithiz->end - ithiz->first)) {
            free(ithiz->first);
            ithiz->first = malloc(size);
            ithiz->end  = ithiz->first + size;
        }
    }

    ithiz->last = ithiz->first + size;
    memcpy(ithiz->first, first, size);
}

static void vector_im_fill(vector_t *thiz, void* position, unsigned int n, const void* val) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    void* data = val;
    if (position == NULL || data == NULL || n <= 0)
        return;
    if (position < ithiz->first || position > ithiz->last)
        return;
    unsigned int newsize = n * ithiz->typesize, size = ithiz->last - ithiz->first, capacity = ithiz->end - ithiz->first;
    if (newsize + size > capacity) {
        void* ptr = malloc(2 * (newsize + size));
        unsigned int psize = position - ithiz->first;
        if (psize > 0)
            memcpy(ptr, ithiz->first, psize);
        for (unsigned int step = 0; step < newsize; step = step + ithiz->typesize)
            memcpy(ptr + psize + step, val, ithiz->typesize);
        if (size > psize)
            memcpy(ptr + psize + newsize, position, size - psize);
        free(ithiz->first);
        ithiz->first = ptr;
        ithiz->last  = ptr + newsize + size;
        ithiz->end   = ptr + 2 * (newsize + size);
        return;
    }
    void* ptr = ithiz->last - ithiz->typesize;
    for ( ;ptr >= position; ptr = ptr - ithiz->typesize) {
        memcpy(ptr + newsize, ptr, ithiz->typesize);
    }

    for (unsigned int step = 0; step < newsize; step = step + ithiz->typesize)
        memcpy(position + step, val, ithiz->typesize);
    ithiz->last = ithiz->last + newsize;
}

static void vector_im_insert(vector_t *thiz, void* position, void* first, void* last) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (position == NULL || first == NULL || last == NULL)
        return;
    if (position < ithiz->first || position > ithiz->last || first >= last)
        return;
    unsigned int newsize = last - first, size = ithiz->last - ithiz->first, capacity = ithiz->end - ithiz->first;
    if (newsize + size > capacity) {
        void* ptr = malloc(2 * (newsize + size));
        unsigned int psize = position - ithiz->first;
        if (psize > 0)
            memcpy(ptr, ithiz->first, psize);
        memcpy(ptr + psize, first, newsize);
        if (size > psize)
            memcpy(ptr + psize + newsize, position, size - psize);
        free(ithiz->first);
        ithiz->first = ptr;
        ithiz->last  = ptr + newsize + size;
        ithiz->end   = ptr + 2 * (newsize + size);
        return;
    }
    void* ptr = ithiz->last - ithiz->typesize;
    for ( ;ptr >= position; ptr = ptr - ithiz->typesize) {
        memcpy(ptr + newsize, ptr, ithiz->typesize);
    }
    memcpy(position, first, newsize);
    ithiz->last = ithiz->last + newsize;
}

static void vector_im_reverse(vector_t *thiz) {
    vector_im_t *ithiz = (vector_im_t*)thiz;
    if (ithiz->first == ithiz->last)
        return;
    void* first = ithiz->first, *last = ithiz->last - ithiz->typesize;
    void* ptr  = malloc(ithiz->typesize);
    for (; first < last; first = first + ithiz->typesize, last = last - ithiz->typesize) {
        memcpy(ptr,  first, ithiz->typesize);
        memcpy(first, last,  ithiz->typesize);
        memcpy(last,  ptr,  ithiz->typesize);
    }
    free(ptr);
}

static    void vector_im_swap(vector_t *thiz, vector_t *that) {
    void* ptr = NULL;
    vector_im_t *ithiz = (vector_im_t*) thiz, *ithat = (vector_im_t*)that;
    if (that == NULL) {
        return;
    }
    ptr = ithiz->first;
    ithiz->first = ithat->first;
    ithat->first = ptr;

    ptr = ithiz->last;
    ithiz->last = ithat->last;
    ithat->last = ptr;

    ptr = ithiz->end;
    ithiz->end = ithat->end;
    ithat->end = ptr;

    unsigned int c = ithiz->typesize;
    ithiz->typesize = ithat->typesize;
    ithat->typesize = c;
}


vector_t* vector_alloc(unsigned int cnt, unsigned int typesize) {//vector unit size
    vector_im_t* ithiz = NULL;
    vector_t*  thiz  = NULL;
    if (cnt <= 0 || typesize <= 0) {
        return thiz;
    }
    ithiz = (vector_im_t*) malloc(sizeof(vector_im_t));
    ithiz->first = malloc(cnt * typesize);
    ithiz->last  = ithiz->first;
    ithiz->end   = ithiz->first + cnt * typesize;
    ithiz->typesize  = typesize;// unit typesize > 0

    thiz     = &(ithiz->thiz);
    thiz->clear  = vector_im_clear;// but not free thiz
    thiz->free   = vector_im_free;
    thiz->typesize   = vector_im_typesize;

    thiz->empty    = vector_im_empty;
    thiz->size     = vector_im_size;
    thiz->capacity = vector_im_capacity;
    thiz->resize   = vector_im_resize;
    thiz->reserve  = vector_im_reserve;

    thiz->back     = vector_im_back;
    thiz->front    = vector_im_front;
    thiz->at       = vector_im_at;
    thiz->data     = vector_im_data;

    thiz->begin    = vector_im_begin;
    thiz->end      = vector_im_end;
    thiz->rbegin   = vector_im_rbegin;
    thiz->rend     = vector_im_rend;

    thiz->push_back  = vector_im_push_back;
    thiz->push_front = vector_im_push_front;
    thiz->pop_back   = vector_im_pop_back;
    thiz->pop_front  = vector_im_pop_front;
    thiz->erase      = vector_im_erase;
    thiz->remove     = vector_im_remove;

    thiz->assign     = vector_im_assign;
    thiz->fill       = vector_im_fill;
    thiz->insert     = vector_im_insert;
    thiz->reverse    = vector_im_reverse;
    thiz->swap       = vector_im_swap;
    return thiz;
}
