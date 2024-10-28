#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {
    int comm_sz, rank, n;
    double *matrix = NULL, *vector = NULL, *local_matrix = NULL;
    double *local_result, *global_result;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Tamaño de la matriz */
    n = 4;

    int local_cols = n / comm_sz;

    /* Declarar matriz y vector */
    local_matrix = malloc(n * local_cols * sizeof(double));  // Bloque local de columnas
    local_result = malloc(n * sizeof(double));  // Vector resultado parcial (local)
    for (int i = 0; i < n; i++) {
        local_result[i] = 0.0;  // Inicializar a 0
    }

    /* El proceso 0 almacena y llena la matriz y el vector */
    if (rank == 0) {
        matrix = malloc(n * n * sizeof(double));
        vector = malloc(n * sizeof(double));

        printf("Matriz A:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i * n + j] = i + j + 1;  // Ejemplo: valores i + j + 1
                printf("%.1f ", matrix[i * n + j]);
            }
            printf("\n");
        }

        printf("\nVector x:\n");
        for (int i = 0; i < n; i++) {
            vector[i] = 1.0;
            printf("%.1f ", vector[i]);
        }
        printf("\n");
    }

    /* El proceso 0 distribuye las columnas de la matriz a los demás procesos */
    if (rank == 0) {
        for (int p = 1; p < comm_sz; p++) {
            MPI_Send(&matrix[p * local_cols], n * local_cols, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        // El proceso 0 mantiene su bloque local
        for (int i = 0; i < n * local_cols; i++) {
            local_matrix[i] = matrix[i];
        }
    } else {
        /* Los demás procesos reciben su parte de la matriz */
        MPI_Recv(local_matrix, n * local_cols, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    /* El vector completo se envía a todos los procesos */
    if (rank == 0) {
        MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        vector = malloc(n * sizeof(double));
        MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    /* Cada proceso calcula su parte del producto matriz-vector */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < local_cols; j++) {
            local_result[i] += local_matrix[i * local_cols + j] * vector[j + rank * local_cols];
        }
    }

    /* Almacenar el resultado final en el proceso 0 usando MPI_Reduce_scatter */
    global_result = malloc(n * sizeof(double));  // Espacio para el resultado final
    int recv_counts[comm_sz];  // Número de elementos que cada proceso envía/recibe
    for (int i = 0; i < comm_sz; i++) recv_counts[i] = n / comm_sz;

    MPI_Reduce_scatter(local_result, global_result, recv_counts, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    /* El proceso 0 imprime el resultado final */
    if (rank == 0) {
        printf("\nResultado de A*x:\n");
        for (int i = 0; i < n; i++) {
            printf("%.1f ", global_result[i]);
        }
        printf("\n");
    }

    free(local_matrix);
    free(local_result);
    if (rank == 0) {
        free(matrix);
        free(vector);
    }
    free(global_result);

    MPI_Finalize();
    return 0;
}

