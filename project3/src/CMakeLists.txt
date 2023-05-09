
AUX_SOURCE_DIRECTORY(. DIR_MATRIX_SRCS)
include_directories(../include)
ADD_LIBRARY(Matrix ${DIR_MATRIX_SRCS})