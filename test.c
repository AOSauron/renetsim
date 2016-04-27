#include <stdlib.h>

#include "renetsim.h"

int main(int argc, char **argv){

    repository_t *ra = rns_newRepository(1, 0, 0);
    repository_t *rb = rns_newRepository(1, 0, 1);

    link_t *link = rns_newLink(ra, 1);
    link_t *l = rns_newLink(rb, 1);

    rns_addLink(ra, l);
    rns_addLink(rb, link);

    rnsGaph_t *graph = rns_newGraph(2);

    rns_addRepository(graph, ra);
    rns_addRepository(graph, rb);

    return render_graph(graph, "result.jpg", "jpg");
}
