#define RNS_FAILURE -1

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
    int nbrLinks;
    int maxLinks;
};

typedef struct {
    repository_t **repos;
    int size;
    int maxsize;
} rnsGaph_t;


rnsGaph_t *rns_newGraph(int maxsize);

repository_t *rns_newRepository(int maxLinks, int x, int y);
int rns_addRepository(rnsGaph_t *rnsGraph, repository_t *repo);

link_t *rns_newLink(repository_t *ra, unsigned int weight);
int rns_addLink(repository_t *repo, link_t *link);
