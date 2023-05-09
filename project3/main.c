// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


void solve1() {
  struct matrix *mat23, *mat32, *mat31, *mat22, *imat22, *mat21, *imat21;
  {
    float data[] = {1.0,-2.0,-5.0,2.0,5.0,6.0};
    mat23 = matrix_new(2, 3, data);
    mat32 = matrix_new(3, 2, NULL);
  }

  matrix_tra(mat23, mat32);//

  {
    float data[] = {7.0,4.0,-3.0};
    mat31 = matrix_new(3, 1, data);
  }

  {
    mat22  = matrix_new(2, 2, NULL);
    imat22 = matrix_new(2, 2, NULL);
    mat21  = matrix_new(2, 1, NULL);
    imat21 = matrix_new(2, 1, NULL);
  }

  matrix_mul(mat23, mat32, mat22);//

  matrix_trace(mat22);

  matrix_inv(mat22, imat22);

  matrix_trace(imat22);

  matrix_mul(mat23, mat31, mat21);

  matrix_mul(imat22, mat21, imat21);

  matrix_trace(imat21);

  matrix_del(mat23);
  matrix_del(mat32);
  matrix_del(mat31);

  matrix_del(mat22);
  matrix_del(imat22);

  matrix_del(mat21);
  matrix_del(imat21);
}


int main (int argc, char *argv[]) {
  solve1();
  return 0;
}