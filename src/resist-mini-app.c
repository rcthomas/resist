
#include "resist-config.h"

int resist_mini_app(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    return resist_mini_app(argc, argv);
}

int resist_mini_app(int argc, char* argv[]) {

    struct resist_config_t* config;

    resist_config_init_default(&config);
    resist_config_output(config);
    resist_config_finalize(config);

    return 0;

}
