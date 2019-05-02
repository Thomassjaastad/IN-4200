#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __MACH__
#else 
#include <malloc.h>
#endif

#include "serial_data.h"

void import_JPEG_file (const char* filename, unsigned char** image_chars,
                       int* image_height, int* image_width,
                       int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,
                       int image_height, int image_width,
                       int num_components, int quality);


int main (int narg, char *argv[])
{   
    int m, n, c, iters, i, j, k;
    float kappa;
    image u, u_bar;
    unsigned char *image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;
    
    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */
  	
    if (narg == 5) 
    {
        printf("Arguments supplied:\n");
        printf("kappa: %s, iters: %s, input filename: %s, output filename: %s\n", argv[1], argv[2], argv[3], argv[4]);
    }
    else if( narg > 5) 
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
      
    import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
    printf("Succeeded to import! vertical pixels: %d, horizontal pixels: %d, num components: %d\n",
		m, n, c);

    u.m = m;
    u.n = n;

    allocate_image(&u, m, n);
    allocate_image (&u_bar, m, n);
    convert_jpeg_to_image(image_chars, &u);

    iso_diffusion_denoising(&u, &u_bar, kappa, iters);

    convert_image_to_jpeg(&u_bar, image_chars);

    export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);

    deallocate_image(&u);
    deallocate_image (&u_bar);
    return 0;
}
