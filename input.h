#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

#include <json/json.h>

#include "renderer.h"

/**
  * Parse un fichier JSON et renvoie un objet
  * le représentant
  */
struct json_object *parse_file_json(const char *filename);

/**
  * Parse un fichier RNS et renvoie un contexte de rendu
  */
rendering_ctx_t *graph_from_rns_file(const char *filename);

/**
  * Traite un objet JSON et renvoie un contexte de rendu
  */
rendering_ctx_t *graph_from_json(struct json_object *json);

#endif
