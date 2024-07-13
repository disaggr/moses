#include "memory_mapped_file_page_manager.h"
#include <filesystem>

namespace moses {

MemoryMappedFilePageManager::MemoryMappedFilePageManager(const std::string& directory, const std::string& filename, size_t size)
    : filename(filename), size(size) {
    std::filesystem::create_directories(directory);
    std::string full_path = directory + "/" + filename;
    fd = open(full_path.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }
    if (ftruncate(fd, size) == -1) {
        throw std::runtime_error("Failed to truncate file");
    }
    mapped_file = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_file == MAP_FAILED) {
        throw std::runtime_error("Failed to map file");
    }
    free_regions.push_back({0, size});
}

MemoryMappedFilePageManager::~MemoryMappedFilePageManager() {
    munmap(mapped_file, size);
    close(fd);
}

void* MemoryMappedFilePageManager::Allocate(size_t alloc_size) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& region : free_regions) {
        if (region.size >= alloc_size) {
            void* start = static_cast<char*>(mapped_file) + region.start;
            if (region.size == alloc_size) {
                free_regions.erase(std::find(free_regions.begin(), free_regions.end(), region));
            } else {
                region.start += alloc_size;
                region.size -= alloc_size;
            }
            return start;
        }
    }
    throw std::runtime_error("Failed to allocate memory");
}

void MemoryMappedFilePageManager::Deallocate(void* start, size_t dealloc_size) {
    std::lock_guard<std::mutex> lock(mtx);
    size_t offset = static_cast<char*>(start) - static_cast<char*>(mapped_file);
    free_regions.push_back({offset, dealloc_size});
    MergeFreeRegions();
}

void MemoryMappedFilePageManager::IncreaseSize(size_t additional_size) {
        std::lock_guard<std::mutex> lock(mtx);
        size_t new_size = size + additional_size;

        // Resize the file
        if (ftruncate(fd, new_size) == -1) {
            throw std::runtime_error("Failed to resize file");
        }

        // Map the new part of the file
        void* new_mapped_region = mmap(nullptr, additional_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, size);
        if (new_mapped_region == MAP_FAILED) {
            throw std::runtime_error("Failed to map new part of file to memory");
        }

        // Add the new region to the list of mapped regions
        mapped_regions.push_back({new_mapped_region, additional_size});

        // Update the free regions
        free_regions.push_back({size, additional_size});
        size = new_size;
    }

} // namespace moses