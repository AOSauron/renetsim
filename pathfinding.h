#ifndef INCLUDE_PATHFINDING_H
#define INCLUDE_PATHFINDING_H

#include "renetsim.h"


/**
  * Applique la méthode de dijsktra sur un graphe
  * Marque les liens constituants le plus court chemin
  */
void apply_dijkstra(rnsGraph_t *graph, repository_t *start,
                                       repository_t *end);

#endif
