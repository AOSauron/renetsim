#ifndef INCLUDE_PATHFINDING_H
#define INCLUDE_PATHFINDING_H

#include "renetsim.h"


void apply_dijkstra(rnsGraph_t *graph, repository_t *start,
                                       repository_t *end);

#endif
