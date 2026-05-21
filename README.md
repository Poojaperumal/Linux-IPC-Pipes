# Linux-IPC-Pipes

# Ex03 - Linux IPC - Pipes

## AIM:
To write a C program that illustrates communication between two processes using unnamed and named pipes.

---

## DESIGN STEPS:

Step 1: Use Linux environment (Parrot OS / VM / JSLinux)

Step 2: Write programs using pipe() and mkfifo()

Step 3: Compile and execute and verify output

---

## PROGRAM

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "/tmp/my_fifo"
#define FILE_NAME "hello.txt"

void server();
void client();

int main() {
    pid_t pid;

    mkfifo(FIFO_FILE, 0666);

    pid = fork();

    if (pid > 0) {
        sleep(1);
        server();
    } else if (pid == 0) {
        client();
    } else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void server() {
    int fifo_fd, file_fd;
    char buffer[1024];
    ssize_t bytes_read;

    file_fd = open(FILE_NAME, O_RDONLY);

    if (file_fd == -1) {
        perror("Error opening hello.txt");
        exit(EXIT_FAILURE);
    }

    fifo_fd = open(FIFO_FILE, O_WRONLY);

    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        write(fifo_fd, buffer, bytes_read);
    }

    close(file_fd);
    close(fifo_fd);
}

void client() {
    int fifo_fd;
    char buffer[1024];
    ssize_t bytes_read;

    fifo_fd = open(FIFO_FILE, O_RDONLY);

    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fifo_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fifo_fd);
}

```

### Unnamed Pipe Output

<img width="603" height="239" alt="image" src="https://github.com/user-attachments/assets/4117ad5f-6e1d-4128-84fb-fe7b6629d2c8" />


---

### PROGRAM

```
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

```



### Named Pipe Output

![Named Pipe Output](named_output.png)

---

## RESULT:
Linux IPC programs using unnamed and named pipes executed successfully.
