
#include "shared.h"

#ifdef COMPILED
int main(void) {

    const char *code = "inject";

#else
int main(int argc, const char **argv) {

    if (argc < 2) return 1;
    const char *code = argv[1];

#endif

    init();

    run(code);

    cleanup();

    return 0;
}

