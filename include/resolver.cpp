/*
 * Copyright (C) Yakiv Matiash
 */

#include "resolver.h"

namespace json {

void resolve_token(const token& token, handler_ctx& ctx, const token_resolver* resolvers, size_t resolver_count) {

  // size_t resolver_count = sizeof(resolvers) / sizeof(resolvers[0]);
  // __debug("%d", resolver_count);
  for (size_t i = 0; i < resolver_count; ++i) {
    __debug("%d", resolvers[i].type);
    if (resolvers[i].type == token.type) {
      resolvers[i].handler(ctx, token);
      return;
    }
  }
  
  // Если обработчик не найден, вызываем обработчик для неизвестного токена
  // __err("Unknown token.");
}

} // namespace json

