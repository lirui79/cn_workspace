
#ifndef  __LINE_MATRIX__H__

#define  __LINE_MATRIX__H__

// matrixA   M X K     matrixB  K X N     matrixC   M X N

void  matrix_mul(float *matrixA, float *matrixB, float *matrixC, int M, int K, int N) {
	int i = 0,j = 0, k = 0;
	for (i = 0 ; i < M; ++i) {
		for (j = 0 ; j < N; ++j) {
			matrixC[N * i + j] = 0.0;
			for (k = 0 ; k < K; ++k) {
               matrixC[N * i + j] += matrixA[K * i + k] * matrixB[N * k + j];
			}
		}
	}

}





#endif  //__LINE_MATRIX__H__