#pragma once

//TODO: There is a cacheline def defaulting to 64, which is not ideal for ppc64le and s390x, check if we can vary that by platform
#include <jemalloc/internal/jemalloc_internal_types.h>
#include <pthread.h>
#include <jemalloc/jemalloc.h>

#ifdef __linux__
#include <numa.h>
//#include <linux/getcpu.h>
#endif

constexpr size_t CACHELINE_SIZE = 128;
