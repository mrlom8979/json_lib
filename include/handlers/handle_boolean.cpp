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

    add_node_to_collection(ctx.current_node, pair_node);
    
    if (ctx.current_key) free(ctx.current_key);

    ctx.current_key = nullptr;

  } delse if (ctx.current_node->type == JSON_AST_ARRAY) {
    // Если текущий узел - это массив, добавляем булевое значение в массив
    int boolean_value = (t.type == JSON_TOKEN_TRUE) ? 1 : 0;
    ast_node* boolean_node = ast_create_boolean(boolean_value);

    add_node_to_collection(ctx.current_node, boolean_node);
  }
}

} // namespace json



