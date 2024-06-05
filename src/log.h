#ifndef LOG_H
#define LOG_H

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"

#define LOG_NL printf("\n")

#define LOG_RAW(...)                                                           \
  printf(COLOR_GREEN __VA_ARGS__);                                             \
  fflush(stdout)

#define LOG(...)                                                               \
  printf(COLOR_GREEN "[LOG]: "__VA_ARGS__);                                    \
  LOG_NL
#define LOG_ERR(...)                                                           \
  fprintf(stderr, COLOR_RED "[ERR]: " __VA_ARGS__);                             \
  LOG_NL

#define PANIC(...)                                                             \
  LOG_ERR(__VA_ARGS__);                                                        \
  exit(-1)

#endif /* LOG_H */
