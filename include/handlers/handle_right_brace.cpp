/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_right_brace(handler_ctx& ctx, const token& t) {

  __debug("JSON_TOKEN_RIGHT_BRACE");
  // ctx.current_node = ctx.stack_object[ctx.stack_top_object--]; // Возврат к родительскому узлу
  if (ctx.stack_top_object >= 0) {
    ctx.current_node = ctx.stack_object[ctx.stack_top_object--];
  } else {
    __err("[BRACE] Stack underflow, invalid state.");
    return; // или другой подходящий способ обработки ошибки
  }

}

} // namespace json

