#pragma once

#include "place.h"
#include <cstddef>

namespace moses {

template <typename T>
class MosesAllocator {
public:
    using value_type = T;

    MosesAllocator(Place *place, std::string identifier);
    ~MosesAllocator();

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);

private:
    Place *_place;

};

}