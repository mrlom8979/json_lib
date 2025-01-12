/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_null(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_NULL");

  // Проверка, что текущий узел является объектом и существует текущий ключ
  if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {
    // Создание узла для null
    ast_node* null_node = ast_create_null();

    // Создание пары "ключ - значение" и добавление её в объект
    ast_node* pair_node = ast_create_pair(ctx.current_key, null_node);

    ctx.current_node->object_values = (ast_node**)realloc(
      ctx.current_node->object_values,
      (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    );
    ctx.current_node->object_values[ctx.current_node->value_count++] = pair_node;

    // Освобождение текущего ключа
    if (ctx.current_key) {
      free(ctx.current_key);
    }

    ctx.current_key = nullptr;
  } else if (ctx.current_node->type == JSON_AST_ARRAY) {
    // Если текущий узел - это массив, добавляем null в массив
    ast_node* null_node = ast_create_null();

    ctx.current_node->array_values = (ast_node**)realloc(
      ctx.current_node->array_values,
      (ctx.current_node->value_count + 1) * sizeof(ast_node*)
    );
    ctx.current_node->array_values[ctx.current_node->value_count++] = null_node;
  }

}

} // namespace json

