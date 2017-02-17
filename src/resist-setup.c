
#include <assert.h>
#include <stdlib.h>

#include "resist-memory.h"
#include "resist-setup.h"

void resist_setup_init_default(struct resist_setup_t** stp)
{
    resist_setup_init(stp);
}

void resist_setup_init(struct resist_setup_t** stp)
{
    *stp = (struct resist_setup_t *)resist_malloc(
        sizeof(struct resist_setup_t));
}

void resist_setup_free(struct resist_setup_t* stp)
{
    resist_free(stp);
}
