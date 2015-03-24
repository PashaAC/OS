#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include "helpers.h"

#define BUFFER_SIZE 4100 

int find_elem(char const * buffer, int count, char symbol)
{
    for (int i = 0; i != count; ++i)
    {
        if (buffer[i] == symbol)
        {
            return i;
        }
    }
    return count;
}

void move(char * from, char * to, size_t count) 
{
    while (count-- && (*to++ = *from++));
}

int main(int argc, char ** argv) 
{
    char * new_argv[argc + 1];
    for (int i = 0; i != argc - 1; ++i)
    {
        new_argv[i] = argv[i + 1];
    }
    
    char buffer[BUFFER_SIZE];
    char last_arg[BUFFER_SIZE];
    int offset = 0;
    for(ssize_t read_block = -1;read_block;)
    {
        read_block = read_until(STDIN_FILENO, buffer + offset, BUFFER_SIZE - offset, '\n');
        if (read_block < 0)
        {
            exit(EXIT_FAILURE);
        }
        offset += read_block;
        for (;offset > 1;)
        {
//            write_(STDOUT_FILENO, buffer, offset);
            int ind = find_elem(buffer, offset, '\n');
            move(buffer, last_arg, ind);
            last_arg[ind] = 0;
            new_argv[argc - 1] = last_arg;
            new_argv[argc] = NULL;
            if (!spawn(new_argv[0], new_argv)) 
            {
                if (write_(STDOUT_FILENO, buffer, ind + (ind != offset)) < 0)
                {
                    exit(EXIT_FAILURE);
                }
            }
            move(buffer + ind + 1, buffer, offset - ind);
            offset -= ind;
            offset -= (ind == offset);
        }
    }
    exit(EXIT_SUCCESS);
}
