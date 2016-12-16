
#include <assert.h>

#include "resist-config.h"
#include "resist-memory.h"

void resist_config_init_default(struct resist_config_t** cfg)
{
    resist_config_init(cfg, 10.0, 1.0e6, 0.3, 60.0, 0.1, 32);
}

void resist_config_init_json(struct resist_config_t** cfg)
{
    /* TODO */
}

void resist_config_init(struct resist_config_t** cfg,
                        real_t min_wl,
                        real_t max_wl,
                        real_t wl_step,
                        real_t max_vr,
                        real_t vr_step,
                        size_t mu_per_vr)
{
    *cfg = (struct resist_config_t *)resist_malloc(
        sizeof(struct resist_config_t));
    _resist_config_init(*cfg, min_wl, max_wl, wl_step, max_vr, vr_step,
                        mu_per_vr);
}

void resist_config_free(struct resist_config_t* cfg)
{
    resist_free(cfg);
    cfg = NULL;
}

void _resist_config_init(struct resist_config_t* cfg,
                         real_t min_wl,
                         real_t max_wl,
                         real_t wl_step,
                         real_t max_vr,
                         real_t vr_step,
                         size_t mu_per_vr)
{

    /* Bluest wavelength line loaded should be positive. */

    assert(min_wl > 0.0);
    cfg->min_wl = min_wl;

    /* Reddest wavelength line loaded should be greater than bluest. */

    assert(max_wl > cfg->min_wl);
    cfg->max_wl = max_wl;

    /* Wavelength bin width should be positive. */

    assert(wl_step > 0.0);
    cfg->wl_step = wl_step;

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
