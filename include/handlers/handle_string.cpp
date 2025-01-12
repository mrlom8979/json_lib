/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_string(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_STRING");

  if (ctx.current_node->type == JSON_AST_OBJECT && !ctx.current_key) {

    // Если это ключ для объекта
    if (ctx.current_key) free(ctx.current_key);

    ctx.current_key = strdup(t.value);

  } else if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {

    // Если это значение для текущего ключа
    ast_node* string_node = ast_create_string(t.value);
    ast_node* pair_node = ast_create_pair(ctx.current_key, string_node);

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
  } else if (ctx.current_node->type == JSON_AST_ARRAY) {
    // Если текущий узел - это массив, добавляем строку в массив
    ast_node* string_node = ast_create_string(t.value);

    ctx.current_node->array_values = (ast_node**)realloc(
      ctx.current_node->array_values,
      (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    );
    ctx.current_node->array_values[ctx.current_node->value_count++] = string_node;
  }

}

} // namespace json

