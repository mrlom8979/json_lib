/*
 * Copyright (C) Yakiv Matiash
 */

#include "ast.h"

#include "resolver.h"
#include "handlers/handlers.h"

namespace json {

ast_node *ast_create_object(size_t value_count = 0) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_OBJECT;
  node->object_values = (value_count > 0) ? (ast_node **)malloc(value_count * sizeof(ast_node *)) : NULL;
  node->value_count = value_count;
  return node;
}

ast_node *ast_create_array() {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_ARRAY;
  node->array_values = NULL;
  node->value_count = 0;
  return node;
}

ast_node *ast_create_string(const char *value) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_STRING;
  node->string_value = strdup(value); // Копирование строки
  return node;
}

ast_node *ast_create_number(double value) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_NUMBER;
  node->number_value = value;
  return node;
}

ast_node *ast_create_boolean(int value) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_BOOLEAN;
  node->boolean_value = value;
  // node->object_values = nullptr;
  // node->value_count = 0;
  return node;
}

ast_node *ast_create_null() {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_NULL;
  return node;
}

ast_node *ast_create_pair(char *key, ast_node *value) {
  ast_pair *pair = (ast_pair *)malloc(sizeof(ast_pair));
  pair->key = strdup(key);  // Копирование ключа
  pair->value = value;

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  node->type = JSON_AST_PAIR;
  node->pair_value = pair;
  return node;
}

void ast_free_node(ast_node *node) {
  if (!node) return;
  switch (node->type) {
    case JSON_AST_STRING:
      free(node->string_value);
      break;
    case JSON_AST_PAIR:
      ast_free_pair(node->pair_value);
      break;
    case JSON_AST_OBJECT:
      for (size_t i = 0; i < node->value_count; i++) {
        ast_free_node(node->object_values[i]);
      }
      free(node->object_values);
      break;
    case JSON_AST_ARRAY:
      for (size_t i = 0; i < node->value_count; i++) {
        ast_free_node(node->array_values[i]);
      }
      free(node->array_values);
      break;
    case JSON_AST_NUMBER:
    case JSON_AST_BOOLEAN:
    case JSON_AST_NULL:
      break;
  }
  free(node);
}

void ast_free_pair(ast_pair *pair) {
  if (!pair) return;
  free(pair->key);
  ast_free_node(pair->value);
  free(pair);
}

ast_node* ast_build_from_tokens(parser* p) {

  token_resolver resolvers[] = {
    { JSON_TOKEN_LEFT_BRACE, handle_left_brace },
    { JSON_TOKEN_RIGHT_BRACE, handle_right_brace },
    { JSON_TOKEN_LEFT_BRACKET, handle_left_bracket },
    { JSON_TOKEN_RIGHT_BRACKET, handle_right_bracket },
    { JSON_TOKEN_STRING, handle_string },
    { JSON_TOKEN_NUMBER, handle_number },
    { JSON_TOKEN_TRUE, handle_boolean },
    { JSON_TOKEN_FALSE, handle_boolean },
    { JSON_TOKEN_NULL, handle_null },
  };

  size_t resolver_count = sizeof(resolvers) / sizeof(resolvers[0]);

  handler_ctx ctx = {
    nullptr,
    nullptr,
    nullptr,
    0,
    0,
    nullptr
  };

  token current_token;
  token next_token = parser_next_token(p);

  while (next_token.type != JSON_TOKEN_END) {
    current_token = next_token;
    next_token = parser_next_token(p);

    resolve_token(current_token, ctx, resolvers, resolver_count);

  }

  // free(stack);
  free(ctx.stack_object);
  // free(stack_array);
  if (ctx.current_key) free(ctx.current_key);

  return ctx.root;
}



// ------------------------------------------------------



ast_result* get_value_from_ast(ast_node* root, const char* path) {
  if (!root || !path) return nullptr;

  printf("Get value from path: %s\n", path);

  // Копируем путь, чтобы безопасно его модифицировать
  char* path_copy = strdup(path);
  if (!path_copy) return nullptr;

  char* token = strtok(path_copy, "->");
  ast_node* current_node = root;

  printf("Token path: %s\n", token);

  while (token) {
    bool key_found = false;
    // Проверка на квадратные скобки, чтобы извлечь индекс
    char* index_str = strchr(token, '[');
    if (index_str) {
      // Разделяем token на ключ и индекс
      *index_str = '\0'; // Разделяем ключ (например "roles")
      int index = atoi(index_str + 1); // Извлекаем индекс после '['

      // Убираем ']' в конце
      if (index_str[strlen(index_str) - 1] == ']') {
        index_str[strlen(index_str) - 1] = '\0';
      }

      printf("Extracted key: %s, index: %d\n", token, index);

      // Проверяем, что текущий узел - это объект, и ищем ключ
      if (current_node->type == JSON_AST_OBJECT) {
        for (size_t i = 0; i < current_node->value_count; ++i) {
          json::ast_pair* pair = current_node->object_values[i]->pair_value;

          if (pair && strcmp(pair->key, token) == 0) {
            current_node = pair->value; // Переходим на следующий уровень
            key_found = true;
            break;
          }
        }
      }

      // После того как нашли ключ, проверяем, что это массив и извлекаем элемент по индексу
      if (key_found && current_node->type == JSON_AST_ARRAY) {
        if (index >= 0 && index < current_node->value_count) {
          current_node = current_node->array_values[index];
          key_found = true;
        } else {
          free(path_copy);
          return nullptr;  // Индекс за пределами массива
        }
      }
    } else {
      // Если это просто ключ (без индекса)
      if (current_node->type == JSON_AST_OBJECT) {
        for (size_t i = 0; i < current_node->value_count; ++i) {
          json::ast_pair* pair = current_node->object_values[i]->pair_value;

          if (pair && strcmp(pair->key, token) == 0) {
            current_node = pair->value;
            key_found = true;
            break;
          }
        }
      }
    }

    // Если ключ не найден
    if (!key_found) {
      printf("Key not found: %s\n", token);
      free(path_copy);
      return nullptr; // Ключ не найден
    }

    // Переходим к следующему сегменту пути
    token = strtok(nullptr, "->");

    // Если осталась часть пути, проверяем, что текущий узел - это объект или массив
    if (token && !(current_node && (current_node->type == JSON_AST_OBJECT || current_node->type == JSON_AST_ARRAY))) {
      printf("Invalid node type for path segment: %s\n", token);
      free(path_copy);
      return nullptr; // Следующая часть пути невозможна
    }
  }

  free(path_copy);

  // Создаем результат
  ast_result* result = (ast_result*)malloc(sizeof(ast_result));
  if (!result) return nullptr;

  // Заполняем результат
  if (current_node) {
    result->type = current_node->type;
    switch (current_node->type) {
      case JSON_AST_STRING:
        result->value = strdup(current_node->string_value);
        break;
      case JSON_AST_NUMBER: {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.17g", current_node->number_value); // Конвертация числа в строку
        result->value = strdup(buffer);
        break;
      }
      case JSON_AST_BOOLEAN:
        result->value = strdup(current_node->boolean_value ? "true" : "false"); // Конвертация булевого значения
        break;
      case JSON_AST_NULL:
        result->value = strdup("null"); // Конвертация null в строку "null"
        break;
      default:
        free(result);
        return nullptr; // Тип не поддерживается
    }
  } else {
    free(result);
    return nullptr; // Узел не найден
  }

  return result;
}










void print_ast_node(ast_node *node, int indent) {
  if (!node) return;

  // Добавление отступа для визуализации вложенности
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }

  // Вывод информации о типе узла
  switch (node->type) {
    case JSON_AST_OBJECT:
      printf("{\n");
      for (size_t i = 0; i < node->value_count; ++i) {
        ast_pair *pair = node->object_values[i]->pair_value;
        printf("  ");
        printf("\"%s\": ", pair->key);
        print_ast_node(pair->value, indent + 1);
      }
      for (int i = 0; i < indent; i++) {
        printf("  ");
      }
      printf("}\n");
      break;

    case JSON_AST_ARRAY:
      printf("[\n");
      for (size_t i = 0; i < node->value_count; ++i) {
        print_ast_node(node->array_values[i], indent + 1);
      }
      for (int i = 0; i < indent; i++) {
        printf("  ");
      }
      printf("]\n");
      break;

    case JSON_AST_STRING:
      printf("\"%s\"\n", node->string_value);
      break;

    case JSON_AST_NUMBER:
      printf("%lf\n", node->number_value);
      break;

    case JSON_AST_BOOLEAN:
      printf(node->boolean_value ? "true\n" : "false\n");
      break;

    case JSON_AST_NULL:
      printf("null\n");
      break;

    case JSON_AST_PAIR:
      printf("\"%s\": ", node->pair_value->key);
      print_ast_node(node->pair_value->value, indent);
      break;

    default:
      printf("Unknown node type\n");
      break;
  }
}

// Функция для вывода всего дерева
void print_ast(ast_node *root) {
  if (root) {
    print_ast_node(root, 0);
  } else {
    printf("Empty tree\n");
  }
}



} // namespace json
