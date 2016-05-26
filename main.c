#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "input.h"
#include "renderer.h"
#include "graphGeneration.h"
#include "pathfinding.h"

int main(int argc, char *argv[]){

    int i,f,e,o,p,s,d,r,m,c;
    int a;

    i=0;
    f=0;
    e=0;
    p=0;
    s=0;
    d=0;
    r=0;
    m=0;
    c=0;

    char *inputFile;
    char *fmt;
    char *ext;
    char *outputFile;
    char *mode;
    char *start;
    char *dest;

    size_t nodes;
    size_t maxedges;

    rendering_ctx_t *ctx;

    if (argc == 1){
          fprintf (stderr, "options: i,f,e,o,m,p,s,d,l,n,c\n");
          return 0;
    }

    c = 0;
    opterr = 0;

    while ((a = getopt (argc, argv, "i:f:e:o:m:ps:d:rl:n:c")) != -1){
        switch (a) {
          case 'i':
              i = 1;
              inputFile = optarg;
              break;
          case 'f':
              f = 1;
              fmt = optarg;
              break;
          case 'e':
              e = 1;
              ext = optarg;
              break;
          case 'o':
              o = 1;
              outputFile = optarg;
              break;
          case 'm':
              m = 1;
              mode = optarg;
              break;
          case 'p':
              p = 1;
              break;
          case 's':
              s = 1;
              start = optarg;
              break;
          case 'd':
              d = 1;
              dest = optarg;
              break;
          case 'r':
              r = 1;
              break;
          case 'l':
              maxedges = atoi(optarg);
              break;
          case 'n':
              nodes = atoi(optarg);
              break;
          case 'c':
              c = 1;
              break;
          case '?':
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
              return 1;
          default :
              return 1;
        }
     }

     if (i && f){
          if (!strcmp(fmt, "rns")){
              ctx = graph_from_rns_file(inputFile);
          }else if (!strcmp(fmt, "json")){
              ctx = graph_from_json(parse_file_json(inputFile));
          }else{
              return 1;
          }
     }else if (r && m){
          ctx = new_rctx();

          if (!strcmp(mode, "digraph")){
              ctx->directed = _TRUE;
          }else if (!strcmp(mode, "graph")){
              ctx->directed = _FALSE;
          }else{
              return 1;
          }

          ctx->graph = randomGeneration(nodes, maxedges, ctx->directed,
                                              (bool_t)c);
     }

     if (p && s && d){
         apply_dijkstra(ctx->graph, repository_by_id(ctx->graph,start),
                                    repository_by_id(ctx->graph,dest));
     }

     if (o && e){
         render_graph(ctx, outputFile, ext);
     }

     if(r && m){
         rns_freeGraph(ctx->graph);
     }

     if(i && f){
         free(ctx);
     }

     return 0;
}
