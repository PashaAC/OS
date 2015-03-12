#include <unistd.h>
#include "helpers.h"

ssize_t read_(int fd, void * buffer, size_t count)
{
    size_t offset = 0;
    while (offset < count) 
    {
        ssize_t read_block = read(fd, buffer + offset, count - offset);
        if (read_block < 0) 
        {
            return read_block;
        }
        if (read_block == 0) 
        {
            break;
        }
        offset += read_block;
    }
    return offset;
}
       
ssize_t write_(int fd, const void * buffer, size_t count) 
{
    size_t offset = 0;
    while (offset < count) 
    {
        ssize_t write_block = write(fd, buffer + offset, count - offset);
        if (write_block < 0) 
        {
            return write_block;
        }
        offset += write_block;
    }
    return offset;
}

