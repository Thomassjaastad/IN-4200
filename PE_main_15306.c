#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "PE_data.h"

int main(int narg, char *argv[])
{
    txtfile *data;

    double damping;
    double eps;
    int top_n_pages;
    clock_t start_PageRank, end_PageRank, start_READgraph, end_READgraph, start_top_n, end_top_n;
    double total_PageRank, total_READgraph, total_top_n;
    



    if (narg == 5) 
    {
        printf("Arguments supplied:\n");
        printf("txtfile: %s, damping constant: %s, epsilon: %s, top n pages: %s\n", argv[1], argv[2], argv[3], argv[4]);
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
        printf("Requies filename, damping const d, error threshold eps and number of pages n.\n");
        printf("-----------------------------------------------------------------------------\n");
        exit(0);
    }   
    
    // converting args to correct data type
    data = (txtfile *)malloc(sizeof(txtfile));
    damping = atof(argv[2]);
    eps = atof(argv[3]);
    top_n_pages = atoi(argv[4]);

    start_READgraph = clock(); 
    read_graph_from_file(argv[1], data);
    end_READgraph = clock();
    total_READgraph = (double)(end_READgraph - start_READgraph)/CLOCKS_PER_SEC;

    printf("\n");
    printf("###############################################################\n");
    printf("Time used on READ file is: %f\n", total_READgraph);
    printf("###############################################################\n");
    printf("\n");

    start_PageRank = clock();
    PageRank_iterations(data, eps, damping);
    end_PageRank = clock();
    total_PageRank = (double)(end_PageRank - start_PageRank)/CLOCKS_PER_SEC;

    printf("\n");
    printf("###############################################################\n");
    printf("Time used on PageRank calculations is: %f\n", total_PageRank);
    printf("###############################################################\n");
    printf("\n");

    start_top_n = clock();
    top_n_webpages(data, top_n_pages);
    end_top_n = clock();
    total_top_n = (double)(end_top_n - start_top_n)/CLOCKS_PER_SEC;

    printf("\n");
    printf("###############################################################\n");
    printf("Time used on top_n pages is: %f\n", total_top_n);
    printf("###############################################################\n");
    printf("\n");


    
    /*
    printf("number of lines is: %d\n", data->n);
    
    for (int i = 0; i < data->n; i++)
    {
        printf("%d %d\n", data->inNodes[i], data->outNodes[i]);
    }
    */
    //for (int i = 0; i <= data->max_val; i++)
    //{
    //    printf("%d ", data->row_ptr[i]);
    //}
    //for (int i = 0; i < data->n; i++)
    //{
    //    printf("%d ", data->col_idx[i]);
    //}
    //for (int i = 0;i < data->n;i++)
    //{
    //    printf("%.2f ", data->vals[i]);
    //}
    free(data->dangle_idx);
    free(data->dangle_vals);
    //free(data->outNodes);
    //free(data->inNodes);
    free(data->row_ptr);
    free(data->col_idx);
    free(data->vals);
    free(data->final_score_vec);
    free(data);
}
