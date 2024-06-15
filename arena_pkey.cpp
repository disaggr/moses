#include "arena_pkey.h"

namespace moses {

ProtectedArena::ProtectedArena(int memory_protection_key)
    : _memory_protection_key(memory_protection_key) {
}

}