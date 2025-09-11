#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int picoshell(char **cmd[])
{
    int i = 0;
    int p = -1;
    int fd[2];
    int pid;
 
    while(cmd[i])
    {
        if(cmd[i + 1] && pipe(fd))
            return -1;
        pid = fork();
        if(pid == 0)
        {
            if(p != -1)
            {
                dup2(p , 0);
                close(p);
            }
            if(cmd[i + 1])
            {
                dup2(fd[1] , 1);
                close(fd[1]);
            }
            execvp(cmd[i][0] , cmd[i]);
            exit(1);
        }
        if(p != -1)
            close(p); 
        if(cmd[i + 1])
        {
            close(fd[1]);
            p = fd[0];
        }
        i++;
    }
    i = 0;
    int status;
    while(cmd[i])
    {
        wait(&status);
        if(status)
            return 1;
        i++;
    }
    return (0);
}

int main()
{
    char *ls[] = {"ls" , "-la" , NULL};
    char *cat[] = {"cat" , "-e" , NULL};
    char *wc[] = {"wc" , "-l" , NULL};
    char **cmd[] = {ls , cat ,wc, NULL};
    picoshell(cmd);
}
