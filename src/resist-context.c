
#include <stdio.h>
#include <tgmath.h>

#include "resist-config.h"
#include "resist-constants.h"
#include "resist-context.h"
#include "resist-memory.h"

void resist_context_init(struct resist_context_t** ctx,
                         struct resist_config_t* cfg)
{
    *ctx =
        (struct resist_context_t*)resist_malloc(sizeof(struct resist_context_t));
    _resist_context_init(*ctx, cfg);
}

void resist_context_free(struct resist_context_t* ctx)
{
    resist_free(ctx->src);
    resist_free(ctx->tau);
    resist_free(ctx->in);
    resist_free(ctx->mu);
    resist_free(ctx->vr);
    resist_free(ctx->opac_wl);
    resist_free(ctx);
}

void resist_context_compute(struct resist_context_t* ctx,
                            struct resist_setup_t* stp)
{
}

void resist_context_output(struct resist_context_t* ctx)
{

    /* TODO Make this a char buffer formatted with snprintf instead. */
    /* TODO Or do we bother with logging */
    printf("resist context\n");
    printf(
        "    spec_wl_min = %10.2e  spec_wl_max  = %10.2e  spec_wl_step = %10f\n",
        ctx->spec_wl_min, ctx->spec_wl_max, ctx->spec_wl_step);
    printf("    max_vr    = %10f  vr_step = %10f\n", ctx->max_vr, ctx->vr_step);
    printf("    mu_per_vr = %10zu\n", ctx->mu_per_vr);
    printf("    opac_wl_size = %10zu\n", ctx->opac_wl_size);
    printf("    vr_count  = %10zu  real_vr_count = %10zu\n", ctx->vr_count,
           ctx->real_vr_count);
    printf("    mu_count  = %10zu\n", ctx->mu_count);

}

void _resist_context_init(struct resist_context_t* ctx,
                          struct resist_config_t* cfg)
{

    /* Copy spectrum parameters from config. */

    ctx->spec_wl_min = cfg->spec_wl_min;
    ctx->spec_wl_max = cfg->spec_wl_max;
    ctx->spec_wl_step = cfg->spec_wl_step;

    /* Copy opacity parameters from config. */

    ctx->opac_wl_step = cfg->opac_wl_step;

    /* Copy velocity parameters from config directly. */

    ctx->max_vr = cfg->max_vr;
    ctx->vr_step = cfg->vr_step;

    /* Compute bounds for opacity bins.  Opacity bins start blueward of the
       bluest spectrum wavelength allowed and stop redward of the reddest
       spectrum wavelength allowed.  Blue limit is determined by a Doppler
       shift corresponding to (at least) 2 * max_vr, red limit is determined
       by a Doppler shift corresponding to just max_vr. */

    ctx->opac_wl_min = ctx->spec_wl_min / (1.0 + 2.0 * ctx->max_vr / c_Mms);
    ctx->opac_wl_max = ctx->spec_wl_max * (1.0 + ctx->max_vr / c_Mms);

    /* Compute number of opacity bins based on opacity bin bounds. */

    real_t doppler_factor = 1.0 + ctx->opac_wl_step / c_Mms;
    ctx->opac_wl_size = (size_t)(log(ctx->opac_wl_max / ctx->opac_wl_min) /
                                 log(doppler_factor));

    /* Allocate wavelength bins. */

    ctx->opac_wl = (real_t*)resist_malloc(ctx->opac_wl_size * sizeof(real_t));

    /* Assign wavelengths to bins. */

    ctx->opac_wl[0] = ctx->opac_wl_min;
    for (size_t i = 1; i < ctx->opac_wl_size; i++) {
        ctx->opac_wl[i] = ctx->opac_wl[i - 1] * doppler_factor;
    }

    /* Compute number of real velocity grid points based on velocity
       parameters. */

    ctx->real_vr_count = (size_t)(ctx->max_vr / ctx->vr_step) + 1;

    /* Ejecta velocity grid includes ghost points outside the line forming
       region. For now we simply make the entire velocity grid larger by
       some factor determined by the geometry of the line forming region.
       The highest velocity grid point needs to be able to couple to a
       point roughly 2 * max_vr away, or exactly 2 * max_vr if there is no
       photosphere present. Hence we enlarge the grid by a factor of 3, and
       the outer 2/3 of all grid points are ghost points. */

    ctx->vr_count = 3 * ctx->real_vr_count;

    /* Allocate velocity grid but defer definition. */

    ctx->vr = (real_t*)resist_malloc(ctx->vr_count * sizeof(real_t));

    /* Copy angle parameters from config directly. */

    ctx->mu_per_vr = cfg->mu_per_vr;

    /* Compute number of angles based on angle parameters and velocity grid
       size. We only need to consider angles for real velocity grid points. */

    ctx->mu_count = ctx->mu_per_vr * ctx->real_vr_count;

    /* Allocate angle but defer definition. */

    ctx->mu = (real_t*)resist_malloc(ctx->mu_count * sizeof(real_t));

    /* Allocate specific intensities for real velocity grid points. */

    ctx->in = (real_t*)resist_malloc(ctx->mu_count * sizeof(real_t));

    /* Allocate Sobolev opacity bins, including ghost values. */

    ctx->tau = (real_t*)resist_malloc(ctx->opac_wl_size * ctx->vr_count *
                                      sizeof(real_t));

    /* Allocate source function values, including ghost values. */

    ctx->src = (real_t*)resist_malloc(ctx->opac_wl_size * ctx->vr_count *
                                      sizeof(real_t));

}
