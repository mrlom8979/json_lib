/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "log.h"
#include "parser.h"
#include "ast.h"

#define __json_get_val(ast, path) json::get_value_from_ast(ast, #path)

json::ast_node* json_decode(const char* j);
