#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void PassPrime(int p[2]) {
    int prime, n;

    close(p[1]);
    if (read(p[0], &prime, 4) != 4) {
        printf("failed to read prime from pipe\n");
        exit(1);
    }
    printf("prime %d\n", prime);

    if (read(p[0], &n, 4)) {
        int newPipe[2];
        pipe(newPipe);

        if (fork() == 0) {
            PassPrime(newPipe);
        }
        else {
            close(newPipe[0]);

            if (n % prime)
                write(newPipe[1], &n, 4);
            
            while (read(p[0], &n, 4)) {
                if (n % prime)
                    write(newPipe[1], &n, 4);
            }

            close(p[0]);
            close(newPipe[1]);
            wait(0);
        }
    }
    exit(0);
}

int main(char argc, char* argv[]) {
    if (argc != 1) {
        printf("Error! Usage: prime\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    if (fork() == 0) {
        PassPrime(p);
    } else {
        close(p[0]);

        for (int i = 2; i <= 35; i++) {
            if (write(p[1], &i, 4) != 4) {
                printf("failed to write prime %d to pipe\n", i);
                exit(1);
            }
        }

        close(p[1]);
        wait(0);
        exit(0);
    }
    return 0;
}