#include <stdlib.h>
#include <gvc.h>

#include "renderer.h"


rendering_ctx_t *new_rctx(void){
    return malloc(sizeof(rendering_ctx_t));
}

int render_graph(rendering_ctx_t *rctx,
                const char* filename,
                const char* format)
{
    Agraph_t *g;
    Agnode_t *n;
    Agnode_t *d;
    Agedge_t *e;
    GVC_t *gvc;
    rnsGraph_t *rnsGraph;
    char w[64];

    FILE *fp;
    fp = fopen(filename, "w+");

    gvc = gvContext();
    rnsGraph = rctx->graph;

    if (rctx->directed){
        g = agopen("g", Agstrictdirected, 0);
    }else{
        g = agopen("g", Agstrictundirected, 0);
    }

    int i, j;
    for (i=0; i<rnsGraph->size; i++){
        n = agnode(g, rnsGraph->repos[i]->id, 1);
        for (j=0; j<rnsGraph->repos[i]->nbrLinks; j++){
          d = agnode(g, rnsGraph->repos[i]->links[j]->repo->id, 1);
          e = agedge(g, n, d, 0, 1);
          sprintf(w, "%d", rnsGraph->repos[i]->links[j]->weight);
          agsafeset(e, "label", w, "");
        }
    }

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, format, fp);
    gvFreeLayout(gvc, g);

    agclose(g);
    fclose(fp);

    return (gvFreeContext(gvc));
}
