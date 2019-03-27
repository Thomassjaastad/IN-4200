#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "PE_data.h"

int main(int narg, char **argv)
{
    example *data;

    if (narg < 2) {
        printf("Filename required.\n");
        exit(0);
    }   

    data = (example *)malloc(sizeof(example));
    printf("main\n");
    read_graph_from_file(argv[1], data);


    printf("main before free argument\n");


    for (int i = 0; i < data->n; i++)
    {
        printf("%d", i);
        printf("%d %d\n", data->inNodes[i], data->outNodes[i]);
    }
    
    free(data->outNodes);
    free(data->inNodes);
    free(data);

}
