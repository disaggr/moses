#pragma once

namespace moses {

class PlaceGuardStack {
	//TODO Try the 'tcache.flush' API before setting the custom hooks.
	//See https://github.com/jemalloc/jemalloc/issues/1016
	public:
		PlaceGuardStack() {
			(void) pthread_once(&key_once, Initialize());
			arenas = new std::vector<unsigned>();
			(void) pthread_setspecific(pg_stack_key, this);
			unsigned current_arena;
			size_t arena_size = sizeof(unsigned);
			mallctl("thread.arena", (void *) &current_arena, &size, NULL, NULL);
			Push(current_arena);
		}

		void Push(unsigned arena) {
			Push(arena, false);
		}

		void Push(unsigned arena, bool isolated) {
			std::pair<unsigned,bool> arena_pair = arenas->back();
			if(isolated == false && arena_pair.second == true) {
				//Error this is forbidden
			}
			arenas->push(std::make_pair(arena, isolated));
			size_t size = sizeof(unsigned);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
		}

		unsigned Pop() {
			unsigned last_arena = arenas->back();
			arenas->pop_back();
			unsigned arena = arenas->back();
			size_t size = sizeof(unsigned);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
			return last_arena;
		}
	private:
		Initialize() {
			(void) pthread_key_create(&pg_stack_key, NULL);
		}
		std::vector<std::pair<unsigned,bool>> arenas;
}

class PlaceGuard {
	public:
		PlaceGuard(const unsigned arena) {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena);
		}

		~PlaceGuard() {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			unsigned arena = _pg_stack->pop();
			// give arena back to moses?
	       }
}

class IsolatedPlaceGuard {
	//TODO: tcache disable or flush to make sure, no leakage is done in the thread caches
	//see https://github.com/jemalloc/jemalloc/issues/1016
	public:
		IsolatedPlaceGuard(const unsigned arena) {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena, true);
		}

		~IsolatedPlaceGuard() {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			unsigned arena = _pg_stack->pop();
		}
}

}