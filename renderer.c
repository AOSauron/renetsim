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
    char buff[64];

    FILE *fp;
    fp = fopen(filename, "w+");

    gvc = gvContext();
    rnsGraph = rctx->graph;

    if (rctx->directed){
        g = agopen("g", Agstrictdirected, 0);
    }else{
        g = agopen("g", Agstrictundirected, 0);
    }

    size_t i, j;
    for (i=0; i<rnsGraph->size; i++){
        if ( !rnsGraph->repos[i]) {
            continue;
        }
        n = agnode(g, rnsGraph->repos[i]->id, 1);

        for (j=0; j<rnsGraph->repos[i]->maxLinks; j++){
          if (!rnsGraph->repos[i]->links[j]){
              continue;
          }
          d = agnode(g, rnsGraph->repos[i]->links[j]->repo->id, 1);
          e = agedge(g, n, d, 0, 1);
          sprintf(w, "%1.3g", (double) cost(rnsGraph->repos[i],
                                            rnsGraph->repos[i]->links[j]));
          agsafeset(e, "label", w, "");
          if (rnsGraph->repos[i]->links[j]->is_path){
              agsafeset(e, "color", "red", "");
          }
        }
    }

    // char *args[] = {
    //   "sfdp",
    //   "-x",
    //   "-Tpng",
    //   "-Goverlap=scale",
    //   "-GK=5.0",
    //   "-ooutput/random.png"
    // };
    // gvParseArgs (gvc, sizeof(args)/sizeof(char*), args);
    // gvLayoutJobs(gvc, g);
    // gvRenderJobs(gvc, g);

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, format, fp);
    gvFreeLayout(gvc, g);

    agclose(g);
    fclose(fp);

    return (gvFreeContext(gvc));
}
