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
    MemoryMappedFilePageManager(const std::string& directory, const std::string& filename, uint64_t size);
    ~MemoryMappedFilePageManager();

    void* Allocate(uint64_t alloc_size);
    void Deallocate(void* start, uint64_t dealloc_size);
    void IncreaseSize(uint64_t additional_size);
private:

    uint64_t RoundToNextPage(uint64_t alloc_size)
    {
        return (alloc_size + page_size - 1) & ~(page_size - 1);
    }
    void MergeFreeRegions() {
        std::sort(free_regions.begin(), free_regions.end(), [](const Region& a, const Region& b) {
            return a.start < b.start;
        });
        auto it = free_regions.begin();
        while (it != free_regions.end() - 1) {
            if (it->start + it->size == (++it)->start) {
                it->size += (++it)->size;
                free_regions.erase(++it);
            } else {
                ++it;
            }
        }
    }

    void MergeFileMappings() {
        std::sort(mapped_regions.begin(), mapped_regions.end(), [](const FileMapping& a, const FileMapping& b) {
            return a.start < b.start;
        });
        auto it = mapped_regions.begin();
        while (it != mapped_regions.end() - 1) {
            auto next_it = std::next(it);
            if (reinterpret_cast<uintptr_t>(it->start) + it->size == reinterpret_cast<uintptr_t>(next_it->start)) {
                it->size += next_it->size;
                mapped_regions.erase(next_it);
            } else {
                ++it;
            }
        }
    }

    struct FileMapping {
        void* start;
        uint64_t size;
        bool operator==(const FileMapping& other) const {
            return start == other.start && size == other.size;
        }
    };

    struct Region {
        void* base_addr;
        uint64_t start;
        uint64_t size;
        FileMapping *fmp;
        bool operator==(const Region& other) const {
            return start == other.start && size == other.size && fmp == other.fmp;
        }
    };

    std::string filename;
    uint64_t file_offset;
    int fd;
    void* mapped_file;
    std::vector<Region> free_regions;
    std::vector<FileMapping> mapped_regions;
    std::recursive_mutex mtx;
    //TODO: Magic number
    uint64_t page_size = 65536U;
};
}