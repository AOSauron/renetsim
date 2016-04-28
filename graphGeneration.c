#include <stdlib.h>
#include <time.h>

#include "graphGeneration.h"


rnsGaph_t *newRandomGraph(int size){
    srand(time(NULL));

    rnsGaph_t *graph = rns_newGraph(size);

}
