#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test.h"
#include "include/vector.h"


static void print_vector_int(vector_t *ivector) {
    int value = 0;
    void* it;
    printf("vector_t::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->begin(ivector); it < ivector->end(ivector); it += ivector->typesize(ivector)) {
        value = *(int *) it;
        printf("%x ", value);
    }
    printf("\n");
}

static void print_vector_int_back(vector_t *ivector) {
    int value = 0;
    void* it;
    printf("vector_t::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->rbegin(ivector); it > ivector->rend(ivector); it -= ivector->typesize(ivector)) {
        value = *(int *) it;
        printf("%x ", value);
    }
    printf("\n");
}

static void test_vector_int() {
    vector_t *ivector = vector_alloc(8, sizeof(int));
    int value = 0x11;
    ivector->resize(ivector, 20, &value);
    print_vector_int(ivector);
    print_vector_int_back(ivector);
    value= *(int*)ivector->data(ivector);
    printf("vector_t first %x\n", value);

    int buf[] = {0x21, 0x22, 0x43, 0x101020, 0x032390};
    void* first = buf, *last = buf + 5;

    ivector->assign(ivector, first, last);
    print_vector_int(ivector);
    print_vector_int_back(ivector);

    value = *(int*)ivector->data(ivector);
    printf("vector_t first %x\n", value);

    ivector->clear(ivector);
    printf("vector_t::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));

    ivector->free(ivector);
}


void test_vector() {
   test_vector_int();
}
