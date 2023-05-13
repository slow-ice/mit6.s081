#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int MAXSIZE = 8;

int main(char argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    int p[2];
    int pid;
    char bytes[MAXSIZE];

    pipe(p);

    pid = fork();

    if (pid == 0) {
        read(p[0], bytes, MAXSIZE);
        // close(p[0]);
        printf("%d: received ping\n", getpid());
        write(p[1], "1", MAXSIZE);
        // close(p[1]);
    } else {
        write(p[1], "hello", MAXSIZE);
        // close(p[1]);
        // wait for child process to receive ping
        wait(0);
        read(p[0], bytes, MAXSIZE);
        // close(p[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}