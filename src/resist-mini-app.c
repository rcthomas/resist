
#include <unistd.h>

#include "resist-config.h"
#include "resist-context.h"
#include "resist-memory.h"

int resist_mini_app(int argc,
                    char* argv[]);

int main(int argc,
         char* argv[])
{

    return resist_mini_app(argc, argv);

}

int resist_mini_app(int argc,
                    char* argv[])
{

    struct resist_config_t* config;
    struct resist_context_t* context;

    /* Application configuration. */

    resist_config_init_default(&config);

    /* Context initialization with configuration. */

    resist_context_init(&context, config);
    resist_config_free(config);

    resist_context_output(context);

    /* Tear-down. */

    resist_context_free(context);

    return 0;

}
