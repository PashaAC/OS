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

void reverse(void * buf, size_t s, size_t f) {  
    for (size_t i = s; i < (s + f) / 2; ++i) {
        char c = ((char *) buf)[i];
        ((char *) buf)[i] = ((char *) buf)[f - i + s - 1];
        ((char *) buf)[f - i + s - 1] = c;
    }   
}

int main(int argc, char * argv[]) {
    char buffer[BUFFER_SIZE];
    size_t offset = 0;
    while (1) {
        ssize_t read_block = read_until(STDIN_FILENO, buffer + offset, BUFFER_SIZE - offset, ' ');
        if (read_block == -1) 
            exit(EXIT_FAILURE);
        if (read_block == 0)
            break;
        offset += read_block;
        while (1) {
            ssize_t delimiter_pos = delimiter_position(buffer, offset, ' ');
            if (delimiter_pos == -1)
                break;
            reverse(buffer, 0, delimiter_pos);
            if (delimiter_pos > 0) {
                write_(STDOUT_FILENO, buffer, delimiter_pos);
                write_(STDOUT_FILENO, " ", 1);
            }
            memmove(buffer, buffer + delimiter_pos + 1, offset - delimiter_pos - 1);
            offset -= delimiter_pos + 1;
        }
    }
    reverse(buffer, 0, offset);
    write_(STDOUT_FILENO, buffer, offset);
    exit(EXIT_SUCCESS);
}

