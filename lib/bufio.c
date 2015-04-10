#include <string.h>
#include "bufio.h"

struct buf_t 
{
    size_t size;
    size_t capacity;
    char * buffer;
};

struct buf_t * buf_new(size_t capacity)
{
    struct buf_t * buf = (struct buf_t * ) malloc(sizeof(struct buf_t));
    if (buf == NULL)
    {
        return NULL;
    }
    buf->buffer = (char * ) malloc(sizeof(char) * capacity);
    if (buf->buffer == NULL)
    {
        return NULL;
    }
    buf->capacity = capacity;
    buf->size = 0;
    return buf;
}

void buf_free(struct buf_t * buf)
{
    if (buf != NULL)
    {
        free(buf->buffer);
        free(buf);
    }
}

size_t buf_capacity(struct buf_t * buf)
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    return buf->capacity;
}

size_t buf_size(struct buf_t * buf)
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    return buf->size;
}

ssize_t buf_fill(fd_t fd, struct buf_t * buf, size_t required) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    size_t max_read_size = required < buf->capacity ? required : buf->capacity;
    while (buf->size < max_read_size)
    {
        ssize_t was_read = read(fd, buf->buffer + buf->size, buf->capacity - buf->size);
        if (was_read < 0)
        {
            return -1;
        }
        if (was_read == 0)
        {
            break;
        }
        buf->size += was_read;
    }
    return buf->size;
}

ssize_t buf_flush(fd_t fd, struct buf_t * buf, size_t required)
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    size_t prev_size = buf->size;
    size_t max_write_size = required < buf->size ? required : buf->size;
    size_t offset = 0;
    while (offset < max_write_size)
    {
        ssize_t was_written = write(fd, buf->buffer + offset, buf->size -  offset);
        if (was_written < 0)
        {
            memmove(buf->buffer, buf->buffer + offset, buf->size - offset);
            buf->size = buf->size - offset;
            return -1;
        }
        offset += was_written;
    }
    memmove(buf->buffer, buf->buffer + offset, buf->size - offset);
    buf->size = buf->size - offset;
    return prev_size - buf->size;
}

ssize_t buf_getline(fd_t fd, struct buf_t * buf, char * dest)
{
#ifdef DEBUG 
    if (buf == NULL) 
    {
        abort();
    }
#endif
    size_t str_len = 0;
    for(;;)
    {
        for (size_t i = 0; i != buf->size; ++i)
        {
            if (buf->buffer[i] == '\n')
            {
                memmove(buf->buffer, buf->buffer + i + 1, buf->size - i - 1);
                buf->size -= i + 1;
                return str_len;
            }
            dest[str_len++] = buf->buffer[i];
        }
        if (buf_fill(fd, buf + buf->size, 1) <= 0)
        {
            return -1;
        }
    }
    return -1;
}
