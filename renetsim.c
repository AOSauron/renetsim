#include <stdlib.h>
#include <stdio.h>

#include "renetsim.h"

rnsGraph_t *rns_newGraph(int maxsize){
    rnsGraph_t *graph;

    graph = (rnsGraph_t*) malloc(sizeof(rnsGraph_t));
    graph->repos = (repository_t **)calloc(maxsize, sizeof(repository_t*));
    graph->maxsize = maxsize;
    graph->size = 0;

    return graph;
}

int rns_addRepository(rnsGraph_t *rnsGraph, repository_t *repo,
                      const char *id, bool_t check_integrity)
{
    int i;

    if (!rnsGraph || !repo || rnsGraph->size >= rnsGraph->maxsize){
      return RNS_FAILURE;
    }

    if (check_integrity == RNS_TRUE){
      for (i=0; i<rnsGraph->size; i++){
        if (rnsGraph->repos[i]->position.x == repo->position.x &&
            rnsGraph->repos[i]->position.y == repo->position.y)
            {
              return RNS_FAILURE;
            }
      }
    }

    if (id){
        sprintf(repo->id, "%s", id);
    }else{
        sprintf(repo->id, "%u", rnsGraph->size);
    }

    rnsGraph->repos[rnsGraph->size++] = repo;

    return RNS_SUCCESS;
}

repository_t *rns_newRepository(int maxLinks, int x, int y){
    repository_t *repo;

    repo = (repository_t *) malloc(sizeof(repository_t));
    repo->links = (link_t **) calloc(maxLinks, sizeof(link_t*));
    repo->maxLinks = maxLinks;
    repo->nbrLinks = 0;
    repo->position.x = x;
    repo->position.y = y;

    return repo;
}

int rns_addLink(repository_t *repo, link_t *link){
    if (repo && repo->nbrLinks < repo->maxLinks && link && link->repo != repo){
      repo->links[repo->nbrLinks] = link;
      return repo->nbrLinks++;
    }
    return RNS_FAILURE;
}

link_t *rns_newLink(repository_t *repo, unsigned int weight){
    link_t *link = malloc(sizeof(link_t));
    link->repo = repo;
    link->weight = weight;

    return link;
}

int rns_directedEdge(repository_t *from,
                    repository_t *dest,
                    unsigned int weight)
{
    return rns_addLink(from, rns_newLink(dest, weight));
}

repository_t *repository_by_id(rnsGraph_t *graph, const char *id){
    int i;

    for (i=0; i<graph->size; i++){
      if (graph->repos[i]->id && strcmp(graph->repos[i]->id, id) == 0){
          return graph->repos[i];
      }
    }
}
