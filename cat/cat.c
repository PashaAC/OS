#include <unistd.h>
#include <stdlib.h>
#include <helpers.h>

#define BUFFER_SIZE 4096

int main() {
    char buffer[BUFFER_SIZE];
    while (1) {
        ssize_t read_block = read_(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (read_block == -1)
            exit(EXIT_FAILURE);
        if (read_block == 0)
            break;
        if (write_(STDOUT_FILENO, buffer, read_block) == -1)
            exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
