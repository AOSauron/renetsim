#ifndef INCLUDE_RENETSIM_H
#define INCLUDE_RENETSIM_H

#include <stddef.h>

#define FAILURE -1
#define SUCCESS 0

typedef enum {
  _FALSE,
  _TRUE,
} bool_t;

typedef struct repository_s repository_t;

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
  repository_t  *repo;
  bool_t is_path;
  int weight;
} link_t;

struct repository_s {
    link_t **links;
    position_t position;
    size_t nbrLinks;
    size_t maxLinks; //is always prime
    char id[32];
    void *data;
};

typedef struct {
    repository_t **repos;
    size_t size;
    size_t maxsize;
} rnsGraph_t;


rnsGraph_t *rns_newGraph(size_t maxsize);
void rns_freeGraph(rnsGraph_t *graph);

repository_t *rns_newRepository(size_t maxLinks, int x, int y);
void rns_freeRepository(repository_t *repo);

link_t *rns_newLink(repository_t *ra, int weight);

int rns_addRepository(rnsGraph_t *rnsGraph, repository_t *repo,
                        const char *id, bool_t check_integrity);

int rns_addLink(repository_t *repo, link_t *link);

int rns_directedEdge(repository_t *from, repository_t *dest, int weight);

repository_t *repository_by_id(rnsGraph_t *graph, const char *id);

unsigned long distance(position_t a, position_t b);

unsigned long cost(repository_t *repo, link_t *link);

#endif
