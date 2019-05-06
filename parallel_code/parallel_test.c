#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int ierr, my_rank, numprocs;
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    
    printf("Hello world! I am process %d out of %d processes\n", my_rank, numprocs);
    if(my_rank == 0)
    {
        printf("my_rank is %d\n", my_rank);
    }
    ierr = MPI_Finalize();
}