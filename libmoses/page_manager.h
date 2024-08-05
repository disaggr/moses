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

class PageManager {
public:
    PageManager(const std::string& directory, const std::string& filename);
    ~PageManager();

    void* Allocate(uint64_t alloc_size);
    void Deallocate(void* start, uint64_t dealloc_size);
private:

    uint64_t RoundToNextPage(uint64_t alloc_size)
    {
        return (alloc_size + page_size - 1) & ~(page_size - 1);
    }

    struct FileMapping {
        void* start;
        uint64_t size;
        bool operator==(const FileMapping& other) const {
            return start == other.start && size == other.size;
        }
    };

    std::string filename;
    uint64_t file_offset;
    void *filepointer;
    int fd;
    void *mapped_file;
    std::vector<FileMapping> mapped_regions;
    std::recursive_mutex mtx;
    //TODO: Magic number
    uint64_t page_size = 65536U;
};
}