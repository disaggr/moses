#pragma once

#include <pthread.h>
#include <jemalloc.h>
#include <numa.h>
#include <linux/getcpu.h>

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

class PlaceGuardStack {
	public:
		PlaceGuardStack() {
			(void) pthread_once(&key_once, Initialize());
			arenas = new std::vector<unsigned>();
			(void) pthread_setspecific(pg_stack_key, this);
			//TODO: Set default arena for this thread
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

class ExtentHookBase {
	public:
		ExtendHookBase(unsigned arena_id) {
			size_t sz = sizeof(extent_hooks_t);
			mallctl("arena.0.extent_hooks", (void *)&default_hooks, &sz, NULL, 0)
		}
		bool ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_ind) {
			default_hooks->alloc(default_hooks, new_addr, size, alignment, zero, commit);
		}

		bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_ind) {
			default_hooks->dalloc(default_hooks
		bool ExtentHookDestroy(extent_hook_t *extent_hooks, void *addr, size_t size,
			bool committed, unsigned arena_id)
		bool ExtentHookCommit(extent_hook_t *extent_hooks, void *addr, size_t size,
			size_t offset, size_t length, unsigned arena_id)
		bool ExtentHookDecommit(extent_hook_t *extent_hooks, void *addr, size_t size,
			size_t offset, size_t length, unsigned arena_id)
		bool ExtentHookPurge(extent_hook_t *extent_hooks, void *addr, size_t size,
			size_t offset, size_t length, unsigned arena_id)
		bool ExtentSplit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t size_a,
			size_t size_b, bool committed, unsigned arena_id)
		bool ExtentMerge(extent_hook_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
			size_t size_b, bool committed, unsigned arena_id)
	private:
		extent_hooks_t *default_hooks;
}

class ExtendHookNuma {
	public:
		ExtendHookNuma(unsigned node) : _node(node) { }
		bool ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_ind) {
			new_addr = numa_alloc_onnode(size, _node);
			return 
		}
	private:
		unsigned _node;
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

enum Place {
	fast,
	parallel,
	capacity,
	may_spill,
	explicit_place,
	dontcare
}

class Place {
	public:
		Place(std::string name, Place p) : _name(name), _place(place)
	private:
		std::string _name;
		Place _place;
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
			unsigned arena_ind;
			size_t unsigned_sz = sizeof(unsigned);
			extent_hooks_t hooks;
			size_t hooks_sz = sizeof(extent_hooks_t);
			mallctl("arenas.create", (void *) &arena_ind, sz, (void *) hooks, hooks_sz);	
		}

		unsigned GetArena(Places p) {
			switch(p) {
				case fast:
					unsigned *cpu, *node;
					int error = getcpu(cpu, node, NULL);
					if(error)
						exit(-1);
					ExtendHookNuma(*node);
					CreateArena
					break;
			}
		}
	private:
		Moses() {}
		Moses *_instance;
		std::map<Place, std::vector<unsigned>> arena_cache; 
}

}
