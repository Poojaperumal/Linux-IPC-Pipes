#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void server(int, int);
void client(int, int);

int main() {
    int p1[2], p2[2], pid;

    pipe(p1);
    pipe(p2);

    pid = fork();

    if (pid == 0) {
        close(p1[1]);
        close(p2[0]);

        server(p1[0], p2[1]);
        exit(0);
    }

    close(p1[0]);
    close(p2[1]);

    client(p1[1], p2[0]);

    wait(NULL);

    return 0;
}

void server(int rfd, int wfd) {
    int n;
    char fname[2000];
    char buff[2000];

    n = read(rfd, fname, 2000);
    fname[n] = '\0';

    int fd = open(fname, O_RDONLY);

    if (fd < 0) {
        write(wfd, "can't open", 10);
    } else {
        n = read(fd, buff, 2000);
        write(wfd, buff, n);
        close(fd);
    }
}

void client(int wfd, int rfd) {
    int n;
    char fname[2000];
    char buff[2000];

    printf("Enter file name: ");
    scanf("%s", fname);

    write(wfd, fname, 2000);

    n = read(rfd, buff, 2000);
    buff[n] = '\0';

    printf("\nFile Content:\n");
    write(1, buff, n);
}
