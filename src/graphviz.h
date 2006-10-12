#include <stdbool.h>

void start_graph (FILE *fout);
void output_node (int id, char const *label, FILE *fout);
void output_edge (int source, int destination, char const *label,
		  char const *style, FILE *fout);
void finish_graph (FILE *fout);
