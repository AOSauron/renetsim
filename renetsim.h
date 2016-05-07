#ifndef INCLUDE_RENETSIM_H
#define INCLUDE_RENETSIM_H

#define RNS_FAILURE -1
#define RNS_SUCCESS 0

typedef enum {
  RNS_FALSE,
  RNS_TRUE
} bool_t;

typedef struct repository_s repository_t;

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
  repository_t  *repo;
  unsigned int weight;
} link_t;

struct repository_s {
    link_t **links;
    position_t position;
    unsigned int nbrLinks;
    unsigned int maxLinks;
    char id[10];
};

typedef struct {
    repository_t **repos;
    unsigned int size;
    unsigned int maxsize;
} rnsGraph_t;


rnsGraph_t *rns_newGraph(int maxsize);

repository_t *rns_newRepository(int maxLinks, int x, int y);
int rns_addRepository(rnsGraph_t *rnsGraph, repository_t *repo,
                        const char *id, bool_t check_integrity);

link_t *rns_newLink(repository_t *ra, unsigned int weight);
int rns_addLink(repository_t *repo, link_t *link);
int rns_directedEdge(repository_t *from, repository_t *dest, unsigned int weight);
repository_t *repository_by_id(rnsGraph_t *graph, const char *id);

#endif
