/*
 * Copyright (C) Yakiv Matiash
 */

#include "resolver.h"

namespace json {

void resolve_token(const token& token, handler_ctx& ctx, const token_resolver* resolvers, size_t resolver_count) {

  for (size_t i = 0; i < resolver_count; ++i) {
    if (resolvers[i].type == token.type) {
      resolvers[i].handler(ctx, token);
      return;
    }
  }
  
  __err("Unexpected token: %s", token.value);
}

} // namespace json

