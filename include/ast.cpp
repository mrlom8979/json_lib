/*
 * Copyright (C) Yakiv Matiash
 */

#include "ast.h"

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

  ast_node* root = nullptr;          // Корневой узел
  ast_node* current_node = nullptr;  // Текущий узел

  // ast_node** stack = nullptr;        // Динамический стек
  // int stack_top = -1;
  // int stack_capacity = 0;

  ast_node** stack_object = nullptr;
  size_t stack_top_object = 0;
  size_t stack_capacity_object = 0;

  // ast_node** stack_array = nullptr;
  // size_t stack_top_array = 0;
  // size_t stack_capacity_array = 0;

  char* current_key = nullptr;       // Текущий ключ для пар

  token current_token;
  token next_token = parser_next_token(p);

  while (next_token.type != JSON_TOKEN_END) {
    current_token = next_token;
    next_token = parser_next_token(p);

    switch (current_token.type) {
      case JSON_TOKEN_LEFT_BRACE: { // Начало объекта

        __debug("JSON_TOKEN_LEFT_BRACE");

        ast_node* new_object = ast_create_object();

        if (!root) { // Если корневой узел не установлен
          root = new_object;
        } else if (current_node->type == JSON_AST_OBJECT && current_key) {
          // Добавляем объект как значение текущего ключа
          ast_node* pair = ast_create_pair(current_key, new_object);

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

        }/* else if (current_node->type == JSON_AST_ARRAY) {
          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = new_object;
        }*/

        // Сохраняем текущий узел в стек
        if (++stack_top_object >= stack_capacity_object) {
          stack_capacity_object = stack_capacity_object > 0 ? stack_capacity_object * 2 : 8;
          stack_object = (ast_node**)realloc(stack_object, stack_capacity_object * sizeof(ast_node*));
          if (!stack_object) {
            __err("Memory allocation for stack failed.");
            return nullptr;
          }
        }
        stack_object[stack_top_object] = current_node;

        // Новый текущий узел
        current_node = new_object;

        break;
      } // END JSON_TOKEN_LEFT_BRACE
      case JSON_TOKEN_RIGHT_BRACE: { // Конец объекта
        __debug("JSON_TOKEN_RIGHT_BRACE");
        current_node = stack_object[stack_top_object--]; // Возврат к родительскому узлу
        if (stack_top_object >= 0) {
          current_node = stack_object[stack_top_object--];
        } else {
          __err("[BRACE] Stack underflow, invalid state.");
          return nullptr; // или другой подходящий способ обработки ошибки
        }
        break;
      } // END JSON_TOKEN_RIGHT_BRACE
      case JSON_TOKEN_LEFT_BRACKET: { // Начало массива
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
        break;
      } // END JSON_TOKEN_LEFT_BRACKET
      case JSON_TOKEN_RIGHT_BRACKET: { // Конец массива
        /*
        PHX_CORE_DEBUG("JSON_TOKEN_RIGHT_BRACKET");
        // current_node = stack[stack_top--];
        if (stack_top_array >= 0) {
          current_node = stack_array[stack_top_array--];
        } else {
          __err("[BRACKET] Stack underflow, invalid state.");
          return nullptr; // или другой подходящий способ обработки ошибки
        }
        */
        break;
      } // END JSON_TOKEN_RIGHT_BRACKET
      case JSON_TOKEN_STRING: { // Строковый токен
        __debug("JSON_TOKEN_STRING");

        if (current_node->type == JSON_AST_OBJECT && !current_key) {

          // Если это ключ для объекта
          if (current_key) free(current_key);

          current_key = strdup(current_token.value);

        } else if (current_node->type == JSON_AST_OBJECT && current_key) {

          // Если это значение для текущего ключа
          ast_node* string_node = ast_create_string(current_token.value);
          ast_node* pair_node = ast_create_pair(current_key, string_node);

          current_node->object_values = (ast_node**)realloc(
            current_node->object_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->object_values[current_node->value_count++] = pair_node;

          // free(current_key);
          if (current_key) {
            free(current_key);
          }

          current_key = nullptr;
        }/* else if (current_node->type == JSON_AST_ARRAY) {
          // Если текущий узел - это массив, добавляем строку в массив
          ast_node* string_node = ast_create_string(current_token.value);

          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = string_node;
        }*/

        break;
      } // END JSON_TOKEN_STRING
      case JSON_TOKEN_NUMBER: { // Числовой токен
        __debug("JSON_TOKEN_NUMBER");

        if (current_node->type == JSON_AST_OBJECT && current_key) {
          // Создаем числовой узел
          double number_value = strtod(current_token.value, nullptr);
          ast_node* number_node = ast_create_number(number_value);
          ast_node* pair_node = ast_create_pair(current_key, number_node);

          // Добавляем пару в текущий объект
          current_node->object_values = (ast_node**)realloc(
            current_node->object_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->object_values[current_node->value_count++] = pair_node;

          // free(current_key);
          if (current_key) {
            free(current_key);
          }

          current_key = nullptr;
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

        break;
      } // END JSON_TOKEN_NUMBER
      case JSON_TOKEN_TRUE:   // Булев токен "true"
      case JSON_TOKEN_FALSE: { // Булев токен "false"
        __debug("JSON_TOKEN_FALSE");
        if (current_node->type == JSON_AST_OBJECT && current_key) {
          int boolean_value = (current_token.type == JSON_TOKEN_TRUE) ? 1 : 0;
          ast_node* boolean_node = ast_create_boolean(boolean_value);
          ast_node* pair_node = ast_create_pair(current_key, boolean_node);

          current_node->object_values = (ast_node**)realloc(
            current_node->object_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->object_values[current_node->value_count++] = pair_node;

          // free(current_key);
          if (current_key) {
            free(current_key);
          }

          current_key = nullptr;
        }/* else if (current_node->type == JSON_AST_ARRAY) {
          // Если текущий узел - это массив, добавляем булевое значение в массив
          int boolean_value = (current_token.type == JSON_TOKEN_TRUE) ? 1 : 0;
          ast_node* boolean_node = ast_create_boolean(boolean_value);

          current_node->array_values = (ast_node**)realloc(
            current_node->array_values,
            (current_node->value_count + 1) * sizeof(ast_node*)
          );
          current_node->array_values[current_node->value_count++] = boolean_node;
        }*/

        break;
      } // END JSON_TOKEN_FALSE
      case JSON_TOKEN_COMMA: {
        __debug("JSON_TOKEN_COMMA");
        // Разделитель элементов, ничего делать не нужно
        break;
      }
      default: {
        __err("Unexpected token: %s", current_token.value);
        break;
      }
    }
  }

  // free(stack);
  free(stack_object);
  // free(stack_array);
  if (current_key) free(current_key);

  return root;
}



// ------------------------------------------------------



ast_result* get_value_from_ast(ast_node* root, const char* path) {
  if (!root || !path || root->type != JSON_AST_OBJECT) return nullptr;

  // Копируем путь, чтобы безопасно его модифицировать
  char* path_copy = strdup(path);
  if (!path_copy) return nullptr;

  char* token = strtok(path_copy, "->");
  ast_node* current_node = root;

  while (token) {
    bool key_found = false;

    // Ищем ключ в текущем объекте
    for (size_t i = 0; i < current_node->value_count; ++i) {
      json::ast_pair* pair = current_node->object_values[i]->pair_value;

      if (pair && strcmp(pair->key, token) == 0) {
        current_node = pair->value; // Переходим на следующий уровень
        key_found = true;
        break;
      }
    }

    if (!key_found) {
      free(path_copy);
      return nullptr; // Ключ не найден
    }

    // Если остались ещё части пути, проверяем, что текущий узел - объект
    token = strtok(nullptr, "->");
    if (token && (!current_node || current_node->type != JSON_AST_OBJECT)) {
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

} // namespace json
