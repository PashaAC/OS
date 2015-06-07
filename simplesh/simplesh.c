#define _POSIX_SOURCE

#include <string.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <helpers.h>
#include <bufio.h>

#define BUFFER_SIZE 4096

char buffer[BUFFER_SIZE];

char** split(char *s, const char *d)
{
    int cnt = 0;
    char **ans = NULL;
    char *ptr = (char *) strtok(s, d);

    while (ptr != NULL)
    {
        ++cnt;
        ans = (char**) realloc(ans, cnt * sizeof(char *));
        ans[cnt - 1] = ptr;
        ptr = (char *) strtok(NULL, d);
    }
    ++cnt;
    ans = (char**) realloc(ans, cnt * sizeof(char *));
    ans[cnt - 1] = NULL;
    return ans;
}

void handler(int code) {
    code = 0;
}

int main(int argc, char * argv[]) { 
    struct sigaction sig_action;
    memset(&sig_action, 0, sizeof(struct sigaction));
    sigset_t sig_set;
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGINT);
    sig_action.sa_mask = sig_set;
    sig_action.sa_handler = handler;
    sigaction(SIGINT, &sig_action, 0);

    struct buf_t * tbuf = buf_new(BUFFER_SIZE);
    while (1) {
        if (write_(STDOUT_FILENO, "$ ", 2) < 0) 
            exit(EXIT_FAILURE);
        
        int line_size = buf_getline(STDIN_FILENO, tbuf, buffer);
        
        if (tbuf->finish)
            exit(EXIT_SUCCESS);
        
        if (line_size > 0) {
            char ** programs = split(buffer, "|\n");
            int i = 0;
            struct execargs_t ** exec_programs = NULL;
            while (programs[i] != NULL)
            {
                char ** program = split(programs[i], " ");
                exec_programs = (struct execargs_t **) realloc(exec_programs, sizeof(struct execargs_t *) * (i + 1));
                exec_programs[i++] = create_execargs(program);
            }
            runpiped(exec_programs, i);
            memset(buffer, 0, BUFFER_SIZE);
        }
    }
}
