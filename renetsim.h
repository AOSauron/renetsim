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
  /**
    * Structure représentant une position
    * dans un plan cartésien
    */

    int x;
    int y;
} position_t;

typedef struct {
  /**
    * Un lien garde une référence vers un dépôt
    * avec un poids associé, ainsi plusieurs "liens"
    * peuvent pointer sur un même dépôt.
    */

  repository_t  *repo;
  bool_t is_path;
  int weight;
} link_t;

struct repository_s {
  /**
    * Structure représentant un dépôt
    * Elle consiste en une gestion dynamique
    * d'un tableau de pointeurs sur liens
    */

    link_t **links;
    position_t position;
    size_t nbrLinks;
    size_t maxLinks;
    char id[32];
    void *data;
};

typedef struct {
  /**
    * Structure représentant un graphe
    * Elle consiste en une gestion dynamique
    * d'un tableau de pointeurs sur dépôts
    */

    repository_t **repos;
    size_t size;
    size_t maxsize;
} rnsGraph_t;


/**
 * Constructeur, crée un nouveau graphe en mémoire
 * et renvoi un pointeur sur celui-ci
 */
rnsGraph_t *rns_newGraph(size_t maxsize);

/**
 * Destructeur, libère la mémoire alouée au graphe
 */
void rns_freeGraph(rnsGraph_t *graph);

/**
 * Constructeur, crée un nouveau dépôt en mémoire
 * et renvoi un pointeur sur celui-ci
 */
repository_t *rns_newRepository(size_t maxLinks, int x, int y);

/**
 * Destructeur, libère la mémoire alouée au dépôt
 */
void rns_freeRepository(repository_t *repo);

/**
 * Constructeur, crée un nouveau lien en mémoire
 * et renvoi un pointeur sur celui-ci
 */
link_t *rns_newLink(repository_t *ra, int weight);

/**
 * Ajoute un dépôt à un graphe
 * Permet de vérifier si un dépôt avec les
 * même coordonnéés n'existe pas déjà dans le graphe
 */
int rns_addRepository(rnsGraph_t *rnsGraph, repository_t *repo,
                        const char *id, bool_t check_integrity);

/**
  * Ajoute un lien dans la liste d'adjacence du dépôt
  */
int rns_addLink(repository_t *repo, link_t *link);

/**
 * Fonction raccourcis, effectue une création
 * de lien puis ajoute ce lien dans la liste d'adjacence
 * du dépôt passé en paramètre
 */
int rns_directedEdge(repository_t *from, repository_t *dest, int weight);

/**
  * Retrouve un lien à partir de sont identifiant
  * par une méthode itérative
  */
repository_t *repository_by_id(rnsGraph_t *graph, const char *id);

/**
  * Calcul la distance entre deux positions
  */
unsigned long distance(position_t a, position_t b);

/**
  * Calcul le coût de parcours d'un dépôt
  * à un autre
  */
unsigned long cost(repository_t *repo, link_t *link);


#endif
