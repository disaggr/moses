#include "arena.h"

namespace moses {

Arena::Arena(unsigned arena, ExtentHookBase *hook, int memory_protection_key)
    : _arena(arena), _memory_protection_key(memory_protection_key), _hook(hook) {
}

Arena::Arena(unsigned arena, ExtentHookBase *hook)
    : _arena(arena), _hook(hook) {

}
}