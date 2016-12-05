
#include <stdlib.h>

#include "resist-types.h"

/* Application configuration type. */

struct resist_config_t {

    real_t wavelength_min;  /* Bluest wavelength line loaded, AA.           */
    real_t wavelength_max;  /* Reddest wavelength line loaded, AA.          */
    real_t wavelength_step; /* Wavelength bin width, Mm/s.                  */

    real_t velocity_max;    /* Fastest ejecta velocity considered, Mm/s.    */
    real_t velocity_step;   /* Ejecta velocity grid step, Mm/s.             */

    size_t angle_count;     /* Angles per ejecta velocity grid point.       */

};

/* Use sensible values to initialize app configuration. */

void resist_config_init_default(struct resist_config_t** config);

/* Parse JSON object to initialize app configuration. */

void resist_config_init_json(struct resist_config_t** config);

/* Pass parameters to initialize app configuration. */

void resist_config_init(struct resist_config_t** config,
    real_t wavelength_min, real_t wavelength_max, real_t wavelength_step,
    real_t velocity_max, real_t velocity_step,
    size_t angle_count);

/* Tear down app configuration. */

void resist_config_finalize(struct resist_config_t* config);

/* TODO: This should return snprintf'ed report or decide on logging. */

void resist_config_output(struct resist_config_t* config);

/* Wavelength bin parameter initialization. */

void _resist_config_init_wavelength_params(struct resist_config_t* config, 
        real_t wavelength_min, real_t wavelength_max, real_t wavelength_step);

/* Ejecta velocity parameter initialization. */

void _resist_config_init_velocity_params(struct resist_config_t* config, 
        real_t velocity_max, real_t velocity_step);

/* Source function specific intensity angle parameter initialization. */

void _resist_config_init_angle_params(struct resist_config_t* config,
        real_t angle_count);
