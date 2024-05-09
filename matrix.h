// matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <pthread.h>

// Structure for the matrix
typedef struct
{
    int rows;
    int cols;
    double *data; // Pointer to matrix data stored in row-major order
} Matrix;

// Structure for thread arguments
typedef struct
{
    int row;
    int col;
    const Matrix *A;
    const Matrix *B;
    double *result; // Pointer to the result cell
} ThreadArg;

#endif