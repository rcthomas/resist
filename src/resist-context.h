
#ifndef RESIST_CONTEXT_H
#define RESIST_CONTEXT_H

#include "resist-config.h"
#include "resist-types.h"

/* Application context type. */

struct resist_context_t {

    real_t min_wl;          /* Bluest wavelength line loaded, AA.           */
    real_t max_wl;          /* Reddest wavelength line loaded, AA.          */
    real_t wl_step;         /* Wavelength bin width, Mm/s.                  */

    size_t wl_count;
    real_t* wl;

    real_t max_vr;          /* Fastest ejecta velocity considered, Mm/s.    */
    real_t vr_step;         /* Ejecta velocity grid step, Mm/s.             */

    size_t vr_count;
    real_t* vr;

    size_t mu_per_vr;       /* Angles per ejecta velocity grid point.       */

    size_t mu_count;
    real_t* mu;

};

/* Allocate and initialize app context from config. */

void resist_context_init(struct resist_context_t** ctx,
                         struct resist_config_t* cfg);

/* Tear down context. */

void resist_context_free(struct resist_context_t* ctx);

/* TODO: This should return snprintf'ed report or decide on logging. */

void resist_context_output(struct resist_context_t* ctx);

/* Validate and initialize an allocated app context. */

void _resist_context_init(struct resist_context_t* ctx,
                          struct resist_config_t* cfg);

#endif /* RESIST_CONTEXT_H */
