#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

#include <json/json.h>

#include "renderer.h"

struct json_object *parse_file_json(const char *filename);

rendering_ctx_t *graph_from_rns_file(const char *filename);
rendering_ctx_t *graph_from_json(struct json_object *json);

#endif
