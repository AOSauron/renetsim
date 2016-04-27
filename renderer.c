#include <stdio.h>
#include <stdlib.h>
#include <gvc.h>

#include "renderer.h"


int render_graph(rnsGaph_t *rnsGraph,
                const char* filename,
                const char* format)
{
    Agraph_t *g;
    Agnode_t *n;
    Agnode_t *d;
    GVC_t *gvc;

    FILE *fp;
    fp = fopen(filename, "w+");

    gvc = gvContext();

    g = agopen("g", Agstrictdirected, 0);

    int i, j;
    for (i=0; i<rnsGraph->maxsize; i++){
        char buff[20];
        sprintf(buff, "%p", rnsGraph->repos[i]);
        n = agnode(g, buff, 1);
        for (j=0; j<rnsGraph->repos[i]->nbrLinks ; j++){
          sprintf(buff, "%p", rnsGraph->repos[i]->links[j]->repo);
          d = agnode(g, buff, 1);
          agedge(g, n, d, 0, 1);
        }
    }

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, format, fp);
    gvFreeLayout(gvc, g);

    agclose(g);
    fclose(fp);

    return (gvFreeContext(gvc));
}
