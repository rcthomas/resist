
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
    ctx->src = NULL;

    resist_free(ctx->tau);
    ctx->tau = NULL;

    resist_free(ctx->mu);
    ctx->mu = NULL;

    resist_free(ctx->vr);
    ctx->vr = NULL;

    resist_free(ctx->wl);
    ctx->wl = NULL;

    resist_free(ctx);
    ctx = NULL;

}

void resist_context_output(struct resist_context_t* ctx)
{

    /* TODO Make this a char buffer formatted with snprintf instead. */
    /* TODO Or do we bother with logging */
    printf("resist context\n");
    printf("    min_wl    = %10.2e  max_wl  = %10.2e  wl_step = %10f\n",
           ctx->min_wl, ctx->max_wl, ctx->wl_step);
    printf("    max_vr    = %10f  vr_step = %10f\n", ctx->max_vr, ctx->vr_step);
    printf("    mu_per_vr = %10zu\n", ctx->mu_per_vr);
    printf("    wl_count  = %10zu\n", ctx->wl_count);
    printf("    vr_count  = %10zu  real_vr_count = %10zu\n", ctx->vr_count,
           ctx->real_vr_count);
    printf("    mu_count  = %10zu\n", ctx->mu_count);

}

void _resist_context_init(struct resist_context_t* ctx,
                          struct resist_config_t* cfg)
{

    /* Copy wavelength parameters from config directly. */

    ctx->min_wl = cfg->min_wl;
    ctx->max_wl = cfg->max_wl;
    ctx->wl_step = cfg->wl_step;

    /* Compute number of bins based on wavelength parameters. */

    real_t doppler_factor = 1.0 + ctx->wl_step / c_Mms; /* precision on 1.0 */
    ctx->wl_count = (size_t)(log(ctx->max_wl / ctx->min_wl) /
                             log(doppler_factor));

    /* Allocate wavelength bins. */

    ctx->wl = (real_t*)resist_malloc(ctx->wl_count * sizeof(real_t));

    /* Assign wavelengths to bins. */

    ctx->wl[0] = ctx->min_wl;
    for (size_t i = 1; i < ctx->wl_count; i++) {
        ctx->wl[i] = ctx->wl[i - 1] * doppler_factor;
    }

    /* Copy velocity parameters from config directly. */

    ctx->max_vr = cfg->max_vr;
    ctx->vr_step = cfg->vr_step;

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

    /* Allocate Sobolev opacity bins, including ghost values. */

    ctx->tau = (real_t*)resist_malloc(ctx->wl_count * ctx->vr_count *
                                      sizeof(real_t));

    /* Allocate source function values, including ghost values. */

    ctx->src = (real_t*)resist_malloc(ctx->wl_count * ctx->vr_count *
                                      sizeof(real_t));

}
