/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_number(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_NUMBER");

  if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {
    // Создаем числовой узел
    double number_value = strtod(t.value, nullptr);
    ast_node* number_node = ast_create_number(number_value);
    ast_node* pair_node = ast_create_pair(ctx.current_key, number_node);

    add_node_to_collection(ctx.current_node, pair_node);

    if (ctx.current_key) free(ctx.current_key);

    ctx.current_key = nullptr;
  } else if (ctx.current_node->type == JSON_AST_ARRAY) {
    // Если текущий узел - это массив, добавляем число в массив
    double number_value = strtod(t.value, nullptr);
    ast_node* number_node = ast_create_number(number_value);

    add_node_to_collection(ctx.current_node, number_node);
  }
}

} // namespace json

