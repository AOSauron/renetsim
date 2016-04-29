#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <gvc.h>

#include "renderer.h"


static void getNodeId(char *buff, repository_t *repo){
    //use of uintptr_t and PRIuPTR to suppress warnings
    //TODO: parametred mask ?
    sprintf(buff, "%"PRIuPTR, (uintptr_t) repo & 0x000fff);
}

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
    for (i=0; i<rnsGraph->size; i++){
        char buff[10];
        getNodeId(buff, rnsGraph->repos[i]);
        n = agnode(g, buff, 1);
        for (j=0; j<rnsGraph->repos[i]->nbrLinks; j++){
          getNodeId(buff, rnsGraph->repos[i]->links[j]->repo);
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
