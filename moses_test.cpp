#include "moses.h"
#include "moses_allocator.h"

// Make memory management a cooperative task
// Give examples for:
// - Thread to arena pinning, e.g. place guards with priorities
// - Core to arena pinning, look up what jemalloc can do here
// - Program Execution Phase to arena pinning (instruction steram executed by a thread), e.g. place guards
// - Object to arena pinning (hardest and most expensive one)
// - Class to arena pinning (Factory(Place) pattern, because otherwise the memory get allocated outside of class definition)

std::map<std::string, moses::Place> places = {
    {"table", moses::Place("/mnt/moses/long_lived", "table", moses::contention::LOW)},
    {"temp", moses::Place("/mnt/moses/short_lived", "temp", moses::contention::HIGH)}};

void function_1()
{
    //268435457 overflows 1 GiB by 1 element
    std::vector<int> *huge_vector = new std::vector<int>(268435457U);
    printf("table vector: %p\n", huge_vector);
    printf("table vector data: %p\n", huge_vector->data());
    std::vector<int> *empty_vector = new std::vector<int>();
    printf("empty table vector: %p\n", empty_vector);
    printf("empty table vector data: %p\n", empty_vector->data());
    std::vector<int> stack_vector;
    printf("stack vector: %p\n", &stack_vector);
    moses::PlaceGuard guard(&places.at("temp"));
    //int *huge_array = (int *)malloc(100000 * sizeof(int));
    int *huge_array = new int[100000];
    int *small_array = new int[2];
    printf("temp array: %p\n", huge_array);
    printf("temp array: %p\n", small_array);
    for (int i = 0; i < 100000; i++)
    {
        huge_vector->at(i) = i;
        huge_array[i] = i;
    }
    free(huge_array);
    free(small_array);
    free(huge_vector);
    free(empty_vector);
}

void function_2()
{
    const size_t num_elements = 268435457;
    std::shared_ptr<moses::Place> place_ptr = std::make_shared<moses::Place>(places.at("table"));
    moses::MosesAllocator *_alloc = new moses::MosesAllocator(place_ptr, "table1");
    void *addr = _alloc->Allocate(num_elements * sizeof(int));
    int* int_ptr = static_cast<int*>(addr);
    std::vector<int> vec(int_ptr, int_ptr + num_elements);
    for (size_t i = 0; i < num_elements; ++i) {
        vec.at(i) = 2;  // Manually destroy each int
    }
}

int main(int argc, char *argv[])
{
    moses::Moses::Initialize(&places);
    moses::PlaceGuard guard(&places.at("table"));
    // Allocate all tables()
    function_1();
    function_2();
    pause();
    return 0;
}

// Could we use the explicit arena placeguards to debug and improve object placement
// What can we do with unwanted side-effects:
// Like: Thread 1 allocates vector 1 with arena 1
// Thread 1 or another Thread hits a PlaceGuard and is assigned to arena 2
// Now the vector allocates new memory because some internal thresholds of capacity are reached
// The whole vector is now in arena 2

// For this we could modify the implementation of the datastructures, so that whenever the constructor is called,
// the object gets an allocator object from the current placeguard stack and uses this allocator for all further allocations
// provide pseudo code for this szenario

// With moses we can specify during start of the application which Places are allocated on which nodes
// We could do this with bitmaps representing nodes where the memory could come from, with a home node for default allocations

// Object to arena mapping with placeguards and explicit malloc(x) calls
// Thread to arena mapping with placeguards and regular checks?

// Tradeoff between finegrain control (e.g. specifiy on every malloc call the arena) vs. coarsegrain implementation with placeguards.
// For the latter it may happen that some objects are unintentionally placed within an arena

// Can we find better names for the concept of place, placement, memory, memory pool?

// We could visualize the page access trace with additional allocation metadata, e.g. collect a stacktrace whenever malloc is called.
// Though this has several limitations:
// First, instead of malloc, a program could (for larger allocations) call directly mmap, a intercepting library would not notice
// Second, not neccessarily must an buffer or object be allocated at one time, but could consist of different allocations at different times.
// The question arises, how we would visualize this and deal with this situation.
// It could be that we dispaly the allocation stack traces as memory flamegraphs, then different allocations would be collapsed.
// Still, this is very fine grained and to get an overview we argue, that a more coars-grain view is better.
// This is why we suggest to group allocations into "memory arenas".
// A memory arena could be served from different memory source, depending on the use case: e.g. spilling over different NUMA nodes or explicit tiering.

// Idea for the tracefile format:
// Have a predefined threshold for access count groups. E.g.
// 0 /s - cold
// 1-1000 /s - luke warm
// 1001 - 10000 - warm
// >10000 - hot
// This could be encoded with 2 bits per entry and is probably sufficient.
// Because we sample the access counts anyways, we might think of the consequences here.