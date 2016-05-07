#ifndef INCLUDE_RENDERER_H
#define INCLUDE_RENDERER_H

#include "renetsim.h"

typedef struct {
    rnsGraph_t *graph;
    bool_t directed;
} rendering_ctx_t;

rendering_ctx_t *new_rctx(void);

int render_graph(rendering_ctx_t *rctx,
                const char* filename,
                const char* format);

#endif
