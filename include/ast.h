/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "log.h"
#include "parser.h"

namespace json {

typedef enum {
  JSON_AST_OBJECT,
  JSON_AST_ARRAY,
  JSON_AST_PAIR,
  JSON_AST_STRING,
  JSON_AST_NUMBER,
  JSON_AST_BOOLEAN,
  JSON_AST_NULL
} ast_type;

typedef struct ast_pair {
  char *key;          // Ключ
  struct ast_node *value;  // Значение
} ast_pair;

typedef struct ast_node {
  ast_type type;
  union {
    char *string_value;               // Строка (для JSON_AST_STRING)
    double number_value;              // Число (для JSON_AST_NUMBER)
    int boolean_value;                // Булевое значение (для JSON_AST_BOOLEAN)
    struct ast_pair *pair_value;      // Пара ключ-значение (для JSON_AST_PAIR)
    struct ast_node **array_values;   // Элементы массива (для JSON_AST_ARRAY)
    struct ast_node **object_values;  // Элементы объекта (для JSON_AST_OBJECT)
  };
  size_t value_count;
} ast_node;

typedef struct {
  int type;               // Тип узла: JSON_AST_STRING, JSON_AST_NUMBER и т.д.
  char* value;            // Значение в строковом представлении
} ast_result;

ast_node *ast_create_object(size_t value_count);
ast_node *ast_create_array();
ast_node *ast_create_string(const char *value);
ast_node *ast_create_number(double value);
ast_node *ast_create_boolean(int value);
ast_node *ast_create_null();
ast_node *ast_create_pair(char *key, ast_node *value);

ast_node *ast_build_from_tokens(parser *p);

void ast_free_node(ast_node *node);
void ast_free_pair(ast_pair *pair);

ast_result* get_value_from_ast(ast_node* root, const char* path);

void print_ast_node(ast_node *node, int indent);
void print_ast(ast_node *root);

}

