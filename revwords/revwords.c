#include <unistd.h>
#include <stdlib.h>
#include "helpers.h"

#define BUFFER_SIZE 50

void reverse(char * buf, size_t from, size_t to)
{
    for (size_t i = from; i != (from + to + 1) / 2; ++i)
    {
        char tmp = buf[i];
        buf[i] = buf[to - i + from];
        buf[to - i + from] = tmp;
    }
}

ssize_t reverse_words(char * buf, size_t count, char delimeter) 
{
    ssize_t last_delimeter_pos = -1;
    for (size_t i = 0; i != count; ++i) 
    {
        if (buf[i] == delimeter) 
        {
            reverse(buf, last_delimeter_pos + 1, i - 1);
            last_delimeter_pos = i;
        }
    }
    return last_delimeter_pos;
}

void move(char * from, char * to, size_t count) 
{
    while (count-- && (*to++ = *from++));
}

int main(int argc, char ** argv) 
{
    char buf[BUFFER_SIZE];
    char delimeter = ' ';
    int offset = 0;
    for (;;) 
    {
        ssize_t was_read = read_until(STDIN_FILENO, buf + offset, BUFFER_SIZE - offset, delimeter);
        if (was_read < 0) 
        {
            exit(EXIT_FAILURE);
        }
        if (was_read == 0) 
        {
            reverse(buf, 0, offset - 2);
            write_(STDOUT_FILENO, buf, offset - 1);
            break;
        }
        ssize_t reversed_block = reverse_words(buf, was_read + offset, delimeter) + 1;
        if (reversed_block < 0) 
        {
            offset += was_read;
            continue;
        }
        write_(STDOUT_FILENO, buf, reversed_block);
        move(buf + reversed_block, buf, was_read + offset - reversed_block);
        offset += was_read;
        offset -= reversed_block;
    }
    exit(EXIT_SUCCESS); 
}
