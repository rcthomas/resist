
#include <assert.h>
#include <stdio.h>

#include "resist-config.h"

void resist_config_init_default(struct resist_config_t** config) {
    resist_config_init(config, 
            10.0, 1.0e6, 0.3, 
            60.0, 0.1,
            32);
}

void resist_config_init_json(struct resist_config_t** config) {
    // TODO
}

void resist_config_init(struct resist_config_t** config,
    real_t wavelength_min, real_t wavelength_max, real_t wavelength_step,
    real_t velocity_max, real_t velocity_step,
    size_t angle_count) {

    *config = (struct resist_config_t*)malloc(sizeof(struct resist_config_t));

    _resist_config_init_wavelength_params(*config, 
            wavelength_min, wavelength_max, wavelength_step);

    _resist_config_init_velocity_params(*config,
            velocity_max, velocity_step);

    _resist_config_init_angle_params(*config,
            angle_count);

}

void resist_config_finalize(struct resist_config_t* config) {
    free(config);
    config = NULL;
}

void resist_config_output(struct resist_config_t* config) {
    /* TODO Make this a char buffer formatted with snprintf instead. */
    /* TODO Or do we bother with logging */
    printf("resist config\n");
    printf("    wavelength min=%f max=%f step=%f\n", 
            config->wavelength_min, config->wavelength_max, config->wavelength_step);
    printf("    velocity   max=%f step=%f\n", 
            config->velocity_max, config->velocity_step);
    printf("    angle      count=%zu\n", 
            config->angle_count);
}

void _resist_config_init_wavelength_params(struct resist_config_t* config, 
        real_t wavelength_min, real_t wavelength_max, real_t wavelength_step) {

    /* Bluest wavelength line loaded should be positive. */

    assert(wavelength_min > 0.0);
    config->wavelength_min = wavelength_min;

    /* Reddest wavelength line loaded should be greater than bluest. */

    assert(wavelength_max > config->wavelength_min);
    config->wavelength_max = wavelength_max;

    /* Wavelength bin width should be positive. */

    assert(wavelength_step > 0.0);
    config->wavelength_step = wavelength_step;

}

void _resist_config_init_velocity_params(struct resist_config_t* config, 
        real_t velocity_max, real_t velocity_step) {

    /* Fastest ejecta velocity should be positive. */

    assert(velocity_max > 0.0);
    config->velocity_max = velocity_max;

    /* Ejecta velocity grid step should be positive. */

    assert(velocity_step > 0.0);
    config->velocity_step = velocity_step;

}

void _resist_config_init_angle_params(struct resist_config_t* config,
        real_t angle_count) {

    /* Angles per ejecta velocity grid point should be positive. */

    assert(angle_count > 0);
    config->angle_count = angle_count;

}
