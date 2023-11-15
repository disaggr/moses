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

		void Push(Arena &arena) {
			Push(arena, false);
		}

		void Push(Arena &arena, bool isolated) {
			std::pair<unsigned,bool> arena_pair = arenas->back();
			if(isolated == false && arena_pair.second == true) {
				//Error this is forbidden
			}
			arenas->push_back(arena);
			size_t size = sizeof(unsigned);
			je_mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
		}

		unsigned Pop() {
			unsigned last_arena = arenas->back();
			arenas->pop_back();
			unsigned arena = arenas->back();
			size_t size = sizeof(unsigned);
			je_mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
			return last_arena;
		}
	private:
		std::vector<Arena> *arenas;
}

thread_local PlaceGuardStack *_pg_stack;

class PlaceGuard {
	public:
		PlaceGuard(const unsigned arena) {
			if(_pg_stack == nullptr) {
				Initialize();
			}
			_pg_stack->Push(arena);
		}

		~PlaceGuard() {
			unsigned arena = _pg_stack->Pop();
			// give arena back to moses?
	    }

	private:
		Initialize() {
			_pg_stack = new PlaceGuardStack();
		}
};

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
		Initialize() {
			_pg_stack = new PlaceGuardStack();
		}
};

}