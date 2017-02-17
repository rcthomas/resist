#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef USE_HBM
#include <hbwmalloc.h>
#endif

// Wrapper which allocates "regular" or high-bandwidth memory, depending on
// what is available on the system. Allocating HBM requires the "memkind"
// library. This wrapper also aligns memory to cache line boundaries. On x86 a
// cache line is 64 bytes; as there is no way to determine this value at run
// time, we define it in a macro. We should put this parameter into the
// Makefile.

#define CACHE_LINE_SIZE 64

void* resist_malloc(size_t n)
{

    const size_t padded_size = n + CACHE_LINE_SIZE;
    void* p_base; // holds requested plus padded memory
    void* p; // points to aligned address in padded array

#ifdef USE_HBM
    p_base = hbw_malloc(padded_size * sizeof(void*));
#else
    p_base = malloc(padded_size * sizeof(void*));
#endif

    assert(p_base);

    // Align the memory to the cache line boundary.
    p =
        (void*)((uintptr_t)(p_base + CACHE_LINE_SIZE) &
                (~((uintptr_t)CACHE_LINE_SIZE - 1)));
    *((void**)p - 1) = p_base;

    return p;
}

void resist_free(void* p)
{

#ifdef USE_HBM
    hbw_free(*((void**)p - 1));
#else
    free(*((void**)p - 1));
#endif

    p = NULL;
}
