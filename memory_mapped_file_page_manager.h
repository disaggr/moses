#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <mutex>

namespace moses {

class MemoryMappedFilePageManager {
public:
    MemoryMappedFilePageManager(const std::string& directory, const std::string& filename, size_t size);
    ~MemoryMappedFilePageManager();

    void* Allocate(size_t alloc_size);
    void Deallocate(void* start, size_t dealloc_size);
    void IncreaseSize(size_t additional_size);
private:

    void MergeFreeRegions() {
        std::sort(free_regions.begin(), free_regions.end(), [](const Region& a, const Region& b) {
            return a.start < b.start;
        });
        auto it = free_regions.begin();
        while (it != free_regions.end() - 1) {
            if (it->start + it->size == (it + 1)->start) {
                it->size += (it + 1)->size;
                free_regions.erase(it + 1);
            } else {
                ++it;
            }
        }
    }

    struct Region {
        size_t start;
        size_t size;
        bool operator==(const Region& other) const {
            return start == other.start && size == other.size;
        }
    };

    std::string filename;
    size_t size;
    int fd;
    void* mapped_file;
    std::vector<Region> free_regions;
    std::vector<std::pair<void*, size_t>> mapped_regions;
    std::mutex mtx;
};
}