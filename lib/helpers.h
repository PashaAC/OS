#ifndef HELPERS_H_
#define HELPERS_H_

#include <unistd.h>

ssize_t read_(int fd, void * buf, size_t count);
ssize_t write_(int fd, const void * buf, size_t count);

ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
int spawn(const char * file, char * const argv []);

struct execargs_t {
    char** prog_args;
    char* prog_name;
};

struct execargs_t *create_execargs(char * args[]);
int exec(struct execargs_t* args);
int runpiped(struct execargs_t** programs, size_t n);

#endif
