#pragma once

#define LOG_UNINITIALIZED 0
#define LOG_DISABLED 1
#define LOG_ENABLED 2

extern int log_state;

int load_log_state();

#define LOG(msg, ...) \
  do { \
    switch (log_state) { \
      case LOG_DISABLED: \
        break; \
      case LOG_UNINITIALIZED: \
        log_state = load_log_state(); \
        if (log_state != LOG_ENABLED) \
          break; \
      /* else fall through */ \
      case LOG_ENABLED: \
        fprintf(stderr, msg, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        break; \
    } \
  } while (0)
