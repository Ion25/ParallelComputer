#include <stdio.h>
#include <mpi.h>

int Find_bin(float meas, float *bin_maxes, int bin_count, float min_meas) {
    for (int b = 0; b < bin_count; b++) {
        if ((b == 0 && min_meas <= meas && meas < bin_maxes[b]) ||
            (b > 0 && bin_maxes[b - 1] <= meas && 
                    (b == bin_count - 1 || meas < bin_maxes[b]))) {
            return b;
        }
    }
    return -1;
}

int main(void) {
    int comm_sz, rank;
    int data_count = 20;
    float data[data_count];
    float min_meas = 0.3;
    float max_meas = 4.9;
    int bin_count = 5;

    float bin_width = (max_meas - min_meas) / bin_count;
    float bin_maxes[bin_count];
    int bin_counts[bin_count];
    
    /* Inicializar los máximos de cada bin */
    for (int i = 0; i < bin_count; i++) {
        bin_maxes[i] = min_meas + (i + 1) * bin_width;
    }
  
    /* Variables locales de cada proceso */
    int loc_data_ct;
    float loc_data[data_count];
    int loc_bin_cts[bin_count];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Determinar cuántos datos corresponden a cada proceso */
    loc_data_ct = data_count / comm_sz;

    /* Inicializar conteos locales */
    for (int i = 0; i < bin_count; i++) {
        loc_bin_cts[i] = 0;
    }
    
    /* Proceso 0 lee datos de entrada */
    if (rank == 0) {
      float input_data[] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 
                            3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4,
                            4.2, 4.5, 4.9, 0.9};
      for (int i = 0; i < data_count; i++) {
        data[i] = input_data[i];
      }
    }

    /* Distribuir datos entre procesos */
    MPI_Scatter(data, loc_data_ct, MPI_FLOAT, loc_data, loc_data_ct, 
                MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Incremento en los elementos de bin (frecuencias) */
    for (int i = 0; i < loc_data_ct; i++) {
        int bin = Find_bin(loc_data[i], bin_maxes, bin_count, min_meas);
        loc_bin_cts[bin]++;
    }

    /* Reducir los conteos locales en el proceso 0 */
    MPI_Reduce(loc_bin_cts, bin_counts, bin_count, MPI_INT, MPI_SUM, 
              0, MPI_COMM_WORLD);

    /* El proceso 0 imprime el histograma */
    if (rank == 0) {
        printf("Histograma:\n");
        for (int i = 0; i < bin_count; i++) {
            printf("%d \n", bin_counts[i]);
        }
    }

    MPI_Finalize();
    return 0;
}

