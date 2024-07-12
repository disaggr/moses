#include "moses_allocator.h"
#include <cstddef>

namespace moses {

template <typename T>
MosesAllocator<T>::MosesAllocator(Place *place) : _place(place) {
}

template <typename T>
T* MosesAllocator<T>::allocate(std::size_t n) {
    // TODO: Implement memory allocation logic here
}

template <typename T>
void MosesAllocator<T>::deallocate(T* p, std::size_t n) {
    // TODO: Implement memory deallocation logic here
}
}