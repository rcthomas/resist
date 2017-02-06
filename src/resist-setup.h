
#ifndef RESIST_SETUP_H
#define RESIST_SETUP_H

/* Setup type. */

struct resist_setup_t {
};

/* Use sensible values to initialize a setup. */

void resist_setup_init_default(struct resist_setup_t** stp);

/* Allocate setup, validate and assign parameters to it. */

void resist_setup_init(struct resist_setup_t** stp);

/* Tear down setup object. */

void resist_setup_free(struct resist_setup_t* stp);

#endif /* RESIST_SETUP_H */
