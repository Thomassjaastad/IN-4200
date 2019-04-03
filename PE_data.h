typedef struct {

    int n;
    int max_val;
    int n_dangling;
    //int *inNodes;
    //int *outNodes;
    int *row_ptr;
    int *col_idx;
    double *vals;
    double *final_score_vec;
    int *dangle_idx;
    double *dangle_vals;
    
}txtfile;

void read_graph_from_file(char *filename, txtfile *data);
void PageRank_iterations(txtfile *data, double eps, double d);
void top_n_webpages(txtfile * data, int top_n_webpages);