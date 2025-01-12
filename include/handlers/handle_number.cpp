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

    // Добавляем пару в текущий объект
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
          // Если текущий узел - это массив, добавляем число в массив
          double number_value = strtod(current_token.value, nullptr);
          ast_node* number_node = ast_create_number(number_value);

          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = number_node;
        }*/

}

} // namespace json

