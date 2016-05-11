#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "graphGeneration.h"
#include "renetsim.h"


#define mINF INT_MIN / 2
#define pINF INT_MAX / 2


// a<b is assumed
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
          printf("Memory errror\n");
          return;
    }

    ra->maxLinks++;
    rns_addLink(ra, link);
}

rnsGraph_t *randomGeneration(int nbRepo)
{
    rnsGraph_t *graph;
    repository_t *repo;

    int i,j;
    long dist;
    double maxDist;
    double p;
    double threshold;

    position_t a = (position_t) {.x=mINF, .y=mINF};
    position_t b = (position_t) {.x=pINF, .y=pINF};

    maxDist = (double) distance(a, b);
    threshold = maxDist / 2.0;

    srand(time(NULL));
    graph = rns_newGraph(nbRepo);

    for (i=0; i<nbRepo; i++){
        repo = rns_newRepository(0, rand_bound(mINF, pINF),
                                    rand_bound(mINF, pINF));
        rns_addRepository(graph, repo, NULL, RNS_FALSE);
    }

    for (i=0; i<nbRepo; i++){
        for (j=i; j<nbRepo; j++){
            if (i!=j){
                dist = distance(graph->repos[i]->position,
                                graph->repos[j]->position);
                if (dist < threshold){
                    p = (1-dist/(double)maxDist);
                    if (prand(p)){
                        directedEdgeRealloc(graph->repos[i], graph->repos[j],
                        1);
                    }
              }
            }
        }
    }

    long mindist;
    for (i=0; i<nbRepo; i++){
        if(graph->repos[i]->nbrLinks == 0){
            mindist = maxDist;
            for (j=i; j<nbRepo; j++){
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
        }
    }
    return graph;
}
