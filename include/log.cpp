/*
 * Copyright (C) Yakiv Matiash
 */

#include "log.h"

void __log(log_level lvl, const char *format, ...) {
  // Получение текущего времени
  time_t t = time(NULL);
  struct tm *tm_info = localtime(&t);

  // Буфер для времени
  char time_buffer[20];
  strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", tm_info);

  // Вывод префикса в зависимости от уровня
  switch (lvl) {
    case JSON_LOG_DEBUG:
      printf("[%s] [DEBUG]: ", time_buffer);
      break;
    case JSON_LOG_INFO:
      printf("[%s] [INFO]: ", time_buffer);
      break;
    case JSON_LOG_WARNING:
      printf("[%s] [WARNING]: ", time_buffer);
      break;
    case JSON_LOG_ERROR:
      printf("[%s] [ERROR]: ", time_buffer);
      break;
    default:
      printf("[%s] [UNKNOWN]: ", time_buffer);
  }

  // Обработка аргументов переменной длины
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  // Перевод строки
  printf("\n");
}

void __debug(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  __log(log_level::JSON_LOG_DEBUG, msg, args);
  va_end(args);
}

void __info(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  __log(log_level::JSON_LOG_INFO, msg, args);
  va_end(args);
}

void __warn(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  __log(log_level::JSON_LOG_WARNING, msg, args);
  va_end(args);
}

void __err(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  __log(log_level::JSON_LOG_ERROR, msg, args);
  va_end(args);
}

