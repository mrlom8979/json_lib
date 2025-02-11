/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_left_brace(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_LEFT_BRACE");

  ast_node* new_object = ast_create_object(0);

  if (!ctx.root) { 
    // Если корневой узел не установлен
    ctx.root = new_object;
  } else if (ctx.current_node->type == JSON_AST_OBJECT && ctx.current_key) {
    // Добавляем объект как значение текущего ключа
    ast_node* pair = ast_create_pair(ctx.current_key, new_object);
    
    add_node_to_collection(ctx.current_node, pair);

    if (ctx.current_key) {
      free(ctx.current_key);
      ctx.current_key = nullptr;
    }

    ctx.current_key = nullptr;

  } else if (ctx.current_node->type == JSON_AST_OBJECT && !ctx.current_key) {
    add_node_to_collection(ctx.current_node, new_object);
  } else if (ctx.current_node->type == JSON_AST_ARRAY) {
    add_node_to_collection(ctx.current_node, new_object);
  }

  // Сохраняем текущий узел в стек
  if (++ctx.stack_top_object >= ctx.stack_capacity_object) {
    ctx.stack_capacity_object = ctx.stack_capacity_object > 0 ? ctx.stack_capacity_object * 2 : 8;
    ctx.stack_object = (ast_node**)realloc(ctx.stack_object, ctx.stack_capacity_object * sizeof(ast_node*));
    if (!ctx.stack_object) {
      __err("Memory allocation for stack failed.");
      return;
    }
  }
  ctx.stack_object[ctx.stack_top_object] = ctx.current_node;

  // Новый текущий узел
  ctx.current_node = new_object;
}

} // namespace json

