#include "placeguard.h"

namespace moses {

PlaceGuardStack::PlaceGuardStack() {
	arenas = new std::vector<Arena>();
	
	unsigned current_arena;
	size_t arena_size = sizeof(unsigned);
	je_mallctl("thread.arena", (void *) &current_arena, &arena_size, NULL, 0);
	//check if we already have an arena object
    char command[64];
    snprintf(command, sizeof(command), "arena.%u.extent_hooks", current_arena);
	extent_hooks_t * extent_hooks = ExtentHookDispatch::GetDispatchHooks();
	je_mallctl(command, nullptr, nullptr, static_cast<void*>(extent_hooks), sizeof(extent_hooks_t*));
	ExtentHook *arena_extent_hooks = new ExtentHook();
    Arena *arena = new Arena(current_arena, arena_extent_hooks);
	Push(arena);
}

}