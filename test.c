#include <stdlib.h>


#include "input.h"
#include "renderer.h"
#include "graphGeneration.h"

int main(int argc, char **argv){

    // repository_t *ra = rns_newRepository(1, 0, 0);
    // repository_t *rb = rns_newRepository(2, 0, 1);
    // repository_t *rc = rns_newRepository(0, -5, 8);
    // repository_t *rd = rns_newRepository(4, 40, 8);
    //
    // rnsGraph_t *graph = rns_newGraph(10);
    //
    // rns_addRepository(graph, ra, NULL, RNS_FALSE);
    // rns_addRepository(graph, rb, NULL, RNS_FALSE);
    // rns_addRepository(graph, rc, NULL, RNS_FALSE);
    // rns_addRepository(graph, rd, NULL, RNS_FALSE);
    //
    // rns_directedEdge(ra, rb, 1);
    // rns_directedEdge(rb, ra, 1);
    // rns_directedEdge(rb, rc, 5);
    // rns_directedEdge(rd, rc, 5);

    rendering_ctx_t *c2;
    rendering_ctx_t *c1;
    struct json_object *o;

    o = parse_file_json("input/input.json");
    c1  = graph_from_json(o);
    render_graph(c1, "output/result-json.jpg", "jpg");

    c2 = graph_from_rns_file("input/input.rns");
    render_graph(c2, "output/result-rns.jpg", "jpg");

    rendering_ctx_t *ctx = new_rctx();
    rnsGraph_t *graph = randomGeneration(5);

    ctx->directed = RNS_FALSE;
    ctx->graph = graph;

    render_graph(ctx, "output/result-random.jpg", "jpg");
    return 0;
}
