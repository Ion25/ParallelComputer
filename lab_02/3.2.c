#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(void) {
    int comm_sz, rank;
    long long int total_tosses, tosses_per_proc, 
                local_hits = 0, global_hits = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Tosses (lanzamientos): ");
        scanf("%lld", &total_tosses);
    }

    /* Enviar total_tosses a los demás procesos */
    MPI_Bcast(&total_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    /* Determinar numero de lanzamientos por proceso */
    tosses_per_proc = total_tosses / comm_sz;

    /* Inicializar semilla */
    srand(time(NULL) + rank);

    /* Imcremento de local_hits  */
    for (long long int toss = 0; toss < tosses_per_proc; toss++) {
        /* Generar en [-1, 1] */
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1.0) {
            local_hits++;
        }
    }

    /* Sumar y enviar los resultados al proceso 0 */
    MPI_Reduce(&local_hits, &global_hits, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Calculo de la estimacion de Pi */
    if (rank == 0) {
        double pi_estimate = 4.0 * (double)global_hits / (double)total_tosses;
        printf("Valor de Pi estimado: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}

