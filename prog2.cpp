#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_sum_function(void* inputBuffer, void* outputBuffer, int* len, MPI_Datatype* datatype){
    int* input = (int*)inputBuffer;
    int* output = (int*)outputBuffer;

    for(int i = 0; i < *len; i++){
        output[i] += input[i];
    }
}

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine root's rank
    int root_rank = 0;

    // Get my rank
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Create the operation handle
    MPI_Op operation;
    MPI_Op_create(&my_sum_function, 1, &operation);

    // Initialise the data to send
    int data[2] = { my_rank, my_rank + size };

    // Each MPI process sends its rank to reduction, root MPI process collects the result
    int reduction_results[2] = { 0, 0 };
    MPI_Reduce(data, reduction_results, 2, MPI_INT, operation, root_rank, MPI_COMM_WORLD);

    if(my_rank == root_rank)    {
        printf("The sum of first elements of data is %d.\n", reduction_results[0]);
        printf("The sum of second elements of data is %d.\n", reduction_results[1]);
    }

    // Free the operation handle created
    MPI_Op_free(&operation);

    MPI_Finalize();

    return EXIT_SUCCESS;
}