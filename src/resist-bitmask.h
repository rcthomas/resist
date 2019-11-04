
#ifndef RESIST_BITMASK_H
#define RESIST_BITMASK_H

#define resist_bitmask_chunk_t unsigned char

const size_t BITS_PER_CHUNK = CHAR_BIT * sizeof(resist_bitmask_chunk_t);

/* Bitmask type. */

struct resist_bitmask_t {
    size_t size;                /* Number of bits */
    size_t num_chunks;          /* Number of chunks */
    resist_bitmask_chunk_t* chunks;    /* Actual chunks */
};

/* Allocate bitmask of `size` bits and zero it out. */

void resist_bitmask_init_clear(struct resist_bitmask_t** mask,
                               size_t size);

/* Allocate bitmask of `size` bits. */

void resist_bitmask_init(struct resist_bitmask_t** mask,
                         size_t size);

/* Initialize and allocate bitmask chunks. */

static void _resist_bitmask_init(struct resist_bitmask_t* mask,
                                 size_t size);

/* Tear down bitmask. */

void resist_bitmask_free(struct resist_bitmask_t* mask);

/* Print out bitmask. */

void resist_bitmask_print(struct resist_bitmask_t* mask);

/* Zero out bitmask. */

void resist_bitmask_clear(struct resist_bitmask_t* mask);

/* Make bit at position `pos` be zero. */

void resist_bitmask_clear_bit(struct resist_bitmask_t* mask,
                              size_t pos);

/* Make bit at position `pos` be one. */

void resist_bitmask_set_bit(struct resist_bitmask_t* mask,
                            size_t pos);

/* True if bit at position `pos` is one, false if zero. */

bool resist_bitmask_test_bit(struct resist_bitmask_t* mask,
                             size_t pos);

#endif /* RESIST_BITMASK_H */
