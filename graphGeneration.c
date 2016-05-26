#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#include "graphGeneration.h"
#include "renetsim.h"


#define mINF INT_MIN / 2
#define pINF INT_MAX / 2


static inline
int rand_bound(int a, int b){
    return rand()%(b-a)+a;
}

static inline
bool_t prand(double p){
      return  rand() < p*((double) RAND_MAX + 1.0);
}

static inline
void directedEdgeRealloc(repository_t *ra, repository_t *rb, int weight){
    link_t *link;

    link = rns_newLink(rb, weight);
    ra->links = (link_t **)

    realloc(ra->links, (ra->nbrLinks+1)*sizeof(link_t*));

    if(ra->links == NULL){
          fprintf(stderr, "Memory errror\n");
          return;
    }

    ra->maxLinks++;
    rns_addLink(ra, link);
}

rnsGraph_t *randomGeneration(size_t nbRepo, int threshold,
                             bool_t directed, bool_t connected)
{
    rnsGraph_t *graph;
    repository_t *repo;

    size_t i,j;
    int c;
    unsigned long dist;
    double maxDist;
    double p;

    position_t a = (position_t) {.x=mINF, .y=mINF};
    position_t b = (position_t) {.x=pINF, .y=pINF};

    maxDist = (double) distance(a, b);

    srand(time(NULL));
    graph = rns_newGraph(nbRepo);

    for (i=0; i<nbRepo; i++){
        repo = rns_newRepository(0, rand_bound(mINF, pINF),
                                    rand_bound(mINF, pINF));
        rns_addRepository(graph, repo, NULL, _FALSE);
    }

    c = 0;
    for (i=0; i<nbRepo; i++){
        for (j = (connected) ? i:0; j<nbRepo && c<=threshold; j++){
            if (i!=j){
                dist = distance(graph->repos[i]->position,
                                graph->repos[j]->position);
                p = (1-dist/maxDist);
                if (prand(p)){
                    c++;
                    directedEdgeRealloc(graph->repos[i],graph->repos[j],1);
                    if (!directed){
                        directedEdgeRealloc(graph->repos[j],graph->repos[i],1);
                    }
                }
            }
        }
    }

    long mindist;
    for (i=0; i<nbRepo; i++){
        if(graph->repos[i]->nbrLinks == 0){
            mindist = maxDist;
            for (j=(connected) ? i:0; j<nbRepo; j++){
                if (i != j){
                    dist = distance(graph->repos[i]->position,
                                    graph->repos[j]->position);
                    if (dist < mindist){
                        mindist = dist;
                        repo = graph->repos[j];
                    }
                }
            }
            directedEdgeRealloc(graph->repos[i], repo, 1);
            if (!directed){
                directedEdgeRealloc(repo, graph->repos[i], 1);
            }
        }
    }
    return graph;
}
