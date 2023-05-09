
#ifndef  __LINE_MATRIX__H__

#define  __LINE_MATRIX__H__


#ifdef __cplusplus
extern "C"{
#endif


struct matrix;


struct matrix* matrix_new(int rows, int cols, float *data);

struct matrix* matrix_ide(int rows) ;

int            matrix_add(struct matrix* matA, struct matrix* matB, struct matrix* matC);

int            matrix_sub(struct matrix* matA, struct matrix* matB, struct matrix* matC);

int            matrix_mul(struct matrix* matA, struct matrix* matB, struct matrix* matC);

int            matrix_inv(struct matrix* matA, struct matrix* matB);

int            matrix_tra(struct matrix* matA, struct matrix* matB);

int            matrix_del(struct matrix* matA);

void           matrix_trace(struct matrix *mat);

#ifdef __cplusplus
}
#endif

#endif // __LINE_MATRIX__H__