/*
 * Copyright (C) Yakiv Matiash
 */

#include "json.h"

json::ast_node* json_decode(const char* j) {
  json::parser p;
  json::parser_init(&p, j);
  return json::ast_build_from_tokens(&p);
}
