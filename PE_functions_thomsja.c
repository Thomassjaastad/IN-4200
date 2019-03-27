#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "PE_data.h"

void read_graph_from_file(char *filename, example *data)
// reads a .txt file from current directory
// rows, col_idx and vals array constitute the CRS formated matrix

{    
    FILE *fptr;
    fptr = fopen(filename, "r");

    if (fptr == NULL){
        printf("Could not open file %s\n", filename);
        exit(0);
    }

    // initialize file parameters and allocate memory for inbound of outbound values
    char buffer[512], buffer1[512];
    int i, j;  
    //int n_lines;
    //int max_val;
    char c[100], d[100], e[100];
    data->inNodes = (int *)malloc(data->n * sizeof(int));
    data->outNodes = (int *)malloc(data->n * sizeof(int));
    //int *row;
    //int *arr_count;
    int temp = 0;
    int k = 0;
    // read file, skip each line where # appears expect for line where data->n, nodes and edges are listed. 
    while (fgets(buffer, 512, fptr)){
        if (buffer[0] == '#')
        {
            ++temp;
            if (temp == 3) // line where numbers og edges and nodes are available
            {
                // getting number of lines and edges data->inNodes file
                sscanf(buffer, "%s %s %d %s %d", c, d, &data->max_val, e, &data->n);
                //row = (int*)calloc((data->max_val + 1), sizeof(int));
                //arr_count = (int*)calloc(data->max_val, sizeof(int));
                //printf("%d max %d number of lines \n", data->max_val, data->n);
            }
            continue;
        }
        else
        {
            printf("before\n");
            sscanf(buffer, "%d %d", &data->inNodes[k], &data->outNodes[k]);
            printf("after\n");
            //printf("%d %d\n", data->inNodes[k], data->outNodes[k]);
            //for (i = data->outNodes[k]; i <= data->max_val; i++)
            //{
            //    row[i + 1] ++;
            //}

            //if (k < data->max_val)
            //{
            //    arr_count[k] = k;
            //}

            k = k + 1;
            //printf("%d \n", k);
        }        
    }
    
    printf("here\n");
    fclose(fptr);

    //printf("%d struct access\n", data->n);

    //for (i = 0; i < data->n; i++)
    //{
    //    printf("%d %d ", data->inNodes[i], data->outNodes[i]);
    //}
    //for (i = 0; i < max_val; i++)
    //{
    //    printf("%d \n", arr_count[i]);

    //} 

    //rewind(fptr);
    //int m = 0;
    //int col_counter = 0;
    //int *counted_vals;
    //int *counted_vals = malloc(max_val * sizeof * counted_vals);
    //while (fgets(buffer1, 512, fptr)){
    //    if (buffer1[0] == '#')
    //    {
    //        continue;
    //    }

    //    else
    //    {
    //        //sscanf(buffer1, "%d %d", &in2[m], &out2[m]);
    //        for (i = 0; i < max_val; i++)
    //        {
    //            if (data->inNodes[m] == arr_count[i])
    //                
    //                counted_vals[i] ++;
    //        }
    //    
    //    }
    //    m = m + 1;
    //}
    /*
    for (i = 0; i < max_val; i++)
    {
        printf("%d occurs %d times\n", i, counted_vals[i]);
    }
    */
    //double *vals = malloc(data->n * sizeof * vals);
    //int *col_idx = malloc(data->n * sizeof * col_idx);
    //int dummy = 0;    
    //for (i = 0; i < data->n; i ++)
    //{
    //    for (j = 0; j < data->n; j ++)
    //    {
    //        if (data->outNodes[j] == dummy)
    //        {
    //            col_idx[col_counter] = data->inNodes[j];
    //            vals[col_counter] = 1.0/counted_vals[data->inNodes[j]];
    //            //printf("%.2f ", vals[col_counter]);                
    //            ++col_counter;
    //        }    
    //    }
    //    ++dummy;            
    //}
    //---------
    //printing
    //---------

    //row values
    //printf("Row pointer values:\n");
    //for (i = 0; i <= (max_val); i++)
    //{
    //    printf("%d ", row[i]);
    //}
    //// col_idx
    //printf("\n");
    //printf("Column index values:\n");
    //for (i = 0; i < data->n; i++)
    //{
    //    printf("%d ", col_idx[i]);
    //}
    //printf("\n");
    //printf("Values:\n");
    //for (i = 0; i < data->n; i++)
    //{
    //    printf("%.1f ", vals[i]);
    //}
    //printf("\n");
    // closing statements
    //free(row);
    //free(col_idx);
    //free(vals);
    //free(arr_count);
    //free(counted_vals);
}

//void PageRank_iterations(int damping, int err, int row_ptr[], int col_idx[], double vals[])
//{
//    N = sizeof(col_idx)/sizeof(col_idx[0]);
//
//}

