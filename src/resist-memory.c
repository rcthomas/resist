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
    void* p;

#ifdef USE_HBM
    p = hbw_malloc(padded_size);
#else
    p = malloc(padded_size);
#endif

    assert(p);

    // Align the memory to the cache line boundary.
    while ((uintptr_t)p & (uintptr_t)(CACHE_LINE_SIZE - 1)) p++;

    return p;
}

void resist_free(void* p)
{

#ifdef USE_HBM
    hbw_free(p);
#else
    free(p);
#endif

}
