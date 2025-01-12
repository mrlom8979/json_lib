/*
 * Copyright (C) Yakiv Matiash
 */

#include "parser.h"

namespace json {

void parser_init(parser *p, const char *input) {
  p->input = input;
  p->pos = 0;
  p->length = strlen(input);
}

static void parser_skip_whitespace(parser *p) {
  while (p->pos < p->length && isspace(p->input[p->pos])) {
    p->pos++;
  }
}
token parser_next_token(parser *p) {
  parser_skip_whitespace(p);
    
  if (p->pos >= p->length) {
    return (token){ JSON_TOKEN_END, NULL };
  }
    
  char c = p->input[p->pos];
  token token = { JSON_TOKEN_INVALID, NULL };

  if (c == '{') {
    token.type = JSON_TOKEN_LEFT_BRACE;
    token.value = NULL;
    p->pos++;
  } else if (c == '}') {
    token.type = JSON_TOKEN_RIGHT_BRACE;
    token.value = NULL;
    p->pos++;
  } else if (c == '[') {
    token.type = JSON_TOKEN_LEFT_BRACKET;
    token.value = NULL;
    p->pos++;
  } else if (c == ']') {
    token.type = JSON_TOKEN_RIGHT_BRACKET;
    token.value = NULL;
    p->pos++;
  } else if (c == ',') {
    // token.type = JSON_TOKEN_COMMA;
    // token.value = NULL;
    // parser->pos++;
    p->pos++;
    return parser_next_token(p);
  } else if (c == ':') {
    // token.type = JSON_TOKEN_COLON;
    // token.value = NULL;
    // parser->pos++;
    // Пропустить двоеточие
    p->pos++;
    return parser_next_token(p);
  } else if (c == '"') {
    token.type = JSON_TOKEN_STRING;
    size_t start = ++p->pos;
    while (p->pos < p->length && p->input[p->pos] != '"') {
      p->pos++;
    }
    size_t length = p->pos - start;
    token.value = (char *)malloc(length + 1);
    strncpy(token.value, &p->input[start], length);
    token.value[length] = '\0';
    p->pos++;  // пропускаем закрывающую кавычку
  } else if (isdigit(c) || c == '-') {
    token.type = JSON_TOKEN_NUMBER;
    size_t start = p->pos;
    while (p->pos < p->length && (isdigit(p->input[p->pos]) || p->input[p->pos] == '.')) {
      p->pos++;
    }
    size_t length = p->pos - start;
    token.value = (char *)malloc(length + 1);
    strncpy(token.value, &p->input[start], length);
    token.value[length] = '\0';
  } else if (strncmp(&p->input[p->pos], "true", 4) == 0) {
    token.type = JSON_TOKEN_TRUE;
    token.value = NULL;
    p->pos += 4;
  } else if (strncmp(&p->input[p->pos], "false", 5) == 0) {
    token.type = JSON_TOKEN_FALSE;
    token.value = NULL;
    p->pos += 5;
  } else if (strncmp(&p->input[p->pos], "null", 4) == 0) {
    token.type = JSON_TOKEN_NULL;
    token.value = NULL;
    p->pos += 4;
  } else if (c == '{') {
    token.type = JSON_TOKEN_OBJECT;
    token.value = NULL;
    p->pos++;
  } else if (c == '[') {
    token.type = JSON_TOKEN_ARRAY;
    token.value = NULL;
    p->pos++;
  } else {
    __err("Invalid JSON token encountered.");
  }
    
  return token;
}

void token_free(token *token) {
  if (token->value) {
    free(token->value);
  }
}

} // namespace json

