
#ifndef RESIST_CONTEXT_H
#define RESIST_CONTEXT_H

#include "resist-config.h"
#include "resist-setup.h"
#include "resist-types.h"

/* Application context type. */

struct resist_context_t {

    real_t spec_wl_min;     /* Bluest spectrum wavelength, AA.              */
    real_t spec_wl_max;     /* Reddest spectrum wavelength, AA.             */
    real_t spec_wl_step;    /* Default spectrum sampling, AA.               */

    real_t opac_wl_min;     /* Bluest opacity bin, AA.                      */
    real_t opac_wl_max;     /* Reddest opacity bin, AA.                     */
    real_t opac_wl_step;    /* Opacity bin width, Mm/s.                     */
    size_t opac_wl_size;    /* Number of opacity bins.                      */
    real_t* opac_wl;        /* Opacity bins, blue edge, AA.                 */

    real_t max_vr;          /* Fastest ejecta velocity considered, Mm/s.    */
    real_t vr_step;         /* Ejecta velocity grid step, Mm/s.             */

    size_t vr_count;        /* Velocity grid size including ghost cells.    */
    size_t real_vr_count;   /* Non-ghost cell velocity grid size.           */
    real_t* vr;             /* Velocity grid, Mm/s.                         */

    size_t mu_per_vr;       /* Angles per ejecta velocity grid point.       */

    size_t mu_count;        /* Total number of angles.                      */
    real_t* mu;             /* Angle grid, cosines.                         */
    real_t* in;             /* Specific intensities.                        */

    real_t* tau;            /* Sobolev opacity bins.                        */
    real_t* src;            /* Source function.                             */

};

/* Allocate and initialize app context from config. */

void resist_context_init(struct resist_context_t** ctx,
                         struct resist_config_t* cfg);

/* Tear down context. */

void resist_context_free(struct resist_context_t* ctx);

/* Compute. */

void resist_context_compute(struct resist_context_t* ctx,
                            struct resist_setup_t* setup);

/* TODO: This should return snprintf'ed report or decide on logging. */

void resist_context_output(struct resist_context_t* ctx);

/* Validate and initialize an allocated app context. */

void _resist_context_init(struct resist_context_t* ctx,
                          struct resist_config_t* cfg);

#endif /* RESIST_CONTEXT_H */
