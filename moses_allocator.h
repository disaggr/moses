#pragma once

#include <cstddef>

namespace moses {

class MosesAllocator {
public:
    MosesAllocator(Place *place);
    ~MosesAllocator();

    void* allocate(std::size_t size);
    void deallocate(void* ptr);
};

}