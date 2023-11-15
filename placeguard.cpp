#include "placeguard.h"

namespace moses {

PlaceGuardStack::PlaceGuardStack() {
	arenas = new std::vector<Arena*>();
	
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

void PlaceGuardStack::Push(Arena *arena) {
	Push(arena, false);
}

void PlaceGuardStack::Push(Arena *arena, bool isolated) {
	/* Use this for isolated arenas
    Arena *arena = arenas->back();
	if(isolated == false && arena_pair.second == true) {
		//Error this is forbidden
	}*/
	arenas->push_back(arena);
    unsigned arena_id = arena->GetId();
	size_t size = sizeof(unsigned);
	je_mallctl("thread.arena", NULL, NULL, (void *) &arena_id, size);
}

Arena* PlaceGuardStack::Pop() {
	Arena *last_arena = arenas->back();
	arenas->pop_back();
	unsigned arena = last_arena->GetId();
	size_t size = sizeof(unsigned);
	je_mallctl("thread.arena", NULL, NULL, (void *) &arena, size);
	return last_arena;
}

PlaceGuard::PlaceGuard(Arena *arena) {
	if(_pg_stack == nullptr) {
		Initialize();
	}
	_pg_stack->Push(arena);
}

PlaceGuard::~PlaceGuard() {
	Arena *arena = _pg_stack->Pop();
    // give arena back to moses?
}

void PlaceGuard::Initialize() {
	_pg_stack = new PlaceGuardStack();
}
}