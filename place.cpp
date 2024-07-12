#include "place.h"

#include "arena.h"

#if defined(numa) && defined(__linux__)
#include <sched.h> 
#else
int sched_getcpu(void) {
    return 0;
}
#endif
namespace moses{

Place::Place(std::string path, std::string name, contention arena_contention) 
    : _name(name), _path(path), _arena_contention(arena_contention) {
    _core_to_arena = new std::map<core_index, BaseArena*>();
    _page_managers = new std::vector<std::shared_ptr<MemoryMappedFilePageManager>>();
}

void Place::AddPageManager(std::shared_ptr<MemoryMappedFilePageManager> page_manager) {
    _page_managers->push_back(page_manager);
}

std::shared_ptr<MemoryMappedFilePageManager> Place::GetPageManager() {
    return _page_managers->at(0);
}

BaseArena* Place::GetArena() {
    int cpu = sched_getcpu();
    //get NUMA node
    //if contention is high, have an arena per logical core
    //if contention is medium, have an arena per (big) core
    //if contention is low, have an arena per NUMA node
    switch (_arena_contention)
    {
    case contention::HIGH :
        /* code */
        break;
    case contention::MEDIUM :
        /* code */
        break;
    case contention::LOW :  
        /* code */
        break;
    default:
        break;
    }
    if (_core_to_arena->find(cpu) == _core_to_arena->end()) {
        Arena *arena = new Arena(this);
        _core_to_arena->emplace(cpu, arena);
    }
    return _core_to_arena->at(cpu);
}

std::string Place::GetName() {
    return _name;
}

std::string Place::GetPath() {
    return _path;
}
}