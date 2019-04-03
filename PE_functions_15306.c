#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "PE_data.h"

void read_graph_from_file(char *filename, txtfile *data)
{
  /*Parameters:
 |---------------------------------------------------------------------------------------|
 | name of text file, must be in same directory as PE_functions_xxxx.c                   |     
 | example* data are inNodes and outNodes                                                |         
 |---------------------------------------------------------------------------------------|
  reads a .txt file consisting of two columns of ints two times from current directory
  rows, col_idx and vals array constitute the CRS formated matrix.
  The first read counts number times a number appears in each column. Row_ptr and 
  dangling webpages are made. Second read makes col_idx keeping track of outNodes and vals
   which is a measure of each webpages "importance".   
  */    

    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Could not open file %s\n", filename);
        exit(0);
    }
    // initialize file parameters and allocate memory for inbound of outbound values
    char buffer[512], buffer1[512];
    int i, j;  
    int *connection_rows;
    int *connection_count;
    int newline = 0;
    int from, to;
    // read file, skip each line where # appears expect for line where nodes: and edges: are listed. 
    while (fgets(buffer, 512, fptr)){
        if (buffer[0] == '#')
        {
            ++newline;
            if (newline == 3) // line where number of edges and nodes are available
            {
                // getting number of lines and edges which is needed for
                // data->inNodes and data->outNodes file
                sscanf(buffer, "%*s %*s %d %*s %d", &data->max_val, &data->n);
                //data->inNodes = (int *)malloc(data->n * sizeof(int));
                //data->outNodes = (int *)malloc(data->n * sizeof(int));
                data->row_ptr = (int *)calloc((data->max_val + 1), sizeof(int));
                connection_count = (int *)calloc(data->max_val, sizeof(int));
                connection_rows = (int *)calloc(data->max_val, sizeof(int));
            }
            continue;
        }
        else
        {
            // values from column 1 and 2 from text file are stored in inNode and outNode 
            sscanf(buffer, "%d %d", &from, &to);
            // skipping self-links (same outbound as inbound)
            if (from == to)
            {
                continue;
            }
            // count number of connections between nodes i.e the number of from occurs 
            else
            {
                connection_count[from] ++;
                connection_rows[to] ++;
            }
            //printf("%d %d\n", from[k], to[k]);
        }        
    }
    data->n_dangling = 0;
    for (i = 0; i < data->max_val; i++)
    {
        if (connection_count[i] == 0)
        {
            data->n_dangling ++;
        }
        data->row_ptr[i + 1] = data->row_ptr[i] + connection_rows[i];
    }
    printf("\n");
    printf("the number of dangling webpages are %d\n", data->n_dangling);
    data->dangle_idx = (int *)malloc(data->n_dangling * sizeof(int));
    data->dangle_vals = (double *)malloc(data->n_dangling * sizeof(double));
    int idx = 0;
    for (i = 0; i < data->max_val; i++)
    {

        if (connection_count[i] == 0)
        {
            data->dangle_idx[idx] = i;
            idx ++;
        }
        //printf("connection count %d is counted %d times \n ", i, connection_count[i]);
    }
    
    //for (i = 0; i < (data->max_val+1); i++)
    //{
    //    printf("%d ", data->row_ptr[i]);
    //} 
    //for (i = 0; i < data->max_val ;i++)
    //{
    //    printf("%d %d\n", connection_count[i], connection_rows[i]);
    //}
    printf("end of first read\n");
    rewind(fptr);
    int col_counter = 0;
    data->vals = (double *)malloc(data->n * sizeof(double));
    data->col_idx = (int *)malloc(data->n * sizeof(int));
    int *count;
    count = (int *)calloc(data->max_val, sizeof(int));

    int dummy = 0; 
    int idx_dangle = 0;
    while (fgets(buffer1, 512, fptr))
    {
        if (buffer1[0] != '#')
        {
            sscanf(buffer1, "%d %d", &from, &to);
            if (from == to)
            {
                continue;
            }
            else
            {
                // counter nakes sure data->col_idx[idx] does not equal from
                // i.e the values are not replaced as to elements occur several times
                int idx = data->row_ptr[to] + count[to];
				data->col_idx[idx] = from;
                data->vals[idx] = 1.0/connection_count[from];
                count[to] ++;
            }
            
        }
    }
    
    /*
    |-----------------------------------------------------------------------------------|
    | Printing out:                                                                     |        
    | 1. dangling webpages                                                              |
    | 2. row pointer                                                                    |        
    | 3. column indices                                                                 |        
    | 4. values                                                                         |        
    |-----------------------------------------------------------------------------------|
    */ 
    /*
    printf("dangle_idx looks like this -> [ ");
    for (i = 0; i < data->n_dangling; i++)
    {
        printf("%d", data->dangle_idx[i]);
        printf(" ");
    }
    printf("]\n");
    
    printf("Row pointer values:\n");
    for (i = 0; i <= data->max_val; i++)
    {
        printf("%d ", data->row_ptr[i]);
    }
    printf("\n");
    printf("Column index values:\n");
    for (i = 0; i < data->n; i++)
    {
        printf("%d ", data->col_idx[i]);
    }
    
    printf("\n");
    printf("Values:\n");
    for (i = 0; i < data->n; i++)
    {
        printf("%.1f ", data->vals[i]);
    }
    printf("\n");
    */

    // closing statements
    printf("\nread file done\n");
    fclose(fptr);
}

void PageRank_iterations(txtfile *data, double eps, double d)
{
  /*Parameters:
 |--------------------------------------------------------------------------------------|
 | N is length of row vector                                                            |
 | d is the damping constant                                                            |
 | err is the error for stopping criteria                                               |
 | data->n_dangling is the number of dangling webpages                                                 |                                                                              
 |--------------------------------------------------------------------------------------|
 Implements the Pagerank algorithm with an intial guess x_0
 iterates until a the difference between x[k] and x[k-1] is smaller than some eps
 */

    int i ,j;
    double *x;
    int *ones;
    double N_const = 1.0/data->max_val;
    double diff;
    
    x = (double*)calloc(data->max_val, sizeof(double));  
    ones = (int*)calloc(data->max_val, sizeof(int));

    // making intial array x0 
    for (i = 0; i < data->max_val; i++)
    {
        x[i] = N_const; 
        ones[i] = 1;
    }
    // initializing constants and arrays for storing pagerank values
    double *x_new;
    x_new = (double*)calloc(data->max_val, sizeof(double));
    data->final_score_vec = (double*)calloc(data->max_val, sizeof(double));

    int t = 0;
    int iterations = 200;
    double W = 0;

    while(t < iterations)
    {
        //printf("W before loop %f:\n", W);
        for (i = 0; i < data->n_dangling; i++)
        {
            W += x[data->dangle_idx[i]];
        }
        //printf("\n W %f\n", W);
        double max_diff = 0.0;
        double term =(1 - d + d*W)*N_const;
        //printf("damp is: %f, N_const is: %f, term is: %f and W: %f\n", d, N_const, term, W);
        //printf("------------------------------------------------------------\n");
        //printf("------------------Iteration %d-------------------------------\n", t);
        //printf("------------------------------------------------------------\n");

        for (i = 0; i < data->max_val; i++)
        {
            x_new[i] = term*ones[i];
            for (j = data->row_ptr[i]; j < data->row_ptr[i + 1]; j++)
            {
                x_new[i] += d*data->vals[j]*x[data->col_idx[j]];
            }
            diff = fabs(x_new[i] - x[i]);
            if (diff > max_diff)
            {
                max_diff = diff;
            }
        }
        if (max_diff < eps)
        {
            printf("\n###########  Convergence occured at iteration %d:  ###########\n", t);
            break;
        }
        //printf("------------------------------------------------------------\n\n");
        //printf("     ###### max_diff for Iteration %d is %f ######\n", t, max_diff);
        //printf("------------------------------------------------------------\n");
        //printf("------------------------------------------------------------\n\n");
        //printf("%f %f \n", x_new[0], x[0]);
        memcpy(x, x_new, data->max_val * sizeof * x_new);
        W = 0;
        t++;
    }
    // copying converged array
    memcpy(data->final_score_vec, x_new, data->max_val * sizeof x_new);
    printf("\nPageRank done\n");
    free(x);
    free(ones);
    free(x_new);
}

void top_n_webpages(txtfile * data, int top_pages)
  /*Parameters:
 |--------------------------------------------------------------------------------------|
 |   top pages are the number pages wanting to find in the final score vector           |
 |--------------------------------------------------------------------------------------|
 Goes through array and sorts each values if a element i is smaller than element i+1
 Makes an array keeping track of indices which the corresponding webpage
 Prints out number of from highest to lowest score and which webpage the score belongs to 
 */

{
    int *page_idx = (int *)calloc(data->max_val, sizeof(int));
    int swapped, temp_idx;
    double temp;
    
    printf("Want to find top %d web pages and what web pages\n", top_pages);
    printf("---------------------------------------------------------------\n");
    printf("            ######### Final score vector: #########\n");
    for (int i = 0; i < data->max_val; i++)
    {
        //printf("%.5f ", data->final_score_vec[i]);
        page_idx[i] = i;
    }
    printf("\n");
    printf("---------------------------------------------------------------\n");
    
    while(1)
    { 
        swapped = 0;
        //Go N - 1 as the Nth term cannot compare to anything  
        for (int i = 0; i < data->max_val - 1; i++)
        {
            if (data->final_score_vec[i] < data->final_score_vec[i + 1])
            {
                temp = data->final_score_vec[i];
                data->final_score_vec[i] = data->final_score_vec[i + 1];
                data->final_score_vec[i + 1] = temp;
                temp_idx = page_idx[i];
                page_idx[i] = page_idx[i + 1];
                page_idx[i + 1] = temp_idx;
                swapped = 1;
            }
        }
        if (swapped == 0)
        {
            break;
        } 
    }

    printf("---------------------------------------------------------------\n");
    printf("       ######### Top %d web pages have score and number: #########\n", top_pages);
    printf("                Score:              Webpage:\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < top_pages; i++)
    {
        printf("                %.5f             %d\n", data->final_score_vec[i], page_idx[i]);

    }
    printf("---------------------------------------------------------------\n");
    printf("---------------------------------------------------------------\n");
    printf("---------------####### END OF PROGRAM ########-----------------\n");
    free(page_idx);
}
