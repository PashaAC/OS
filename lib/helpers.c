#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "helpers.h"

ssize_t read_(int fd, void * buf, size_t count) {
    size_t offset = 0;
    while (offset < count) {
        ssize_t read_block = read(fd, buf + offset, count - offset);
        if (read_block == -1) 
            return -1;
        if (read_block == 0) 
            break;
        offset += read_block;
    }
    return offset;
}

ssize_t write_(int fd, const void * buf, size_t count) {
    size_t offset = 0;
    while (offset < count) {
        ssize_t write_block = write(fd, buf + offset, count - offset);
        if (write_block == -1) 
            return -1;
        offset += write_block;
    }
    return offset;
}

ssize_t read_until(int fd, void * buf, size_t count, char delimiter) {    
    size_t offset = 0;
    while (offset < count) {
        ssize_t read_block = read(fd, buf + offset, count - offset);
        if (read_block == -1) 
            return -1;
        if (read_block == 0) 
            break;
        for (size_t i = offset; i != offset + read_block; ++i)
            if (((char *)buf)[i] == delimiter)
                return offset + read_block;
        offset += read_block;
    }
    return offset;
}

int spawn(const char * file, char * const argv[]) {
    pid_t cpid = fork();
    if (cpid == -1) // error 
        return -1;

    if (cpid) {     // parent
        int status;
        while (wait(&status) !=  cpid);
        if (WIFEXITED(status)) 
            return WEXITSTATUS(status);
    }
    else {          // child
        return execvp(file, argv);
    }
    exit(EXIT_FAILURE);
}

struct execargs_t* create_execargs(char * args[]) {
    struct execargs_t * program = (struct execargs_t *) malloc(sizeof(struct execargs_t));
    if (program == NULL)
        exit(EXIT_FAILURE);
    program->args = args;
    program->name = args[0];
    return program;
}

int exec(struct execargs_t * program) {
    return execvp(program->name, program->args);
}

pid_t * cpids;
size_t cpids_size;

void kill_all() {
    for (size_t i = 0; i < cpids_size; ++i)
        if (cpids[i] != -1)
            kill(cpids[i], SIGKILL);
    cpids_size = 0;
}

void kill_handler(int code) {
    code = 0;
    kill_all();
}

void sig_init(struct sigaction *sig_action, sigset_t *sig_set)
{
    memset(sig_action, 0, sizeof(struct sigaction));
    sigemptyset(sig_set);
    sigaddset(sig_set, SIGINT);
    sig_action->sa_mask = (*sig_set);
    sig_action->sa_handler = kill_handler;

}

void exit_handler(int code)
{
    code = 0;
    exit(EXIT_FAILURE);
}

void other_sig_init(struct sigaction *other_sig_action, sigset_t *other_sig_set)
{
    memset(other_sig_action, 0, sizeof(struct sigaction));
    sigemptyset(other_sig_set);
    sigaddset(other_sig_set, SIGPIPE);
    other_sig_action->sa_mask = (*other_sig_set);
    other_sig_action->sa_handler = exit_handler;

}

int runpiped(struct execargs_t** programs, size_t n) {  
    if (n == 0)
        return 0;

    cpids = (pid_t *) malloc (sizeof(pid_t) * n);
    if (cpids == NULL)
        return 0;

    cpids_size = 0;
    for (size_t i = 0; i < n; ++i)
        cpids[i] = -1;

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = kill_handler;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    action.sa_mask = set;
    sigaction(SIGINT, &action, 0);


    int pipefd[n - 1][2];
    for (size_t i = 0; i < n - 1; ++i)
        if (pipe(pipefd[i]) == -1)
            return 0;

    for (size_t i = 0; i < n; ++i)
    {
        pid_t cpid = fork();
        if (cpid < 0) {     // error
            kill_all();
            for (size_t j = 0; j < n - 1; ++j)
                if (close(pipefd[j][0]) == -1 || close(pipefd[j][1]) == -1)
                    return -1;
        }

        if (cpid == 0) {    // child
            if (i != 0 && dup2(pipefd[i - 1][0], 0) == -1) 
                return -1;
            if (i != n - 1 && dup2(pipefd[i][1], 1) == -1)
                return -1;
            for (size_t j = 0; j < n - 1; ++j) {
                if (j != i - 1 && close(pipefd[j][0]) == -1) 
                    return -1;
                if (j != i && close(pipefd[j][1]) == -1)
                    return -1;
            }
            exec(programs[i]);
        }

        if (cpid > 0) {     // parent
            cpids[cpids_size++] = cpid;
        }
    }

    for (size_t j = 0; j < n - 1; ++j)
        if (close(pipefd[j][0]) == -1 || close(pipefd[j][1]) == -1)
            return -1;
    
    int status;
    while (wait(&status) != -1 || errno != ECHILD);
    return 0;
}

