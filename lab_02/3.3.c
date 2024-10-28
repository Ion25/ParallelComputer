#include <stdio.h>
#include <mpi.h>

int main(void) {
    int comm_sz, rank, local_value, partner;
    int global_sum = 0;  // Variable para almacenar la suma global.

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Asignar valor local: rank + 1 */
    local_value = rank + 1;

    /* Ajustar la cantidad de procesos a la potencia de 2 más cercana */
    int active_procs = comm_sz;
    while (active_procs % 2 != 0) {
        active_procs--;  // Reducción al número más cercano que sea potencia de 2.
        if (rank == active_procs) {
            // Procesos sobrantes envían su valor al proceso 0.
            MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    /* Realizar la suma en árbol binario */
    int step = 1;
    while (step < active_procs) {
        if (rank < active_procs && rank % (2 * step) == 0) {
            // Proceso recibe valor del compañero y lo suma a su valor local.
            partner = rank + step;
            int recv_value;
            MPI_Recv(&recv_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += recv_value;
        } else if (rank < active_procs) {
            // Proceso envía su valor al compañero y sale del bucle.
            partner = rank - step;
            MPI_Send(&local_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    /* El proceso 0 recoge los valores de los procesos adicionales */
    if (rank == 0) {
        global_sum = local_value;  // Almacena la suma parcial del árbol.

        for (int i = active_procs; i < comm_sz; i++) {
            int extra_value;
            MPI_Recv(&extra_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += extra_value;
        }
        printf("Suma global = %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}

