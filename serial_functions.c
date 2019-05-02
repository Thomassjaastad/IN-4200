#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "serial_data.h"

void allocate_image(image *u, int m, int n)
/*
--------------------------------------------
Produces a 2d array
can now be used as u[m,n]
--------------------------------------------
*/
{
    u->image_data = malloc(m * sizeof *u->image_data);
    u->image_data[0] = malloc(m*n * sizeof *u->image_data[0]);
    for (int i = 1; i < m; i ++)
	{
        u->image_data[i] = &(u->image_data[0][i*n]);
    }
	printf("An array is made!\n");

}

void deallocate_image(image *u)
/*
--------------------------------------
Frees up memory for a 2d-array.
--------------------------------------
*/
{
  	free(u->image_data[0]);
  	free(u->image_data);
}


void convert_jpeg_to_image(const unsigned char* image_chars, image *u)
/*
----------------------------------------
u->m gives the row vector for image
u->n gives the col vector for image
image_chars contain pixels of image imported from JPEG lib
----------------------------------------
*/
{
	int i, j;
	for (i = 0; i < u->m; i++)
	{
		for (j = 0; j < u->n; j++)
		{
			u->image_data[i][j] = (float)image_chars[i*u->n + j];
		}
	}
}

void convert_image_to_jpeg(const image *u, unsigned char* image_chars)
{
	int i, j;
	for (i = 0; i < u->m; i++)
	{
		for (j = 0; j < u->n; j++)
		{
			image_chars[i*u->n + j] = (unsigned char)u->image_data[i][j];
		}
	}
}

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters)
{
  float** tmp;
	int i, j, t;

  for (i = 0; i < u->m; i++)
  {
    for (j = 0; j < u->n; j++)
    {
      if(i == 0)
      {
        //printf("First if statement %d %d \n", i, j);
          u_bar->image_data[i][j] = u->image_data[i][j];
      }
      else if(j == 0)
      {
        //printf("Second if statement %d %d \n", i, j);
        u_bar->image_data[i][j] = u->image_data[i][j];
      }
      else if(i == u->m)
      {
        //printf("Third if statement %d %d \n", i, j);
          u_bar->image_data[i][j] = u->image_data[i][j];
      }
      else if(j == u->n)
      {
        //printf("Fourth if statement %d %d \n", i, j);
          u_bar->image_data[i][j] = u->image_data[i][j];
      }
    }
  }

	for (t = 0; t < iters; t++)
	{
		for (i = 0; i < u->m; i++)
		{
			for (j = 0; j < u->n; j++)
			{
			  //printf("Fifth if statement and main calc %d %d\n", i, j);
				u_bar->image_data[i][j] = u->image_data[i][j] + kappa*(u->image_data[i-1][j] + u->image_data[i][j-1] - 4* u->image_data[i][j] + u->image_data[i][j+1] + u->image_data[i+1][j]);
			}
		}
	if (t < iters - 1)
	{
		printf("swap objects t = %d\n", t);
		//memcpy(*u->image_data, *u_bar->image_data, u->m * u->n * sizeof * (*u_bar->image_data));
    tmp = u_bar;
    u_bar = u;
    u = tmp;

	}
	else
	{
    //memcpy(*u_bar->image_data, *u->image_data, u->m * u->n * sizeof * (*u_->image_data));
		printf("if statement dont change u and u_bar %d\n", t);

	}
	//printf("iters is %d: \n", t);
	}
}
