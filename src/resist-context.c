
#include <assert.h>
#include <stdio.h>
#include <tgmath.h>

#include "resist-config.h"
#include "resist-constants.h"
#include "resist-context.h"

void resist_context_init(struct resist_context_t** ctx,
                         struct resist_config_t* cfg)
{
    *ctx = (struct resist_context_t*)malloc(sizeof(struct resist_context_t));
    _resist_context_init(*ctx, cfg);
}

void resist_context_free(struct resist_context_t* ctx)
{

    free(ctx->mu);
    ctx->mu = NULL;

    free(ctx->vr);
    ctx->vr = NULL;

    free(ctx->wl);
    ctx->wl = NULL;

    free(ctx);
    ctx = NULL;

}

void resist_context_output(struct resist_context_t* ctx)
{

    /* TODO Make this a char buffer formatted with snprintf instead. */
    /* TODO Or do we bother with logging */
    printf("resist context\n");
    printf("    min_wl    = %f  max_wl  = %f  wl_step = %f\n", ctx->min_wl,
           ctx->max_wl, ctx->wl_step);
    printf("    max_vr    = %f  vr_step = %f\n", ctx->max_vr, ctx->vr_step);
    printf("    mu_per_vr = %zu\n", ctx->mu_per_vr);
    printf("    wl_count  = %zu\n", ctx->wl_count);
    printf("    vr_count  = %zu\n", ctx->vr_count);
    printf("    mu_count  = %zu\n", ctx->mu_count);

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

    ctx->wl = (real_t*)malloc(ctx->wl_count * sizeof(real_t));
    assert(ctx->wl != NULL);

    /* Assign wavelengths to bins. */

    ctx->wl[0] = ctx->min_wl;
    for (size_t i = 1; i < ctx->wl_count; i++) {
        ctx->wl[i] = ctx->wl[i - 1] * doppler_factor;
    }

    /* Copy velocity parameters from config directly. */

    ctx->max_vr = cfg->max_vr;
    ctx->vr_step = cfg->vr_step;

    /* Compute number of radii based on velocity parameters. */

    ctx->vr_count = (size_t)(ctx->max_vr / ctx->vr_step) + 1;

    /* Allocate velocity radii but defer definition. */

    ctx->vr = (real_t*)malloc(ctx->vr_count * sizeof(real_t));
    assert(ctx->vr != NULL);

    /* Copy angle parameters from config directly. */

    ctx->mu_per_vr = cfg->mu_per_vr;

    /* Compute number of angles based on angle parameters and radii. */

    ctx->mu_count = ctx->mu_per_vr * ctx->vr_count;

    /* Allocate angle but defer definition. */

    ctx->mu = (real_t*)malloc(ctx->mu_count * sizeof(real_t));
    assert(ctx->mu != NULL);

}
