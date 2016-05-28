#ifndef INCLUDE_RENDERER_H
#define INCLUDE_RENDERER_H

#include "renetsim.h"

typedef struct {
  /**
    * Structure représentant un contexte
    * de rendu de graphe, selon si on veut une
    * représentation en arcs orientés ou non
    */

    rnsGraph_t *graph;
    bool_t directed;
} rendering_ctx_t;

/**
  * Constructeur, créer un nouveau contexte
  * en mémoire et renvoie un pointeur sur celui-ci
  */
rendering_ctx_t *new_rctx(void);


/**
  * Produit un rendu du graphe en fonction
  * d'un contexte et écrit dans un fichier
  * de sortie
  */
int render_graph(rendering_ctx_t *rctx,
                 const char* filename,
                 const char* format);

#endif
