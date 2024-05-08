#pragma once

//TODO: There is a cacheline def defaulting to 64, which is not ideal for ppc64le and s390x, check if we can vary that by platform
#include <pthread.h>

#ifdef __linux__
#include <numa.h>
//#include <linux/getcpu.h>
#endif

constexpr size_t CACHELINE_SIZE = 128;
