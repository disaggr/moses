#include "placeguard.h"

#include <thread>
#include <pthread.h>
#include "extent_hook_dispatch.h"

namespace moses {

thread_local PlaceGuardStack *_pg_stack;

PlaceGuardStack::PlaceGuardStack() {
	places = new std::vector<Place*>();
	
	
	/*unsigned current_arena;
	size_t arena_size = sizeof(unsigned);
	je_mallctl("thread.arena", (void *) &current_arena, &arena_size, NULL, 0);
	//check if we already have an arena object
    
	char command[64];
    snprintf(command, sizeof(command), "arena.%u.extent_hooks", current_arena);
	extent_hooks_t * extent_hooks = ExtentHookDispatch::GetDispatchHooks();
	je_mallctl(command, nullptr, nullptr, static_cast<void*>(extent_hooks), sizeof(extent_hooks_t*));
	
	ExtentHook *arena_extent_hooks = new ExtentHook();
    Arena *arena = new Arena(current_arena, arena_extent_hooks);
	Push(arena);*/
}

void PlaceGuardStack::Push(Place *place) {
	//check for recursion
	if (!places->empty() && places->back() == place) {
		return;
	}
	places->push_back(place);
    unsigned arena_id = place->GetArena()->GetId();
	size_t size = sizeof(unsigned);
	je_mallctl("tcache.flush", NULL, NULL, NULL, 0);
	je_mallctl("thread.arena", NULL, NULL, (void *) &arena_id, size);
}

void PlaceGuardStack::Pop() {
	if (places->size() == 1) {
		return;
	}
	BaseArena *last_arena = places->back()->GetArena();
	places->pop_back();
	unsigned arena = last_arena->GetId();
	size_t size = sizeof(unsigned);
	je_mallctl("tcache.flush", NULL, NULL, NULL, 0);
	je_mallctl("thread.arena", NULL, NULL, (void *) &arena, size);
	return;
}

PlaceGuard::PlaceGuard(Place *place) {
	if(_pg_stack == nullptr) {
		Initialize();
	}
	_pg_stack->Push(place);
}

PlaceGuard::~PlaceGuard() {
	_pg_stack->Pop();
}

void PlaceGuard::Initialize() {
	_pg_stack = new PlaceGuardStack();
}
}