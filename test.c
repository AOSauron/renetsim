#include <stdlib.h>

#include "renetsim.h"

int main(int argc, char **argv){

    repository_t *ra = rns_newRepository(1, 0, 0);
    repository_t *rb = rns_newRepository(2, 0, 1);
    repository_t *rc = rns_newRepository(0, -5, 8);
    repository_t *rd = rns_newRepository(4, 40, 8);

    rnsGaph_t *graph = rns_newGraph(10);

    rns_addRepository(graph, ra);
    rns_addRepository(graph, rb);
    rns_addRepository(graph, rc);
    rns_addRepository(graph, rd);

    rns_directedEge(ra, rb, 1);
    rns_directedEge(rb, ra, 1);
    rns_directedEge(rb, rc, 5);
    rns_directedEge(rd, rc, 5);


    return render_graph(graph, "result.jpg", "jpg");
}
