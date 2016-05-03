#include <stdlib.h>
#include <stdio.h>

#include "renetsim.h"


rnsGaph_t *rns_newGraph(int maxsize){
    rnsGaph_t *graph;

    graph = (rnsGaph_t*) malloc(sizeof(rnsGaph_t));
    graph->repos = (repository_t **)calloc(maxsize, sizeof(repository_t*));
    graph->maxsize = maxsize;
    graph->size = 0;

    return graph;
}

int rns_addRepository(rnsGaph_t *rnsGraph, repository_t *repo){
    int i;

    if (rnsGraph->size == rnsGraph->maxsize){
      return RNS_FAILURE;
    }

    for (i=0; i<rnsGraph->size; i++){
      if (rnsGraph->repos[i]->position.x == repo->position.x &&
          rnsGraph->repos[i]->position.y == repo->position.y)
          {
            return RNS_FAILURE;
          }
    }
    sprintf(repo->id, "%u", rnsGraph->size);
    rnsGraph->repos[rnsGraph->size++] = repo;

    return RNS_SUCESS;
}

repository_t *rns_newRepository(int maxLinks, int x, int y){
    repository_t *repo;

    repo = (repository_t *) malloc(sizeof(repository_t*));
    repo->links = (link_t **) calloc(maxLinks, sizeof(link_t*));
    repo->maxLinks = maxLinks;
    repo->nbrLinks = 0;
    repo->position.x = x;
    repo->position.y = y;

    return repo;
}

int rns_addLink(repository_t *repo, link_t *link){
    if (repo->nbrLinks != repo->maxLinks && link->repo != repo){
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

int rns_directedEge(repository_t *from,
                    repository_t *dest,
                    unsigned int weight)
{
    return rns_addLink(from, rns_newLink(dest, weight));
}
