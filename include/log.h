/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"

typedef enum {
  JSON_LOG_DEBUG,
  JSON_LOG_INFO,
  JSON_LOG_WARNING,
  JSON_LOG_ERROR
} log_level;

void __log(log_level lvl, const char *format, ...);
void __debug(const char* msg, ...);
void __info(const char* msg, ...);
void __warn(const char* msg, ...);
void __err(const char* msg, ...);
