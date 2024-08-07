#include "mallctl.h"
#include <stdlib.h>

int moses_enabled = MOSES_UNINITIALIZED;

int load_moses_state() {
    char *res = getenv("MOSES_ENABLED");

    if (res) {
        return MOSES_ENABLED;
    }

    return MOSES_DISABLED;
}