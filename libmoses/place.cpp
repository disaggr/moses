#include "place.h"

#include "arena.h"

// FIXME: what is defined(numa)? isn't this always false?
#if /* defined(numa) && */ defined(__linux__)
#include <sched.h> 
#else
int sched_getcpu(void) {
    return 0;
}
#endif

namespace moses {

//void Place::AddPageManager(std::shared_ptr<MemoryMappedFilePageManager> page_manager) {
void Place::AddPageManager(std::shared_ptr<PageManager> page_manager) {
    // FIXME: what is a MemoryMappedFilePageManager?
    _page_managers.push_back(page_manager);
}

//std::shared_ptr<MemoryMappedFilePageManager> Place::GetPageManager() {
std::shared_ptr<PageManager> Place::GetPageManager() {
    // FIXME: why can we "add" page managers, but only access the first? what if it doesn't exist?
    return _page_managers.at(0);
}

BaseArena* Place::GetArena() {
    // for this place, produce the arena for the currently active cpu id, create if necessary
    // FIXME: why do we need an arena per CPU?
    
    // core?

    // cpu
    int cpu = sched_getcpu();

    // node?

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

    if (_core_to_arena.find(cpu) == _core_to_arena.end()) {
        fprintf(stderr, "arena.create: for cpu %d\n", cpu);
        Arena *arena = new Arena(this);
        _core_to_arena.emplace(cpu, arena);
    }

    return _core_to_arena.at(cpu);
}

}