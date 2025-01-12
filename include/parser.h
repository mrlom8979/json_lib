/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "log.h"

namespace json {

typedef enum {
  JSON_TOKEN_LEFT_BRACE,   // {
  JSON_TOKEN_RIGHT_BRACE,  // }
  JSON_TOKEN_LEFT_BRACKET, // [
  JSON_TOKEN_RIGHT_BRACKET,// ]
  JSON_TOKEN_COMMA,        // ,
  JSON_TOKEN_COLON,        // :
  JSON_TOKEN_STRING,       // "строка"
  JSON_TOKEN_NUMBER,       // число
  JSON_TOKEN_TRUE,         // true
  JSON_TOKEN_FALSE,        // false
  JSON_TOKEN_NULL,         // null
  JSON_TOKEN_OBJECT,       // объект {}
  JSON_TOKEN_ARRAY,        // массив []
  JSON_TOKEN_END,          // конец данных
  JSON_TOKEN_INVALID       // неправильный токен
} token_type;

// Структура токена
typedef struct {
    token_type type;
    char *value;
} token;

// Парсер
typedef struct {
    const char *input;  // исходный JSON
    size_t pos;         // текущая позиция в строке
    size_t length;      // длина строки
} parser;

void parser_init(parser *p, const char *input);
token parser_next_token(parser *p);
void token_free(token *token);

}

