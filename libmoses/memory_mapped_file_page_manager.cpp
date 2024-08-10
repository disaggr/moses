#include "memory_mapped_file_page_manager.h"
#include <filesystem>
#include "log.h"

namespace moses
{

    MemoryMappedFilePageManager::MemoryMappedFilePageManager(const std::string &directory, const std::string &filename, size_t size)
        : filename(filename), file_offset(size)
    {
        std::filesystem::create_directories(directory);
        std::string full_path = directory + "/" + filename;
        fd = open(full_path.c_str(), O_RDWR | O_CREAT, 0666);
        if (fd == -1)
        {
            throw std::runtime_error("Failed to open file");
        }
        if (ftruncate(fd, size) == -1)
        {
            throw std::runtime_error("Failed to truncate file");
        }
        mapped_file = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mapped_file == MAP_FAILED)
        {
            throw std::runtime_error("Failed to map file");
        }
        LOG("memorymappedfilemanager: New FileMapping for %s (%d) at %p", full_path.c_str(), fd, mapped_file);
        FileMapping fmp{mapped_file, size};
        mapped_regions.push_back(fmp);
        auto it = mapped_regions.end();
        Region reg = {mapped_file, 0, size, &(*(--it))};
        free_regions.push_back(reg);
    }

    MemoryMappedFilePageManager::~MemoryMappedFilePageManager()
    {
        //Todo for every FileMapping do an munmap
        munmap(mapped_file, file_offset);
        close(fd);
    }

    void *MemoryMappedFilePageManager::Allocate(size_t alloc_size, size_t alignment)
    {
        LOG("Allocating %#zx bytes in arena %s", alloc_size, filename.c_str());
        std::lock_guard<std::recursive_mutex> lock(mtx);
        auto it = free_regions.begin();
        for (;it != free_regions.end(); ++it)
        {
            if (it->size >= alloc_size)
                break;
        }
        Region &region = *it;
        if(it == free_regions.end())
        {
            IncreaseSize(alloc_size);
            it = free_regions.end();
            region = *(--it);
        }

        void *offset = static_cast<char*>(region.fmp->start) + region.start;
        if (region.size == alloc_size)
        {
            free_regions.erase(std::find(free_regions.begin(), free_regions.end(), region));
        }
        else
        {
            region.start += alloc_size;
            region.size -= alloc_size;
        }

        return offset; 
    }



    void MemoryMappedFilePageManager::Deallocate(void *start, uint64_t dealloc_size)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        uint64_t addr = (uint64_t) start;
        FileMapping *fmp;
        uint64_t offset;
        for(Region& region : free_regions)
        {
            if(addr > reinterpret_cast<uint64_t>(region.fmp->start) && addr < (reinterpret_cast<uint64_t>(region.fmp->start) + region.fmp->size))
            {
                offset = addr - reinterpret_cast<uint64_t>(region.fmp->start);
                fmp = region.fmp;
                break;
                
            }
        }
        //uint64_t offset = static_cast<char *>(start) - static_cast<char *>(mapped_file);
        free_regions.push_back({fmp->start, offset, dealloc_size, fmp});
        MergeFreeRegions();
    }

    void MemoryMappedFilePageManager::IncreaseSize(uint64_t additional_size)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        additional_size = RoundToNextPage(additional_size);
        uint64_t new_size = file_offset + additional_size;

        // Resize the file
        if (ftruncate(fd, new_size) == -1)
        {
            throw std::runtime_error("Failed to resize file");
        }
        // Map the new part of the file
        void *new_mapped_region = mmap(nullptr, additional_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, file_offset);
        if (new_mapped_region == MAP_FAILED)
        {
            //throw std::runtime_error("Failed to map new part (Offset: " << std::hex << size << " size: " << std::hex << additional_size << "of file to memory ");
            throw std::runtime_error("Failed to map new part of file to memory");
        }
        LOG("memorymappedfilemanager: New FileMapping for %d at %p", fd, new_mapped_region);
        FileMapping fmp{new_mapped_region, additional_size};
        mapped_regions.push_back(fmp);
        auto it = mapped_regions.end();
        Region reg = {new_mapped_region, file_offset, additional_size, &(*(--it))};
        free_regions.push_back(reg);
        MergeFileMappings();
        file_offset = new_size;
    }

} // namespace moses