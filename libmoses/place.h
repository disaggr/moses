#pragma once

#include "memory_mapped_file_page_manager.h"
#include "page_manager.h"
#include "arena_base.h"
#include "common.h"

#include <string>
#include <map>
#include <memory>
#include <atomic>

namespace moses {

// using a 64bit integer type for enumerating CPU cores.
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

        const std::string Path;
        const std::string Name;

        BaseArena* GetArena();

        //void AddPageManager(std::shared_ptr<MemoryMappedFilePageManager> page_manager);
        void AddPageManager(std::shared_ptr<PageManager> page_manager, std::string identifier);
        //std::shared_ptr<MemoryMappedFilePageManager> GetPageManager();
        std::shared_ptr<PageManager> GetPageManager(std::string identifier = "default");

    private:
        contention _arena_contention;

        struct alignas(CACHELINE_SIZE) CacheAlignedArenaMapping {
        union {
            std::atomic<BaseArena *> arena_ptr;
            char padding[CACHELINE_SIZE];
        };
        };
        CacheAlignedArenaMapping *_core_to_arena;
        //std::vector<std::shared_ptr<MemoryMappedFilePageManager>> _page_managers;
        std::map<std::string, std::shared_ptr<PageManager>> _page_managers;
        //std::map<uint64_t extent_address,
};

}
