/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "log.h"
#include "parser.h"
#include "ast.h"

#define __json_get_val(ast, path) json::get_value_from_ast(ast, #path)

// Получение int
#define __json_get_int(ast, path)                                             \
  atoi((__json_get_val(ast, path))->value);                                   \

// Получение double
#define __json_get_double(ast, path)                                          \
  atof((__json_get_val(ast, path))->value);                                   \

// Получение bool
#define __json_get_bool(ast, path)                                            \
  (strcmp((__json_get_val(ast, path))->value, "true") == 0 ? true : false)    \

// Получение строки
#define __json_get_cstr(ast, path)                                            \
  (__json_get_val(ast, path))->value;                                         \

json::ast_node* json_decode(const char* j);
