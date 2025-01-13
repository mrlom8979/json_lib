/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "../log.h"
#include "../parser.h"
#include "../ast.h"
#include "../resolver.h"

namespace json {

void add_node_to_collection(ast_node* parent, ast_node* new_node);

void handle_left_brace(handler_ctx& ctx, const token& t);
void handle_right_brace(handler_ctx& ctx, const token& t);
void handle_left_bracket(handler_ctx& ctx, const token& t);
void handle_right_bracket(handler_ctx& ctx, const token& t);
// void handle_comma(const token& t);
// void handle_colon(const token& t);
void handle_string(handler_ctx& ctx, const token& t);
void handle_number(handler_ctx& ctx, const token& t);
// void handle_true(const token& t);
void handle_boolean(handler_ctx& ctx, const token& t);
void handle_null(handler_ctx& ctx, const token& t);
// void handle_object(const token& t);
// void handle_array(const token& t);
// void handle_end(const token& t);
// void handle_invalid(const token& t);


} // namespace json

