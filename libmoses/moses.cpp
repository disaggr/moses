#include "moses.h"

#include "extent_hook_dispatch.h"

#include <jemalloc/jemalloc.h>

namespace moses {

std::map<Arena*, Place*> Moses::_arena_place_mapping = {};

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

void Moses::Initialize(std::map<std::string, Place> *initial_config) {
	//je_mallctl("opt.retain", NULL, NULL, NULL, 0);
	//je_mallctl("opt.narenas", NULL, NULL, NULL, 0); // we set this to 1 since we do arena management ourselves
	//je_mallctl("opt.percpu_arena", NULL, NULL, NULL, 0);
	for (auto& pair : *initial_config) {
		Place place = pair.second;
		place.AddPageManager(std::make_shared<MemoryMappedFilePageManager>(place.GetPath(), place.GetName(), 1024 * 1024 * 1024));
		//_place_tree.Insert(place);
		
	}
}

//Is an ExtentHook the same for a given place, from which arenas are spawned?

/*void Moses::CreateArena(Place *place) {
	unsigned arena_ind;
	size_t unsigned_sz = sizeof(unsigned);
	ExtentHook *arena_extent_hooks = new ExtentHook();
    extent_hooks_t *hooks = ExtentHookDispatch::GetDispatchHooks();
	ExtentHookDispatch::RegisterArena();
	size_t hooks_sz = sizeof(extent_hooks_t *);
	je_mallctl("arenas.create", (void *) &arena_ind, &unsigned_sz, (void *) &hooks, hooks_sz);
	char command[128];
	snprintf(command, sizeof(command), "arena.%u.extent_hooks", arena_ind);
	je_mallctl(command, nullptr, nullptr, static_cast<void*>(hooks), sizeof(extent_hooks_t*));
	Arena *arena = new Arena(arena_ind, arena_extent_hooks);
	_place_arena_mapping[arena_ind] = arena;
}*/

//Arena Moses::GetArena(Place p) {
	/*switch(p) {
		case fast:
			unsigned *cpu, *node;
			int error = getcpu(cpu, node, NULL);
			if(error)
				exit(-1);
			ExtendHookNuma(*node);
			CreateArena();
			break;
	}*/
//}
}