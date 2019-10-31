
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resist-bitmask.h"

void resist_bitmask_init_clear(struct resist_bitmask_t** mask,
                               size_t size)
{
    resist_bitmask_init(mask, size);
    resist_bitmask_clear(*mask);
}

void resist_bitmask_init(struct resist_bitmask_t** mask,
                         size_t size)
{
    *mask = (struct resist_bitmask_t *)malloc(sizeof(struct resist_bitmask_t));
    _resist_bitmask_init(*mask, size);
}

void _resist_bitmask_init(struct resist_bitmask_t* mask,
                          size_t size)
{
    mask->size = size;
    mask->num_chunks = mask->size / BITS_PER_CHUNK + 1;
    mask->chunks =
        (resist_bitmask_chunk_t*)malloc(
            mask->num_chunks * sizeof(resist_bitmask_chunk_t));
}

void resist_bitmask_free(struct resist_bitmask_t* mask)
{
    free(mask->chunks);
    free(mask);
}

void resist_bitmask_print(struct resist_bitmask_t* mask)
{
    for (size_t i = 0; i < mask->size; i++) {
        printf("%d", resist_bitmask_test_bit(mask, i));
    }
    printf("\n");
}

void resist_bitmask_clear(struct resist_bitmask_t* mask)
{
    memset(mask->chunks, 0, mask->num_chunks * sizeof(resist_bitmask_chunk_t));
}

void resist_bitmask_clear_bit(struct resist_bitmask_t* mask,
                              size_t pos)
{
    size_t i = pos / BITS_PER_CHUNK;
    size_t j = pos % BITS_PER_CHUNK;
    mask->chunks[i] &= ~(1 << j);
}

void resist_bitmask_set_bit(struct resist_bitmask_t* mask,
                            size_t pos)
{
    size_t i = pos / BITS_PER_CHUNK;
    size_t j = pos % BITS_PER_CHUNK;
    mask->chunks[i] |= (1 << j);
}

bool resist_bitmask_test_bit(struct resist_bitmask_t* mask,
                             size_t pos)
{
    size_t i = pos / BITS_PER_CHUNK;
    size_t j = pos % BITS_PER_CHUNK;
    return mask->chunks[i] & (1 << j);
}
