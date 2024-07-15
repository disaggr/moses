#pragma once

#include "arena_base.h"

namespace moses {

class ProtectedArena : public BaseArena {
    public:
        ProtectedArena(int memory_protection_key);

    private:
        int _memory_protection_key;
};

}
