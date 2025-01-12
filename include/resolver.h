/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "log.h"
#include "parser.h"
#include "ast.h"

namespace json {

typedef struct {
  ast_node* root;
  ast_node* current_node;
  
  ast_node** stack_object;
  size_t stack_top_object;
  size_t stack_capacity_object;

  ast_node** stack_array = nullptr;
  size_t stack_top_array = 0;
  size_t stack_capacity_array = 0;

  char* current_key;
} handler_ctx;

  typedef void (*token_handler)(handler_ctx&, const token&);

  struct token_resolver {
    token_type type;
    token_handler handler;
  };

  void resolve_token(const token& token, handler_ctx& ctx, const token_resolver* resolvers, size_t resolver_count);

} // namespace json

