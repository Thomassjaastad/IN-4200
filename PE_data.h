typedef struct {

    int n;
    int max_val;
    int *inNodes;
    int *outNodes;

}example;
// example.txt needs to be changed when using real file
void read_graph_from_file(char *filename, example *data);
