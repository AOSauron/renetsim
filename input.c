#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "input.h"

#define SE "SET_EDGES"
#define SG "SET_GRAPH"
#define SR "SET_REPOSITORIES"

typedef enum {
    S_NONE,
    S_GRAPH,
    S_REPOS,
    S_EDGES
} input_section_t;

typedef enum {
    E_INT,
    E_FMT,
    E_INP
} error_t;

static inline
void error(error_t err){
    const char *msg;

    switch (err){
      case E_INT:
        msg = "Integrity error"; break;
      case E_FMT:
        msg = "Error in file format"; break;
      case E_INP:
        msg = "Cannot read input file"; break;
    }
    fprintf(stderr, "[ERROR] %s\n", msg);
    exit(EXIT_FAILURE);
}

static inline
bool_t s_check(input_section_t s1, input_section_t s2){
    if (s1 != s2){
        error(E_FMT);
    }
    return _TRUE;
}

rendering_ctx_t *graph_from_rns_file(const char *filename){
    rendering_ctx_t *ctx;
    input_section_t sec;
    rnsGraph_t *graph;
    repository_t *repo, *src, *dest;
    FILE *fp;
    char line[1024];
    char type[8];
    char id[32];
    char id2[32];
    int n, w;
    int x, y;

    fp = fopen(filename, "r");
    if (!fp) {
      error(E_INP);
    }

    ctx = new_rctx();
    sec = S_NONE;

    while (fgets(line, sizeof(line), fp) != NULL){
        if (line[0] == '\n'){
            continue;
        }

        if (!strncmp(line, SG, 9) && s_check(sec, S_NONE)){
              sec = S_GRAPH; continue;
        }else if (!strncmp(line, SR, 16) && s_check(sec, S_GRAPH)){
              sec = S_REPOS; continue;
        }else if (!strncmp(line, SE, 9) && s_check(sec, S_REPOS)){
              sec = S_EDGES; continue;
        }

        switch (sec){
          case S_GRAPH:
              if (sscanf(line, "%[^:]:%d", type, &n) != 2){
                  error(E_FMT);
              }
              if (strncmp(line, "digraph", 7) == 0){
                  ctx->directed = _TRUE;
              }else if (strncmp(type, "graph", 5) == 0){
                  ctx->directed = _FALSE;
              }else{ error(E_FMT); }

              graph = rns_newGraph(n);
              ctx->graph = graph;
              break;

          case S_REPOS:
              if (sscanf(line, "%[^:]:%d:(%d,%d)", id, &n, &x, &y) != 4){
                  error(E_FMT);
              };
              repo = rns_newRepository(n, x, y);
              if (rns_addRepository(graph, repo, id, _TRUE) == FAILURE){
                  error(E_INT);
              }
              break;

          case S_EDGES:
              if (sscanf(line, "%[^->]->%[^:]:%d", id, id2, &w) != 3){
                  error(E_FMT);
              };
              src = repository_by_id(graph, id);
              dest = repository_by_id(graph, id2);

              if (src && dest){
                  if (rns_directedEdge(src, dest, w) == FAILURE){
                      error(E_INT);
                  };
                  if (!ctx->directed &&
                      rns_directedEdge(dest, src, w) == FAILURE)
                  {
                      error(E_INT);
                  }
              }else{
                  error(E_INT);
              }
              break;
          case S_NONE:
              break;
        }
      }

      if (sec != S_EDGES){
            error(E_FMT);
      }
      fclose(fp);

      return ctx;
}

struct json_object *parse_file_json(const char *filename){

    int fd;
    struct stat fs;
    void *fmap;
    struct json_object *json;

    fd = open(filename, O_RDONLY);
    if (fd < 0 || fstat(fd, &fs) == -1){
      error(E_INP);
      return NULL;
    }
    fmap = mmap(0, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (fmap == MAP_FAILED){
      return NULL;
    }
    json = json_tokener_parse(fmap);

    if (!json){
      error(E_INP);
      return NULL;
    }
    return json;
}

rendering_ctx_t *graph_from_json(struct json_object *json){
    rendering_ctx_t *ctx;
    struct json_object *g, *nodes, *edges, *node, *edge, *position;
    json_bool directed;
    repository_t *repo, *src, *dest;
    rnsGraph_t *graph;
    char const *id;
    size_t nNodes, nEdges, idx, n;
    int x, y, weight;

    g = json_object_object_get(json, "graph");
    directed = json_object_get_boolean(json_object_object_get(g, "directed"));
    nodes = json_object_object_get(g, "nodes");
    edges = json_object_object_get(g, "edges");
    nNodes = json_object_array_length(nodes);
    nEdges = json_object_array_length(edges);

    graph = rns_newGraph(nNodes);

    for (idx=0; idx<nNodes; idx++){
        node = json_object_array_get_idx(nodes, idx);
        id = json_object_get_string(json_object_object_get(node, "id"));
        n = json_object_get_int(json_object_object_get(node, "edges"));
        position = json_object_object_get(node, "position");
        x = json_object_get_int(json_object_array_get_idx(position, 0));
        y = json_object_get_int(json_object_array_get_idx(position, 1));

        repo = rns_newRepository(n, x, y);

        if (rns_addRepository(graph, repo, id, _TRUE) == FAILURE)
        {
          error(E_INT);
          return NULL;
        };
    }

    for (idx=0; idx<nEdges; idx++){
        edge = json_object_array_get_idx(edges, idx);
        weight = json_object_get_int(json_object_object_get(edge, "weight"));
        id = json_object_get_string(json_object_object_get(edge, "source"));
        src = repository_by_id(graph, id);
        id = json_object_get_string(json_object_object_get(edge, "target"));
        dest = repository_by_id(graph, id);

        if (!src || !dest){
            error(E_INT);
        }

        if(rns_directedEdge(src, dest, weight) == FAILURE){
            error(E_INT);
        };

        if(!directed && rns_directedEdge(dest, src, weight) == FAILURE){
            error(E_INT);
        }
    }

    ctx = new_rctx();
    ctx->directed = (bool_t) directed;
    ctx->graph = graph;
    return ctx;
}
