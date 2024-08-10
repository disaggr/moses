#include "page_manager.h"
#include <filesystem>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include "log.h"

namespace moses
{

    PageManager::PageManager(const std::string &directory, const std::string &filename)
        : filename(filename), file_offset(0)
    {
        std::filesystem::create_directories(directory);
        std::string full_path = directory + "/" + filename;
        fd = open(full_path.c_str(), O_RDWR | O_CREAT, 0666);
        if (fd == -1)
        {
            throw std::runtime_error("Failed to open file");
        }
        LOG("pagemanager: New FileMapping for %s (%d)", full_path.c_str(), fd);
    }

    PageManager::~PageManager()
    {
        //Todo for every FileMapping do an munmap
        munmap(mapped_file, file_offset);
        close(fd);
    }

    void *PageManager::Allocate(size_t alloc_size, size_t alignment)
    {
        LOG("pagemanager: Allocating %#zx bytes in arena %s", alloc_size, filename.c_str());
        std::lock_guard<std::recursive_mutex> lock(mtx);
        uint64_t additional_size = RoundToNextPage(alloc_size);
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
            std::cout << "mmap failed: " << std::strerror(errno) << std::endl;
            throw std::runtime_error("Failed to map new part of file to memory");
        }
        LOG("pagemanager: New FileMapping for %d at %p", fd, new_mapped_region);
        //7FileMapping fmp{new_mapped_region, additional_size};
        //mapped_regions.push_back(fmp);
        file_offset = new_size;
        return new_mapped_region; 
    }

    void PageManager::Deallocate(void *start, uint64_t dealloc_size)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        LOG("pagemanager: Delete FileMapping for %d at %p and size %#zx", fd, start, dealloc_size);
        int ret = munmap(start, dealloc_size);
        if (ret == -1)
        {
            throw std::runtime_error("Failed to unmap part of memory");
        }
        /*ptrdiff_t offset = file_offset - reinterpret_cast<uint64_t>(start);
        ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE, offset, dealloc_size);
        if (ret == -1)
        {
            throw std::runtime_error("Failed to fallocate (punch a hole) part of the underlying file");
        }*/

        //uint64_t addr = (uint64_t) start;
        //FileMapping *fmp;
        //uint64_t offset;
    }
} // namespace moses