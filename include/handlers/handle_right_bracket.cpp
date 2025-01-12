/*
 * Copyright (C) Yakiv Matiash
 */

#include "handlers.h"

namespace json {

void handle_right_bracket(handler_ctx& ctx, const token& t) {

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

}

} // namespace json

