#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "matrix.h"


typedef struct matrix {
	int    rows;
	int    cols;
	float *data;
} matrix;



struct matrix*    matrix_new(int rows, int cols, float *data) {
	struct matrix *mat = NULL;
	mat = malloc(sizeof(struct matrix));
	mat->data = malloc(rows * cols * sizeof(float));
	mat->rows = rows;
	mat->cols = cols;
	if (data != NULL) {
		memcpy(mat->data, data, rows * cols * sizeof(float));
	} else {
		memset(mat->data, 0, rows * cols * sizeof(float));
	}
	return mat;
}

struct matrix*    matrix_ide(int rows) {
	int i = 0;
	struct matrix *mat = malloc(sizeof(struct matrix));
	mat->data = malloc(rows * rows * sizeof(float));
	mat->rows = rows;
	mat->cols = rows;
	memset(mat->data, 0, rows * rows * sizeof(float));
	for (i = 0 ; i < rows ; ++i)
		mat->data[i * rows + i] = 1.0;

	return mat;
}

int            matrix_add(struct matrix* matA, struct matrix* matB, struct matrix* matC) {
	int i = 0, j = 0;
	if ((matA == NULL) || (matB == NULL) || (matC == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL) || (matC->data == NULL))
		return -2;
	if ((matA->rows != matB->rows) || (matA->rows != matC->rows))
		return -3;
	if ((matA->cols != matB->cols) || (matA->cols != matC->cols))
		return -4;

    for (i = 0 ; i < matC->rows ; ++i) {
    	for (j = 0 ; j < matC->cols ; ++j) {
            matC->data[i * matC->cols + j] = matA->data[i * matA->cols + j] + matB->data[i * matB->cols + j];
    	}
    }

	return 0;
}

int            matrix_sub(struct matrix* matA, struct matrix* matB, struct matrix* matC) {
	int i = 0, j = 0;
	if ((matA == NULL) || (matB == NULL) || (matC == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL) || (matC->data == NULL))
		return -2;
	if ((matA->rows != matB->rows) || (matA->rows != matC->rows))
		return -3;
	if ((matA->cols != matB->cols) || (matA->cols != matC->cols))
		return -4;

    for (i = 0 ; i < matC->rows ; ++i) {
    	for (j = 0 ; j < matC->cols ; ++j) {
            matC->data[i * matC->cols + j] = matA->data[i * matA->cols + j] - matB->data[i * matB->cols + j];
    	}
    }

	return 0;
}

int            matrix_mul(struct matrix* matA, struct matrix* matB, struct matrix* matC) {
	int i = 0, j = 0, k = 0, M = 0, K = 0, N = 0;// M   K        K   N         M   N
	float sum = 0.0;
	if ((matA == NULL) || (matB == NULL) || (matC == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL) || (matC->data == NULL))
		return -2;
	if ((matA->rows != matC->rows) || (matB->cols != matC->cols) || (matA->cols != matB->rows))
		return -3;
	M = matC->rows;
	K = matA->cols;
	N = matC->cols;
    for (i = 0 ; i < M ; ++i) {
    	for (j = 0 ; j < N ; ++j) {
			sum = 0.0;
			for (k = 0 ; k < K ; ++k) {
               sum += matA->data[K * i + k] * matB->data[N * k + j];
			}
            matC->data[N * i + j] = sum;
    	}
    }
	return 0;
}

static int matrix_lup_decomposition(float *a, float *l, float *u, int *p , int N) {
	int i = 0, j = 0, k = 0;
	float sum = 0, s = 0;
	for (i = 0; i < N; ++i)
		p[i] = i;

	for (i = 0; i < N; ++i) {
		l[i * N + i] = 1;
		for (j = i + 1; j < N; ++j)
			l[i * N + j] = 0;
	}

	for (i = 1; i < N; ++i) {
		for (j = 0; j < i; ++j) {
			u[i * N + j] = 0;
		}
	}

	for (i = 0; i < N; ++i) {
		sum = 0;
		for (j = i; j < N; ++j) {
			s = abs(a[j * N + i]);
			if (s > sum) {
				sum = s;
				k = j;
			}
		}

		if (sum <= 0.000000001) 
			return -1;

        j = p[i];
        p[i] = p[k];
        p[k] = j;

		for (j = 0; j < N; ++j) {
			sum = a[i * N + j];
			a[i * N + j] = a[k * N + j];
			a[k * N + j] = sum;
		}

		u[i * N + i] = a[i * N + i];
		for (j = i + 1; j < N; ++j) {
			l[j * N + i] = a[j * N + i] / u[i * N + i];
			u[i * N + j] = a[i * N + j];
		}
		for (j = i + 1; j < N; ++j) {
			for (k = i + 1; k < N; ++k) {
				a[j * N + k] -= l[j * N + i] * u[i * N + k];
			}
		}
	}

	return 1;
}

static void matrix_lup_solve(float *a, float *b, float *l, float *u, int *p, float *x, float *y, int N) {
	int i = 0, j = 0;
	double sum = 0;
	memset(y, 0, N * sizeof(float));

	for (i = 0; i < N; ++i) {
		sum = 0;
		for (j = 0; j < i; ++j)
			sum += l[i * N + j] * y[j];
		y[i] = b[p[i]] - sum;
	}

	for (i = N - 1; i >= 0; --i) {
		sum = 0;
		for (j = i + 1; j < N; ++j)
			sum += u[i * N + j] * x[j];
		x[i] = (y[i] - sum) / u[i * N + i];
	}
}

// http://t.zoukankan.com/darkchii-p-13649311.html 
static void matrix_inverse(float *a, float *b, int N) {
	int i = 0, *P = NULL;
	float *Y = NULL, *I = NULL, *L = NULL, *U = NULL;
	P = malloc(N * sizeof(int));
    Y = malloc(N * sizeof(float));
    I = malloc(N * N * sizeof(float));
    memset(I, 0, N * N * sizeof(float));
    for (i = 0; i < N; ++i) {
    	I[i * N + i] = 1;
    }
    L = malloc(N * N * sizeof(float));
    memset(L, 0, N * N * sizeof(float));
    U = malloc(N * N * sizeof(float));
    memset(U, 0, N * N * sizeof(float));

    matrix_lup_decomposition(a, L, U, P, N);

    for (i = 0; i < N; ++i) {
    	matrix_lup_solve(a, I + i * N, L, U, P, b + i * N, Y, N);
    }

    free(P);
    free(Y);
    free(I);
    free(L);
    free(U);
}

int            matrix_inv(struct matrix* matA, struct matrix* matB) {
	if ((matA == NULL) || (matB == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL))
		return -2;
	if ((matA->rows != matB->rows) || (matA->cols != matB->cols) || (matA->rows != matA->cols))
		return -3;
    matrix_inverse(matA->data, matB->data, matA->rows);
	return 0;
}

int            matrix_inv_1(struct matrix* matA, struct matrix* matB) {
	int i = 0 , j = 0, *p = NULL;
	float *y = NULL;
	struct matrix *matI = NULL, *matX = NULL, *matL= NULL, *matU = NULL;
	if ((matA == NULL) || (matB == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL))
		return -2;
	if ((matA->rows != matB->rows) || (matA->cols != matB->cols) || (matA->rows != matA->cols))
		return -3;
	matI = matrix_ide(matA->rows);
	matX = matrix_new(matA->rows, matA->cols, NULL);
	matL = matrix_new(matA->rows, matA->cols, NULL);
	matU = matrix_new(matA->rows, matA->cols, NULL);
	p = malloc(matA->rows * sizeof(int));
	y = malloc(matA->rows * sizeof(float));

	matrix_lup_decomposition(matA->data, matL->data, matU->data, p, matA->rows);

	for (i = 0 ; i < matA->rows; ++i) {
		matrix_lup_solve(matA->data, matI->data + i * matI->rows, matL->data, matU->data, p, matB->data + i * matB->rows, y, matA->rows);
	}
   
    matrix_del(matI);
    matrix_del(matX);
    matrix_del(matL);
    matrix_del(matU);
    free(p);
    free(y);
	return 0;
}

int            matrix_tra(struct matrix* matA, struct matrix* matB) {
	int i = 0, j = 0;
	if ((matA == NULL) || (matB == NULL))
		return -1;
	if ((matA->data == NULL) || (matB->data == NULL))
		return -2;
	if ((matA->rows != matB->cols) || (matA->cols != matB->rows))
		return -3;

    for (i = 0 ; i < matA->rows ; ++i) {
    	for (j = 0 ; j < matA->cols ; ++j) {
            matB->data[j * matA->rows + i] = matA->data[i * matA->cols + j];
    	}
    }

	return 0;
}

int            matrix_del(struct matrix* matA) {
	if (matA == NULL)
		return 0;
	if (matA->data != NULL) {
		free(matA->data);
		matA->data = NULL;
	}
	free(matA);
	return 0;
}

void           matrix_trace(struct matrix *mat) {
	int i = 0, j = 0;
	if (mat == NULL)
		return;
	printf("matrix rows:%d cols:%d \n", mat->rows, mat->cols);
	for (i = 0; i < mat->rows; ++i) {
		for (j = 0 ; j < mat->cols; ++j) {
			printf(" %f  ", mat->data[i * mat->cols + j]);
		}
		printf("\n");
	}

}