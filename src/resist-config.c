
#include <assert.h>

#include "resist-config.h"
#include "resist-memory.h"

void resist_config_init_default(struct resist_config_t** cfg)
{
    resist_config_init(cfg, 3000.0, 10000.0, 10.0, 0.3, 60.0, 0.1, 32);
}

void resist_config_init_json(struct resist_config_t** cfg)
{
    /* TODO */
}

void resist_config_init(struct resist_config_t** cfg,
                        real_t spec_wl_min,
                        real_t spec_wl_max,
                        real_t spec_wl_step,
                        real_t opac_wl_step,
                        real_t max_vr,
                        real_t vr_step,
                        size_t mu_per_vr)
{
    *cfg = (struct resist_config_t *)resist_malloc(
        sizeof(struct resist_config_t));
    _resist_config_init(*cfg, spec_wl_min, spec_wl_max, spec_wl_step,
                        opac_wl_step, max_vr, vr_step, mu_per_vr);
}

void resist_config_free(struct resist_config_t* cfg)
{
    resist_free(cfg);
}

void _resist_config_init(struct resist_config_t* cfg,
                         real_t spec_wl_min,
                         real_t spec_wl_max,
                         real_t spec_wl_step,
                         real_t opac_wl_step,
                         real_t max_vr,
                         real_t vr_step,
                         size_t mu_per_vr)
{

    /* Bluest spectrum wavelength should be positive. */

    assert(spec_wl_min > 0.0);
    cfg->spec_wl_min = spec_wl_min;

    /* Reddest spectrum wavelength should be greater than bluest. */

    assert(spec_wl_max > cfg->spec_wl_min);
    cfg->spec_wl_max = spec_wl_max;

    /* Default spectrum sampling should be positive. */

    assert(spec_wl_step > 0.0);
    cfg->spec_wl_step = spec_wl_step;

    /* Wavelength bin width should be positive. */

    assert(opac_wl_step > 0.0);
    cfg->opac_wl_step = opac_wl_step;

    /* Fastest ejecta velocity should be positive. */

    assert(max_vr > 0.0);
    cfg->max_vr = max_vr;

    /* Ejecta velocity grid step should be positive. */

    assert(vr_step > 0.0);
    cfg->vr_step = vr_step;

    /* Angles per ejecta velocity grid point should be positive. */

    assert(mu_per_vr > 0);
    cfg->mu_per_vr = mu_per_vr;

}
