// matrix_multiplier.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"
#include "file_reader.h"

/**
 * Computes a single element of the result matrix in a separate thread.
 * This function is designed to be used with pthread_create, allowing
 * parallel computation of matrix multiplication.
 *
 * @param arg A pointer to a ThreadArg structure containing the indices
 * of the element to compute, and pointers to the input matrices.
 * @return NULL
 */
void *compute_element(void *arg)
{
    ThreadArg *threadArg = (ThreadArg *)arg;
    double sum = 0.0;
    for (int i = 0; i < threadArg->A->cols; i++)
    {
        sum += threadArg->A->data[threadArg->row * threadArg->A->cols + i] * threadArg->B->data[i * threadArg->B->cols + threadArg->col];
    }
    *(threadArg->result) = sum;
    pthread_exit(NULL);
}

/**
 * Multiplies two matrices using multiple threads. Each thread computes
 * a single element of the result matrix, allowing for parallel computation.
 * The function initializes threads for each element, waits for all to
 * complete, and then the result is available in matrix C.
 *
 * @param A Pointer to the first matrix operand.
 * @param B Pointer to the second matrix operand.
 * @param C Pointer to the result matrix, already initialized with proper dimensions.
 */
void multiply_matrices(const Matrix *A, const Matrix *B, Matrix *C)
{
    pthread_t threads[C->rows * C->cols];
    ThreadArg args[C->rows * C->cols];

    for (int i = 0; i < C->rows; i++)
    {
        for (int j = 0; j < C->cols; j++)
        {
            int index = i * C->cols + j;
            args[index] = (ThreadArg){.row = i, .col = j, .A = A, .B = B, .result = &C->data[index]};
            if (pthread_create(&threads[index], NULL, compute_element, (void *)&args[index]))
            {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (int i = 0; i < C->rows * C->cols; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

// Main function
int main(int argc, char *argv[])
{
    // The program expects exactly one command-line argument:
    // Argument should be the path to the file containing the matrices.
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <matrix file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Matrix A, B, C;

    // Initialize matrices from the file
    init_matrices_from_file(argv[1], &A, &B);

    // Initialize matrix C
    C.rows = A.rows;
    C.cols = B.cols;
    C.data = (double *)malloc(C.rows * C.cols * sizeof(double));

    if (C.data == NULL)
    {
        perror("Memory allocation for matrix C failed");
        free(A.data); // Free previously allocated memory
        free(B.data);
        exit(EXIT_FAILURE);
    }

    // Perform matrix multiplication
    multiply_matrices(&A, &B, &C);

    // Print the resulting matrix
    for (int i = 0; i < C.rows; i++)
    {
        for (int j = 0; j < C.cols; j++)
        {
            printf("%.5f ", C.data[i * C.cols + j]);
        }
        printf("\n");
    }

    // Cleanup
    free(A.data);
    free(B.data);
    free(C.data);

    return 0;
}