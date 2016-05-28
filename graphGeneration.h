#ifndef INCLUDE_GENERATION_H
#define INCLUDE_GENERATION_H

#include "renetsim.h"


/**
  * Effectue la génération aléatoire d'un graphe
  * et renvoie un pointeur sur celui-ci
  */
rnsGraph_t *randomGeneration(size_t nbRepo, int threshold,
                             bool_t directed, bool_t connected);

#endif
