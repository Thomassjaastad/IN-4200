#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#ifdef __MACH__
#else
#include <malloc.h>
#endif

#include "parallel_data.h"

void import_JPEG_file (const char* filename, unsigned char** image_chars,
                       int* image_height, int* image_width,
                       int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,
                       int image_height, int image_width,
                       int num_components, int quality);

int main(int argc, char *argv[])
{

    int m, n, c, iters;
    int my_m, my_n, my_rank, num_procs, count;
    int my_start_m, my_start_n, my_stop_n, my_stop_m;
    float kappa;

    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;

    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (my_rank == 0)
    {
        if (argc == 5)
        {
            printf("Arguments supplied:\n");
            printf("kappa: %s, iters: %s, input filename: %s, output filename: %s\n", argv[1], argv[2], argv[3], argv[4]);
        }
        else if( argc > 5)
        {
            printf("Too many arguments.\n");
            exit(0);
        }
        else
        {
            printf("-----------------------------------------------------------------------------\n");
            printf("Insert values as stated below, respectively:\n");
            printf("Requires kappa const, iters const, input filename and output filename \n");
            printf("-----------------------------------------------------------------------------\n");
            exit(0);
        }

        kappa = atof(argv[1]);
        iters = atoi(argv[2]);
        input_jpeg_filename = argv[3];
        output_jpeg_filename = argv[4];
    }

    u.m = m;
    u.n = n;

    if (my_rank==0)
    {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image(&whole_image, m, n);
    }
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int e, s;
    /* 2D decomposition of the m x n pixels evenly among the MPI processes */
    //s = 1 + my_rank*floor(m/num_procs);
    //e = s + (m/num_procs) - 1;
    //if (my_rank == (num_procs -1))
    //{
    //    e = m;
    //}
    //else
    //{
    //    e = s + floor(m/num_procs) - 1;
    //}
    //printf("s and e has the values %d and %d\n", s, e);
    my_m = m/num_procs;
    int remainder_m = m % num_procs;
    my_n = n;
    printf("my_m is %d\n", my_m);
    printf("remainder of m is then %d\n", remainder_m);
    //******timing program******
	  //call MPI_Barrier(MPI_COMM_WORLD, ierr)
	  //t1 = MPI_Wtime()
	  //<do work>
	  //call MPI_Barrier(MPI_COMM_WORLD, ierr)
	  //total_time = MPI_Wtime() - t1

    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);
    /* each process asks process 0 for a partitioned region */
    /* of image_chars and copy the values into u */
    /* ... */
    int *sendcounts = malloc(num_procs* sizeof(int));
    int *displs = malloc(num_procs* sizeof(int));
    int sum_send = 0;

    for (int i = 0; i < num_procs; i++)
    {
        //fill remainder in my_rank = 0
        if (i == 0)
        {
            sendcounts[i] = n*(my_m + remainder_m);
            displs[i] = sum_send;
        }
        else
        {
            sendcounts[i] = n*my_m;
            displs[i] = sum_send;

        }
        sum_send += sendcounts[i];
    }

    if (my_rank == 0)
    {
        my_image_chars = (unsigned char*)malloc(my_m*my_n*sizeof(unsigned char));
		    for (int i = 0; i < my_m*my_n; i++)
        {
			       my_image_chars[i] = image_chars[i];
        }

    }

    MPI_Scatterv(my_image_chars, sendcounts, displs, MPI_UNSIGNED_CHAR, num_procs, 0, MPI_COMM_WORLD);

    convert_jpeg_to_image(my_image_chars, &u);
    iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters);

    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    /* ... */
    if (my_rank == 0)
    {
        convert_image_to_jpeg(&whole_image, image_chars);
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        deallocate_image(&whole_image);
    }
    deallocate_image(&u);
    deallocate_image(&u_bar);
    MPI_Finalize();
    return 0;
}
