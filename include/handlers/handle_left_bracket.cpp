/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_left_bracket(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_LEFT_BRACKET");
  ast_node* new_array = ast_create_array();

  if (!ctx.root) {
    ctx.root = new_array;
  } else if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {
    ast_node* pair = ast_create_pair(ctx.current_key, new_array);
    
    add_node_to_collection(ctx.current_node, pair);

    // ctx.current_node->object_values = (ast_node**)realloc(
      // ctx.current_node->object_values,
      // (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    // );
    // ctx.current_node->object_values[ctx.current_node->value_count++] = pair;

    // free(current_key);
    if (ctx.current_key) {
      free(ctx.current_key);
    }

    ctx.current_key = nullptr;

  } else if (ctx.current_node->type == JSON_AST_ARRAY) {
    
    add_node_to_collection(ctx.current_node, new_array);

    // ctx.current_node->array_values = (ast_node**)realloc(
      // ctx.current_node->array_values,
      // (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    // );
    // ctx.current_node->array_values[ctx.current_node->value_count++] = new_array;
  }

  if (++ctx.stack_top_array >= ctx.stack_capacity_array) {
    ctx.stack_capacity_array = ctx.stack_capacity_array > 0 ? ctx.stack_capacity_array * 2 : 8;
    ctx.stack_array = (ast_node**)realloc(ctx.stack_array, ctx.stack_capacity_array * sizeof(ast_node*));
    if (!ctx.stack_array) {
      __err("Memory allocation for stack failed.");
      return;
    }
  }
  ctx.stack_array[ctx.stack_top_array] = ctx.current_node;
  ctx.current_node = new_array;

}

} // namespace json

