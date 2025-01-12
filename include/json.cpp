/*
 * Copyright (C) Yakiv Matiash
 */

#include "json.h"

// #define __json_get_val(ast, path) json::get_value_from_ast(ast, path)

json::ast_node* json_decode(const char* j) {

  json::parser p;
  json::parser_init(&p, j);

  // json::ast_node *r = json::ast_build_from_tokens(&p);
  return json::ast_build_from_tokens(&p);

  // json::ast_universal_result* result = json::get_value_from_ast(r, "user->age");
  /*
  json::ast_universal_result* result = __json_get_val(r, user->name);

  if (result) {
    printf("--- Result ------------------------------------------------\n");
    printf("Value: %s\n", result->value);

    if (result->type == json::JSON_AST_STRING) {
      printf("Type: String\n");
    } else if (result->type == json::JSON_AST_NUMBER) {
      int number = atoi(result->value); // Приведение к числу
      printf("Type: Number\n");
      printf("Number: %d\n", number);
    } else if (result->type == json::JSON_AST_BOOLEAN) {
      printf("Type: Boolean\n");
      printf("Boolean: %s\n", strcmp(result->value, "true") == 0 ? "True" : "False");
    }

    free(result->value);
    free(result);
  } else {
    printf("Value not found or invalid path.\n");
  }
  */
}
