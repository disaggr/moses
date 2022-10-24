#pragma once

#include <pthread.h>
#include <jemalloc.h>

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

class PlaceGuardStack {
	public:
		PlaceGuardStack() {
			(void) pthread_once(&key_once, Initialize());
			arenas = new std::vector<unsigned>();
			(void) pthread_setspecific(pg_stack_key, this);
		}

		void Push(unsigned arena) {
			arenas->push(arena);
			size_t size = sizeof(arena);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
		}

		unsigned Pop() {
			unsigned arena = arenas->back();
			arenas->pop_back();
			return arena;
		}
	private:
		Initialize() {
			(void) pthread_key_create(&pg_stack_key, NULL);
		}
		std::vector<unsigned> arenas;
}

class PlaceGuard {
	public:
		PlaceGuard(const unsigned arena) {
			_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena);
		}

		~PlaceGuard() {
		       _pg_stack->pop();
	       }
	private:
		PlaceGuardStack *_pg_stack;
}

class IsolatedPlaceGuard {
}

class ExtendHookNuma {
	public:
		ExtendHookNuma(int node) : _node(node) { }
		alloc(
	private:
		int _node;
}

class ExtendHookFile {
	public:
		ExtendHookFile(std::string file_backing) : _file_backing(file_backing) {
		}
	private:
		std::string _file_backing;
}

class ExtendHookSpilling {
}

enum Places {
	local,
	remote,
	disaggregated,
	dontcare
}

class Moses {
	public:
		static Moses& GetInstance() {
			if(_instance == NULL) {
				_instance = new Moses();
			}
			return _instance;
		}

		void Initialize() {
			
		}

		unsigned GetArena(Places p) {

		}
	private:
		Moses() {}
		Moses *_instance;
		std::vector 
}

}
