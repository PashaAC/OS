#include <bufio.h>

#define BUFFER_SIZE 4096

int main() {
    struct buf_t * buf = buf_new(BUFFER_SIZE);
    if (buf == NULL)
        exit(EXIT_FAILURE);
    ssize_t fill_block = 1;
    do {
        fill_block = buf_fill(STDIN_FILENO, buf, buf_capacity(buf) - buf_size(buf));
        if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) == -1)
                exit(EXIT_FAILURE);
    } while (fill_block > 0);
    if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) == -1)
        exit(EXIT_FAILURE);
    buf_free(buf);
    exit(EXIT_SUCCESS);
} 

