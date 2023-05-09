

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glm.hpp>
#include <gtc/matrix_inverse.hpp>

#include "matrix.h"

//glm 列优先

int main (int argc, char *argv[]) {
	glm::mat2x3 mat23 = {{1.0,-2.0,-5.0},{2.0,5.0,6.0}};
	glm::mat3x2 mat32 = {{1.0,2.0},{-2.0,5.0},{-5.0,6.0}};
	glm::vec3   mat31 = {7.0,4.0,-3.0};

	glm::mat2x2 mat22 = {{0.0,0.0},{0.0,0.0}}, imat22 = {{0.0,0.0},{0.0,0.0}};
	glm::vec2   mat21 = {0.0,0.0}, imat21 = {0.0,0.0};

	//mat22 = mat23 * mat32;
	mat22 = mat32 * mat23;
	//mat21 = mat23 * mat31;
	mat21 = mat31 * mat23;

	printf(" %f %f %f \n %f  %f %f \n", mat23[0][0], mat23[0][1], mat23[0][2], mat23[1][0], mat23[1][1], mat23[1][2]);
	printf(" %f %f \n %f %f \n", mat22[0][0], mat22[0][1], mat22[1][0], mat22[1][1]);

	matrix_mul(&mat23[0][0], &mat32[0][0], &imat22[0][0], 2, 3, 2);
	printf(" %f %f \n %f %f \n", imat22[0][0], imat22[0][1], imat22[1][0], imat22[1][1]);
	//printf(" %f %f %f \n %f  %f %f \n", mat23[0][0], mat23[0][1], mat23[0][2], mat23[1][0], mat23[1][1], mat23[1][2]);



	printf(" %f %f \n", mat21[0], mat21[1]);

    imat22 = glm::inverseTranspose(mat22);

    //imat21 = imat22 * mat21;
    imat21 = mat21 * imat22;

    printf("%f %f \n", imat21[0], imat21[1]);

    return 0;
}