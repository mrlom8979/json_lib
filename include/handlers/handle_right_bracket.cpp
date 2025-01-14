/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_right_bracket(handler_ctx& ctx, const token& t) {
  __debug("JSON_TOKEN_RIGHT_BRACKET");
  if (ctx.stack_top_array >= 0) {
    ctx.current_node = ctx.stack_array[ctx.stack_top_array--];
  } else {
    __err("[BRACKET] Stack underflow, invalid state.");
    return; // или другой подходящий способ обработки ошибки
  }
}

} // namespace json

