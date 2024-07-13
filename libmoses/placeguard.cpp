#include "placeguard.h"
#include "extent_hook_dispatch.h"

#include <thread>
#include <pthread.h>

namespace moses {

thread_local PlaceGuardStack *_pg_stack;

PlaceGuardStack::PlaceGuardStack() {
	places = new std::vector<Place*>();
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
	places->pop_back();
	BaseArena *last_arena = places->back()->GetArena();
	unsigned arena = last_arena->GetId();
	size_t size = sizeof(unsigned);
	je_mallctl("tcache.flush", NULL, NULL, NULL, 0);
	je_mallctl("thread.arena", NULL, NULL, (void *) &arena, size);
	return;
}

Place* PlaceGuardStack::Top() {
	return places->back();
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