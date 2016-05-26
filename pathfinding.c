#include <stdlib.h>
#include <stdio.h>

#include "pathfinding.h"

#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

#define SET_DATA(r,v) *((size_t *)r->data)=v;
#define GET_DATA(v,r) v = *(size_t*)(r->data);


typedef struct {
    unsigned long cost;
    bool_t visited;
    repository_t *from;
    repository_t *repo;
    link_t *link;
} path_t;

typedef struct {
    size_t index;
    bool_t err;
} min_ret_t;

static path_t *new_path(unsigned long cost, bool_t visited,
                       repository_t *from, repository_t *repo)
{
    path_t *p = (path_t *) malloc(sizeof(path_t));
    p->cost = cost;
    p->visited = visited;
    p->from = from;
    p->repo = repo;

    return p;
}

static min_ret_t get_min(size_t i, size_t size, path_t **paths)
{
    //min && not visited
    //assuming paths integrity
    bool_t c1, c2;
    min_ret_t a, b;

    if (!paths[i]->visited){
        return (min_ret_t){i, _FALSE};
    }

    c1 = LCHILD(i) < size;
    c2 = RCHILD(i) < size;

    if (c1 && !c2)
    {
        return get_min(LCHILD(i), size, paths);
    }

    if (c2 && !c1)
    {
        return get_min(RCHILD(i), size, paths);
    }

    if (c2 && c1)
    {
        a = get_min(LCHILD(i), size, paths);
        b = get_min(RCHILD(i), size, paths);

        if(!a.err && b.err){ return a; }
        if(a.err && !b.err){ return b; }
        if(!a.err && !b.err) {
            return (paths[a.index]->cost < paths[b.index]->cost) ? a:b;
        }
    }

    return (min_ret_t){i, _TRUE};
}

void apply_dijkstra(rnsGraph_t *graph, repository_t *start,
                                       repository_t *end)
{
    size_t i,k,j;
    min_ret_t min;
    path_t *a, *b, *tmp;
    path_t **paths;
    unsigned long cc;

    if (!graph || !start || !end){
          fprintf(stderr, "NULL in function args (bad repository index)\n");
          return;
    }

    paths = calloc(graph->size, sizeof(path_t*));

    j=0;
    for (i=0; i<graph->size; i++){
          if(graph->repos[i]){
              graph->repos[i]->data = malloc(sizeof(size_t));
              //initialize data with initial position
              if(graph->repos[i] == start){
                  paths[j] = paths[0];
                  paths[0] = new_path(0, _FALSE, NULL, start);
                  SET_DATA(graph->repos[i], 0);
                  SET_DATA(graph->repos[0], j);
              }else {
                  paths[j] = new_path((unsigned long)-1, _FALSE, NULL,
                                      graph->repos[i]);
                  SET_DATA(graph->repos[i], j);
              }
          }else{
              j--;
          }
          j++;
    }

    while (1){

        GET_DATA(k, end);

        min = get_min(0, graph->size, paths);

        if(min.err){
              break;
        }
        a = paths[min.index];

        if(a->repo == end){
              break;
        }

        a->visited = _TRUE;

        for (i=0; i<a->repo->nbrLinks; i++){

            if (!a->repo->links[i]){
                  continue;
            }

            GET_DATA(k, a->repo->links[i]->repo); //compute index in paths
            b = paths[k];
            if(!b || b->visited){
                continue;
            }
            cc = cost(a->repo, a->repo->links[i]) + a->cost;

            if (cc >= b->cost){
                continue;
            }
            b->cost = cc;
            b->from = a->repo;
            b->link = a->repo->links[i];
            //level up b in heap
            while (k && b->cost < paths[PARENT(k)]->cost)
            {
                SET_DATA(paths[PARENT(k)]->repo, k);
                paths[k] = paths[PARENT(k)];
                k = PARENT(k);
            }
            paths[k] = b;
            SET_DATA(b->repo, k); //update with current index in heap
        }
    }

    do {
        GET_DATA(k, end);
        if(!paths[k]->from){
            fprintf(stderr, "No path found\n");
            break;
        }
        paths[k]->link->is_path = _TRUE;
        end = paths[k]->from;
    }while (end != start);
}
