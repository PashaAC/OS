#include <stdlib.h>
#include <string.h>
#include <helpers.h>

#define BUFFER_SIZE 4096

ssize_t delimiter_position(const void * buf, size_t size, char delimiter) {
    for (size_t i = 0; i < size; ++i)
        if (((char *) buf)[i] == delimiter)
            return i;
    return -1;
}

int main(int argc, char * argv[]) {
    char buffer[BUFFER_SIZE];
    char * args[argc + 1];
    
    for (int i = 1; i < argc; ++i)
        args[i - 1] = argv[i];
    args[argc] = NULL;

    size_t offset = 0;
    while (1) {
        ssize_t line_length = read_until(STDIN_FILENO, buffer + offset, BUFFER_SIZE - offset, '\n');
        if (line_length < 0)
            exit(EXIT_FAILURE);
        ssize_t delimiter_pos = delimiter_position(buffer, line_length + offset, '\n');
        if (line_length == 0 && delimiter_pos < 0)
            break;
        if (delimiter_pos < 0) {
            offset += line_length;
            continue;
        }
        buffer[delimiter_pos] = 0;
        args[argc - 1] = buffer;
        int spawn_res = spawn(args[0], args);
        if (spawn_res == 0) {
            ssize_t write_block = write_(STDOUT_FILENO, buffer, delimiter_pos);
            if (write_block < 0)
                exit(EXIT_FAILURE);
            write_(STDOUT_FILENO, "\n", 1);
        }
        memmove(buffer, buffer + delimiter_pos + 1, offset + line_length - delimiter_pos - 1);
        offset += line_length - 1;
        offset -= delimiter_pos;

    }

    if (offset > 0) {
        buffer[offset] = 0;
        args[argc - 1] = buffer;
        int spawn_res = spawn(args[0], args);
        if (spawn_res == 0) {
            ssize_t write_block = write_(STDOUT_FILENO, buffer, offset);
            if (write_block < 0)
                exit(EXIT_FAILURE);
            write_(STDOUT_FILENO, "\n", 1);
        }
    }
    exit(EXIT_SUCCESS);
}

