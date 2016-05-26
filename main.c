#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "input.h"
#include "renderer.h"
#include "graphGeneration.h"
#include "pathfinding.h"

int main(int argc, char *argv[]){

    int i,f,e,o,n,p,s,d,r,l,m,c;
    int a;

    char *inputFile;
    char *fmt;
    char *ext;
    char *outputFile;
    char *mode;
    char *start;
    char *dest;
    int index;
    size_t nodes;
    size_t maxedges;

    rendering_ctx_t *ctx;

    if (argc == 1){
          fprintf (stderr, "options: i,f,e,o,m,p,s,d,l,n,c\n");
          return;
    }

    c = 0;
    opterr = 0;

    while ((a = getopt (argc, argv, "i:f:e:o:m:ps:d:l:n:c")) != -1){
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
              l = 1;
              maxedges = atoi(optarg);
              break;
          case 'n':
              n = 1;
              nodes = atoi(optarg);
              break;
          case 'c':
              c = 1;
              break;
          case '?':
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
              return 1;
          default :
              return;
        }
     }

     if (i && f){
          if (!strcmp(fmt, "rns")){
              ctx = graph_from_rns_file(inputFile);
          }else if (!strcmp(fmt, "json")){
              ctx = graph_from_json(parse_file_json("input/input.json"));
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
     free(ctx);

     return 0;
}
