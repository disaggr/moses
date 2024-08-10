#pragma once

#include "place.h"
#include <cstddef>

namespace moses {

class MosesAllocator {
public:

    MosesAllocator(std::shared_ptr<Place> place, std::string identifier);
    ~MosesAllocator() = default;

    void* Allocate(std::size_t n);
    void Reserve(std::size_t bytes);
    void Deallocate(void* p, std::size_t n);

private:
    uint64_t RoundToNextPage(uint64_t alloc_size)
    {
        return (alloc_size + page_size - 1) & ~(page_size - 1);
    }
    std::size_t size;
    std::size_t capacity;
    void* current_base;
    std::shared_ptr<Place> _place;
    std::shared_ptr<PageManager> _page_manager;
    //TODO: Magic number
    uint64_t page_size = 65536U;
};

}