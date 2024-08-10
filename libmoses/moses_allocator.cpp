#include "moses_allocator.h"
#include <cstddef>

namespace moses {

MosesAllocator::MosesAllocator(std::shared_ptr<Place> place, std::string name) : _place(place) {
    _page_manager = std::make_shared<PageManager>(place->Path, name);
    size = 0;
    capacity = 0;
    current_base = nullptr;
}

void MosesAllocator::Reserve(std::size_t n) {
    current_base = _page_manager->Allocate(n);
    size = n;
    capacity = n;
}

void* MosesAllocator::Allocate(std::size_t n) {
    void *addr;
    if(n < capacity)
    {
        capacity -= n;
        addr = current_base;
        current_base = static_cast<char*>(current_base) + n;
    }
    else
    {
        current_base = _page_manager->Allocate(n);
        size = RoundToNextPage(n);
        capacity = size - n;
        addr = current_base;
        current_base = static_cast<char*>(current_base) + n;
    }
    return addr;
}

void MosesAllocator::Deallocate(void* p, std::size_t n) {
    // TODO: Implement memory deallocation logic here
}
}