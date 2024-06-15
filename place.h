#pragma once

#include <string>
#include <map>
#include "arena_base.h"

namespace moses {

using core_index = uint64_t;

enum contention {
    LOW,
    MEDIUM,
    HIGH
};

//Things we want to support could be:
// Sealed Place:
// Create a place and let thread(s) write to it. Then seal it explicitely, turning all pages to read-only.
// Then we can communiate that to our monitor who uses the HCA to watch that there are no writes to the pages.
// The sealing could come externally, so that 

// Query Memory Access Tracing
// Once a query is started it gets its own set of pages through a special place.
// This could be easily achieved by having the HCA read out on begin and end of a query.
// while watching the whole address space. Well, we need to also have a dump of pages that were
// present while starting, every page that gets allocated and every page that gets deallocated.
// This needs to be implemented in special places that have the arenas trace the mmap and munmap calls.
// We need to define a way so that shared pages are sequentially accessed in case of multiquery.
// That could be achieved by memory protection keys.

/*  We create the arenas in a lazy way, meaning that upon association we check if we have an arena object already,
    if not only then we create a new one */

class Place {
	public:
		Place(std::string path, std::string name, contention arena_contention = contention::HIGH);
		std::string GetName();
        std::string GetPath();
        BaseArena* GetArena();
	private:
		std::string _path, _name;
        contention _arena_contention;
        std::map<core_index, BaseArena*> *_core_to_arena;
};
}