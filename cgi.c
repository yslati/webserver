#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
    export REQUEST_METHOD=GET
    export SERVER_PROTOCOL="HTTP/1.1"
    export PATH_INFO=$(pwd)
 */

int     main(int argc, char **argv, char **environ)
{
    int     fds1[2];
    int     fds2[2];
    char    *args[] = { "/Users/yslati/Desktop/webserver/cgi_tester", NULL };

    pipe(fds1);
    pipe(fds2);
    pid_t   pid = fork();
    if (!pid)
    {
        dup2(fds1[0], 0);
        // write pipe
        close(fds2[0]);
        dup2(fds2[1], 1);
        close(fds1[1]);
        // close(fds1[0]);
        execve(args[0], args, environ);
    }
    else
    {
        close(fds1[0]);
        write(fds1[1], "Hello==", 7);
        write(fds1[1], "Hello==", 7);
        write(fds1[1], "Hello==", 7);
        close(fds1[1]);
        int r;
        char c;
        close(fds2[1]);
        while ((r = read(fds2[0], &c, 1)) > 0) {
            write(1, &c, 1);
        }
        waitpid(pid, 0, 0);
    }
    return 0;
}