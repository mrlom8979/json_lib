/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void add_node_to_collection(ast_node* parent, ast_node* new_node) {
  if (!parent) {
    __err("Parent node is null.");
    return;
  }

  if (parent->type == JSON_AST_ARRAY) {
    parent->array_values = (ast_node**)realloc(
      parent->array_values,
      (parent->value_count + 1) * sizeof(ast_node*)
    );

    if (!parent->array_values) {
      __err("Memory allocation failed for array_values.");
      return;
    }

    parent->array_values[parent->value_count++] = new_node;
  } else if (parent->type == JSON_AST_OBJECT) {
    parent->object_values = (ast_node**)realloc(
      parent->object_values,
      (parent->value_count + 1) * sizeof(ast_node*)
    );

    if (!parent->object_values) {
      __err("Memory allocation failed for object_values.");
      return;
    }

    parent->object_values[parent->value_count++] = new_node;
  } else {
    __err("Unsupported parent node type: %d", parent->type);
  }
}

} // namespace json

