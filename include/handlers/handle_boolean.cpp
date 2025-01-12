/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_boolean(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_FALSE");
  if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {
    int boolean_value = (t.type == JSON_TOKEN_TRUE) ? 1 : 0;
    ast_node* boolean_node = ast_create_boolean(boolean_value);
    ast_node* pair_node = ast_create_pair(ctx.current_key, boolean_node);

    ctx.current_node->object_values = (ast_node**)realloc(
      ctx.current_node->object_values,
      (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    );
    ctx.current_node->object_values[ctx.current_node->value_count++] = pair_node;

    // free(current_key);
    if (ctx.current_key) {
      free(ctx.current_key);
    }

    ctx.current_key = nullptr;
  }/* else if (current_node->type == JSON_AST_ARRAY) {
          // Если текущий узел - это массив, добавляем булевое значение в массив
          int boolean_value = (current_token.type == JSON_TOKEN_TRUE) ? 1 : 0;
          ast_node* boolean_node = ast_create_boolean(boolean_value);

          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = boolean_node;
        }*/

}

} // namespace json



