/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_left_bracket(handler_ctx& ctx, const token& t) {
  /*
        PHX_CORE_DEBUG("JSON_TOKEN_LEFT_BRACKET");
        ast_node* new_array = ast_create_array();

        if (!root) {
          root = new_array;
        } else if (current_node->type == JSON_AST_OBJECT && current_key) {
          ast_node* pair = ast_create_pair(current_key, new_array);
          current_node->object_values = (ast_node**)realloc(
            current_node->object_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->object_values[current_node->value_count++] = pair;

          // free(current_key);
          if (current_key) {
            free(current_key);
          }

          current_key = nullptr;

        } else if (current_node->type == JSON_AST_ARRAY) {
          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = new_array;
        }

        if (++stack_top_array >= stack_capacity_array) {
          stack_capacity_array = stack_capacity_array > 0 ? stack_capacity_array * 2 : 8;
          stack_array = (ast_node**)realloc(stack_array, stack_capacity_array * sizeof(ast_node*));
          if (!stack_array) {
            __err("Memory allocation for stack failed.");
            return nullptr;
          }
        }
        stack_array[stack_top_array] = current_node;
        current_node = new_array;
        */

}

} // namespace json

