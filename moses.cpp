#include "moses.h"

#include "extent_hook_named.h"

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

Place::Place(std::string name) : _name(name) {}

std::string Place::GetName() {
	return _name;
}

void Moses::Initialize(std::map<std::string, Place> *initial_config) {
    //TODO check version, to make sure, extent hooks and meta data splitting is available
	//mallctl("version", ...
    for (const auto& pair : *initial_config) {
        const Place place = pair.second;
        if (_place_arena_mapping.find(place) == _place_arena_mapping.end()) {
            CreateArena(&place);
        }
    }
}

//Is an ExtentHook the same for a given place, from which arenas are spawned?
void Moses::CreateArena(Place *place) {
	unsigned arena_ind;
	size_t unsigned_sz = sizeof(unsigned);
	ExtentHookNamed *arena_extent_hooks = new ExtentHookNamed(place->GetName());
    extent_hooks_t *hooks = ExtentHookDispatch::GetDispatchHooks();
	size_t hooks_sz = sizeof(extent_hooks_t);
	mallctl("arenas.create", (void *) &arena_ind, unsigned_sz, (void *) hooks, hooks_sz);
	Arena *arena = new Arena(arena_ind, arena_extent_hooks);
	_place_arena_mapping[arena_ind] = new std::vector<Arena*>(arena);
}

Arena Moses::GetArena(Place p) {
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
}
}