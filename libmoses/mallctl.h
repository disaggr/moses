#pragma once

#define MOSES_UNINITIALIZED 0
#define MOSES_DISABLED 1
#define MOSES_ENABLED 2

extern int moses_enabled;

int load_moses_state();

#define moses_mallctl(C, ...) do { \
    switch (moses_enabled) { \
      case MOSES_DISABLED: \
        break; \
      case MOSES_UNINITIALIZED: \
        moses_enabled = load_moses_state(); \
        if (moses_enabled != MOSES_ENABLED) \
          break; \
      /* else fall through */ \
      case MOSES_ENABLED: \
        int res = mallctl(C, __VA_ARGS__); \
        if (res != 0) { \
            fprintf(stderr, "%s:%i:mallctl:%s: %i - %s\n", __FILE__, __LINE__, C, res, strerror(res)); \
        break; \
    } \
} } while (0)
