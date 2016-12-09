
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#define real double

#define c_Mms (real)299.792458

struct support_t {
    real min_wl;    /* Bluest wavelength of line loaded in AA.              */
    real max_wl;    /* Reddest wavelength of line loaded in AA.             */
    real bin_step;  /* Doppler shift width of wavelength bins in 10^3 km/s. */

    real min_v;     /* Slowest velocity in 10^3 km/s.                       */
    real max_v;     /* Fastest velocity in 10^3 km/s.                       */
    real v_step;    /* Step between velocity grid points in 10^3 km/s.      */

    size_t num_angles;


    size_t num_bins;        /* Opacity/source bin array size. */
    size_t num_radii;
    size_t num_rays;
};

struct setup_t {
};

void initialize(struct support_t* support,
                real min_wl,
                real max_wl,
                real bin_step real min_v,
                real max_v,
                real v_step,
                num_angles)
{
    initialize_bins(support, min_wl, max_wl, bin_step);
    initialize_radii(support, min_v, max_v, v_step);
    initialize_rays(support, num_angles);
}

void initialize_bins(struct support_t* support,
                     real min_wl,
                     real max_wl,
                     real bin_step)
{
    /* Minimum wavelength in Angstroms should be positive. */

    assert(min_wl > 0.0);
    support->min_wl = min_wl;

    /* Maximum wavelength in Angstroms should be greater than minimum. */

    assert(max_wl > support->min_wl);
    support->max_wl = max_wl;

    /* Bin step in Mm/s should be positive. */

    assert(bin_step > 0.0);
    support->bin_step = bin_step;

    /* Number of bins to allocate. */

    real doppler_factor = 1.0 + bin_step / c_Mms;
    support->num_bins = (size_t)(log(support->max_wl / support->min_wl) /
                                 log(doppler_factor));

    /* Allocate bins. */

    support->bins = (real*)malloc(support->num_bins * sizeof(real));
    assert(support->bins != NULL);

    /* Initialize bin centers. */

    real start = 0.5 * support->min_wl * (1.0 + doppler_factor); /* Hmmm have to cast the literals? */
    for (size_t i = 0; i < support->num_bins; i++)
        support->bins[i] = start * pow(doppler_factor, i);
}

void initialize_radii(struct support_t* support,
                      real min_v,
                      real max_v,
                      real v_step)
{
    /* Minimum velocity in Mm/s should be non-negative. */

    assert(min_v >= 0.0);
    support->min_v = min_v;

    /* Maximum velocity in Mm/s should be greater than minimum. */

    assert(max_v > support->min_v);
    support->max_v = max_v;

    /* Velocity step in Mm/s should be positive. */

    assert(v_step > 0.0);
    support->v_step = v_step;

    /* Number of radii to allocate. */

    support->num_radii = (size_t)((support->max_v - support->min_v) /
                                  support->v_step) + 1;

    /* Allocate radii. */

    support->radii = (real*)malloc(support->num_radii * sizeof(real));
    assert(support->radii != NULL);

//  /* Initialize radial grid points. */

//  for(size_t i = 0; i < support->num_radii; i++)
//      support->radii[i] = support->min_v + i * support->v_step;
}

void initialize_rays(struct support_t* support,
                     size_t num_angles)
{
    /* Number of angles must be positive. */

    assert(num_angles > 0);
    support->num_angles = num_angles;

    /* Number of rays to allocate. */

    support->num_rays = num_angles * support->num_radii;

    /* Allocate rays. */

    support->rays = (real*)malloc(support->num_rays * sizeof(real));
    assert(support->rays != NULL);
}

void execute(struct support_t* support,
             struct setup_t* setup)
{
    /* Sum over specific intensities into mean intensity. */

    for (size_t i = 0; i < support->num_bins; i++) {
        size_t i_offset = i * num_radii;
        for (size_t j = 0; j < num_radii; j++) {
            size_t j_offset = i_offset + j * num_angles;
            for (size_t k = 0; k < num_angles; k++) {
                support->src[i_offset + j] += support->in[j_offset + k] * dmu;
            }
        }
    }
}

void shutdown(struct support_t* support)
{
    free(support->bins);
    free(support->radii);
    free(support->rays);
}

int resist_mini(int argc,
                char* argv[])
{
    struct resist_config_t* config;

    resist_config_init_default(config);
    resist_config_finalize(config);

    return 0;
}

int main(int argc,
         char* argv[])
{
    return run(argc, argv);
}
