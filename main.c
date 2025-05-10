#include "echo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-p port] [-v]\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int opt, port = DEFAULT_PORT, verbose = 0;

    while ((opt = getopt(argc, argv, "p:v")) != -1) {
        switch (opt) {
        case 'p':
            port = atoi(optarg);
            if (port <= 0 || port > 65535)
                usage(argv[0]);
            break;
        case 'v':
            verbose = 1;
            break;
        default:
            usage(argv[0]);
        }
    }

    return run_server(port, verbose);
}
