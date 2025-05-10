
#include "echo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_connection(int client_fd, int verbose) {
    char buf[BUF_SIZE];
    ssize_t nread;
    size_t idx = 0;

    while ((nread = read(client_fd, buf + idx, 1)) > 0) {
        if (buf[idx] == '\n' || idx == BUF_SIZE - 2) {
            buf[idx + 1] = '\0';

            if (verbose) {
                printf("%s", buf);
                fflush(stdout);
            }

            if (write(client_fd, buf, strlen(buf)) < 0) {
                perror("write");
                break;
            }
            idx = 0;
        } else {
            idx++;
        }
    }

    if (nread < 0) {
        perror("read");
    }
}
