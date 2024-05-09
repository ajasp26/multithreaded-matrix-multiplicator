// file_reader.c
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/**
 * Reads a matrix from an open file into the provided matrix structure.
 * Assumes the file's pointer is at the beginning of the matrix data and
 * that the matrix structure has already been initialized with the correct
 * dimensions.
 *
 * @param file Pointer to the open file from which to read the matrix.
 * @param matrix Pointer to the matrix structure where data will be stored.
 */
void read_matrix(FILE *file, Matrix *matrix)
{
    for (int i = 0; i < matrix->rows * matrix->cols; i++)
    {
        fscanf(file, "%lf", &matrix->data[i]);
    }
}

/**
 * Initializes two matrices from a given file. The file is expected to
 * start with the dimensions of the matrices followed by their respective
 * data. This function reads these dimensions and data, then allocates
 * memory for each matrix and fills them with the data from the file.
 *
 * @param filename Path to the file containing matrix dimensions and data.
 * @param A Pointer to the first matrix structure to initialize.
 * @param B Pointer to the second matrix structure to initialize.
 */
void init_matrices_from_file(char *filename, Matrix *A, Matrix *B)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Read matrix dimensions
    int m, k, n;
    fscanf(file, "%d %d %d", &m, &k, &n);

    // Initialize matrix A
    A->rows = m;
    A->cols = k;
    A->data = (double *)malloc(m * k * sizeof(double));

    if (A->data == NULL)
    {
        perror("Memory allocation for matrix A failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    read_matrix(file, A);

    // Initialize matrix B
    B->rows = k;
    B->cols = n;
    B->data = (double *)malloc(k * n * sizeof(double));

    if (B->data == NULL)
    {
        perror("Memory allocation for matrix B failed");
        free(A->data); // Free previously allocated memory to avoid leaks
        fclose(file);
        exit(EXIT_FAILURE);
    }

    read_matrix(file, B);

    fclose(file);
}