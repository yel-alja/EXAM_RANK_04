#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
    int pipefd[2];
    int pid;

    pipe(pipefd);
    pid = fork();
    if(!pid)
    {
        if(type == 'r')
        {
            close(pipefd[0]);
            dup2(pipefd[1] , 1);
            close(pipefd[1]);
        }
        if(type == 'w')
        {
            close(pipefd[1]);
            dup2(pipefd[0] , 0);
            close(pipefd[0]);
        }
        execvp(file , argv);
        exit(-1);
    }
    if(type == 'r')
    {
        close(pipefd[1]);
        return(pipefd[0]);
    }
    if(type == 'w')
    {
        close(pipefd[0]);
        return(pipefd[1]);
    }
    return -1;
}

int main(void) {
    char buf[1000];
    char *argv[] = {"ls", "-la", NULL};

    int fd = ft_popen("ls", argv, 'r');
    if (fd == -1) {
        return 1;
    }

    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("%s\n", buf);
    }

    return 0;
}