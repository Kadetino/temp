#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
        int rank,size;
        int *dataset, localdata, localrecv;        

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);


        localdata = rank;
        printf("[Process %d]: has local data %d\n", rank, localdata);

        MPI_Scan(&localdata,&localrecv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


        MPI_Barrier(MPI_COMM_WORLD);

        printf("[Process %d]: has received data: %d \n", rank,localrecv);

        MPI_Finalize();
        return 0;
}