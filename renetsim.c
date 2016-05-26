#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "renetsim.h"


rnsGraph_t *rns_newGraph(size_t maxsize){
    rnsGraph_t *graph;

    graph = (rnsGraph_t*) malloc(sizeof(rnsGraph_t));
    graph->repos = (repository_t **)calloc(maxsize, sizeof(repository_t*));
    graph->maxsize = maxsize;
    graph->size = 0;

    return graph;
}

void rns_freeGraph(rnsGraph_t *graph){
    size_t i;

    for (i=0; i<graph->maxsize; i++){
          rns_freeRepository(graph->repos[i]);
    }

    free(graph);
}

repository_t *rns_newRepository(size_t maxLinks, int x, int y){
    repository_t *repo;

    repo = (repository_t *) malloc(sizeof(repository_t));
    repo->links = (link_t **) calloc(maxLinks, sizeof(link_t*));
    repo->maxLinks = maxLinks;
    repo->nbrLinks = 0;
    repo->position.x = x;
    repo->position.y = y;

    return repo;
}

void rns_freeRepository(repository_t *repo){
      size_t i;

      for (i=0; i<repo->nbrLinks; i++){
            free(repo->links[i]);
      }
      free(repo);
}

link_t *rns_newLink(repository_t *repo, int weight){
    link_t *link = malloc(sizeof(link_t));
    link->repo = repo;
    link->weight = weight;
    link->is_path = _FALSE;

    return link;
}

int rns_addRepository(rnsGraph_t *rnsGraph, repository_t *repo,
                      const char *id, bool_t check_integrity)
{
    size_t i;

    if (!rnsGraph || !repo || rnsGraph->size >= rnsGraph->maxsize){
        return FAILURE;
    }

    if (check_integrity){
      for (i=0; i<rnsGraph->size; i++){
        if (rnsGraph->repos[i] &&
            rnsGraph->repos[i]->position.x == repo->position.x &&
            rnsGraph->repos[i]->position.y == repo->position.y)
            {
              return FAILURE;
            }
      }
    }

    if (id){
        sprintf(repo->id, "%s", id);
    }else{
        sprintf(repo->id, "%d", rnsGraph->size);
    }

    rnsGraph->repos[rnsGraph->size++] = repo;

    return SUCCESS;
}

int rns_addLink(repository_t *repo, link_t *link){
  if (repo && repo->nbrLinks < repo->maxLinks && link && link->repo != repo){
      repo->links[repo->nbrLinks] = link;
      return repo->nbrLinks++;
  }
  return FAILURE;
}


int rns_directedEdge(repository_t *from,
                     repository_t *dest,
                     int weight)
{
    return rns_addLink(from, rns_newLink(dest, weight));
}

repository_t *repository_by_id(rnsGraph_t *graph, const char *id){
    size_t i;

    for (i=0; i<graph->size; i++){
          if (strcmp(graph->repos[i]->id, id) == 0){
              return graph->repos[i];
          }
    }

    return NULL;
}

unsigned long distance(position_t a, position_t b){
    return sqrt( pow(b.x - a.x, 2) +
           pow(b.y - a.y, 2));
}

unsigned long cost(repository_t *repo, link_t *link){
    return distance(repo->position, link->repo->position) * link->weight;
}
