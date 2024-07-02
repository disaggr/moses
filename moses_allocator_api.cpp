#include <cstddef>

template <typename T>
class MyAllocator {
public:
    using value_type = T;

    MosesAllocator::MosesAllocator() noexcept = default;
    template <typename U>
    MyAllocator(const MyAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        // TODO: Implement memory allocation logic here
        return nullptr;
    }

    void deallocate(T* p, std::size_t n) noexcept {
        // TODO: Implement memory deallocation logic here
    }
};