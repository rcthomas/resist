
#ifndef RESIST_CONFIG_H
#define RESIST_CONFIG_H

#include <stdlib.h>

#include "resist-types.h"

/* Application configuration type. */

struct resist_config_t {

    real_t spec_wl_min;     /* Bluest spectrum wavelength, AA.          */
    real_t spec_wl_max;     /* Reddest spectrum wavelength, AA.         */
    real_t spec_wl_step;    /* Default spectrum sampling, AA.           */

    real_t opac_wl_step;    /* Opacity bin width, Mm/s.                 */

    real_t max_vr;      /* Fastest ejecta velocity considered, Mm/s.    */
    real_t vr_step;     /* Ejecta velocity grid step, Mm/s.             */

    size_t mu_per_vr;   /* Angles per ejecta velocity grid point.       */

};

/* Use sensible values to initialize app configuration. */

void resist_config_init_default(struct resist_config_t** cfg);

/* Parse JSON object to initialize app configuration. */

void resist_config_init_json(struct resist_config_t** cfg);

/* Allocate app config, validate and assign parameters to it. */

void resist_config_init(struct resist_config_t** cfg,
                        real_t spec_wl_min,
                        real_t spec_wl_max,
                        real_t spec_wl_step,
                        real_t opac_wl_step,
                        real_t max_vr,
                        real_t vr_step,
                        size_t mu_per_vr);

/* Tear down app configuration object. */

void resist_config_free(struct resist_config_t* cfg);

/* Validate and assign parameters to an allocated app config. */

void _resist_config_init(struct resist_config_t* cfg,
                         real_t spec_wl_min,
                         real_t spec_wl_max,
                         real_t spec_wl_step,
                         real_t opac_wl_step,
                         real_t max_vr,
                         real_t vr_step,
                         size_t mu_per_vr);

#endif /* RESIST_CONFIG_H */
