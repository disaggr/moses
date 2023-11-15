#pragma once

#include <vector>
#include <thread>
#include <pthread.h>
#include "arena.h"
#include "common.h"
#include "extent_hook_dispatch.h"
#include "extent_hook.h"

namespace moses {

class PlaceGuardStack {
	//TODO Try the 'tcache.flush' API before setting the custom hooks.
	//See https://github.com/jemalloc/jemalloc/issues/1016
	public:
		PlaceGuardStack();

		void Push(Arena *arena);
		void Push(Arena *arena, bool isolated);

		Arena* Pop();
	private:
		std::vector<Arena*> *arenas;
};

thread_local PlaceGuardStack *_pg_stack;

class PlaceGuard {
	public:
		PlaceGuard(Arena *arena);
		~PlaceGuard();

	private:
		void Initialize();
};

/*
class IsolatedPlaceGuard {
	//TODO: tcache disable or flush to make sure, no leakage is done in the thread caches
	//see https://github.com/jemalloc/jemalloc/issues/1016
	public:
		IsolatedPlaceGuard(const unsigned arena) {
			if(_pg_stack == nullptr) {
				Initialize();
			}
			_pg_stack->Push(arena, true);
		}

		~IsolatedPlaceGuard() {
			//TODO: Do we need a special Pop function for sec isolation?
			unsigned arena = _pg_stack->Pop();
		}
	
	private:
		void Initialize() {
			_pg_stack = new PlaceGuardStack();
		}
}; */

}