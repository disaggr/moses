#include "log.h"
#include <stdlib.h>

int log_state = LOG_UNINITIALIZED;

int load_log_state() {
    char *res = getenv("MOSES_LOG");

    if (res) {
        return LOG_ENABLED;
    }

    return LOG_DISABLED;
}