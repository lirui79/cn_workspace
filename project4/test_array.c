#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test.h"
#include "include/array.h"


static void print_array_int(array_t *iarray) {
    int value = 0;
    void *it, *end = iarray->end(iarray);
    printf("array_t::size %d \n", iarray->size(iarray));
    for (it = iarray->begin(iarray); it < end; it += iarray->typesize(iarray)) {
        value = *(int *) it;
        printf("%x ", value);
    }
    printf("\n");
}

static void print_array_int_r(array_t *iarray) {
    int value = 0;
    void *it, *rend = iarray->rend(iarray);
    printf("iarray::size %d \n", iarray->size(iarray));
    for (it = iarray->rbegin(iarray); it > rend; it -= iarray->typesize(iarray)) {
        value = *(int *) it;
        printf("%x ", value);
    }
    printf("\n");
}


static void test_array_int() {
    array_t *iarray = array_alloc(32,sizeof(int));
    int value = 0x33;
    iarray->fill(iarray, &value);
    print_array_int(iarray);
    print_array_int_r(iarray);
    value = *(int*)iarray->data(iarray);
    printf("array_t first %x\n", value);

    int buf[] = {0x45, 0x32, 0x65, 0x223232, 0x78797};
    void *first = buf, *last = buf + 5;

    iarray->assign(iarray, first, last);
    print_array_int(iarray);
    print_array_int_r(iarray);

    value = *(int*)iarray->data(iarray);
    printf("array_t first %x\n", value);

    printf("array_t::size %d \n", iarray->size(iarray));

    iarray->free(iarray);
}





void test_array() {
    test_array_int();
}