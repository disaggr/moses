#pragma once

#include <map>
#include "common.h"
#include "placeguard.h"
#include "extent_hook.h"

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

//TODO explicit_place
enum class Policy_Priority {
	Critical,
	Normal,
	May_Spill
};

enum class Policy_Frequency {
	High,
	Medium,
	Low
};

enum class Policy_Lifespan {
	Temporary,
	Long
};

enum class Policy_Granularity {
	Small,
	Large,
};

enum class Policy_Visibility {
	Single_Thread,
	Parallel
};

enum class Policy_Access {
	Read_Only,
	Write_Only,
	Read_Heavy,
	Write_Heavy,
	Mixed
};

enum class Policy_Pattern {
	Pointer_Chase,
	Random_Access,
	Linear_Stream,
	Mixed
};

class Place {
	public:
		Place(std::string name) : _name(name) {}
	private:
		std::string _name;
};

/*
class Moses {
	public:
		static Moses& GetInstance() {
			if(_instance == NULL) {
				_instance = new Moses();
			}
			return _instance;
		}

		void CreateArena(ExtentHookBase &custom_hooks) {
			unsigned arena_ind;
			size_t unsigned_sz = sizeof(unsigned);
			extent_hooks_t *hooks = _dispatch.GetDispatchHooks();
			ExtentHook *arena_extent_hooks = new ExtentHook();
			size_t hooks_sz = sizeof(extent_hooks_t);
			mallctl("arenas.create", (void *) &arena_ind, sz, (void *) hooks, hooks_sz);
			Arena *arena = new Arena(arena_ind, custom_hooks);
			arena_mapping[arena_ind] = arena;
		}

		unsigned GetArena(Place p) {
			switch(p) {
				case fast:
					unsigned *cpu, *node;
					int error = getcpu(cpu, node, NULL);
					if(error)
						exit(-1);
					ExtendHookNuma(*node);
					CreateArena();
					break;
			}
		}
	private:
		Moses() {
			//TODO check version, to make sure, extent hooks and meta data splitting is available
			//mallctl("version", ...

		}
		Moses *_instance;
		std::map<Place, std::vector<unsigned>> arena_cache;
}; */
}