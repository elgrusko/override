#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/prctl.h>

int         main(void)
{
    pid_t     pid = fork();                                      
    char    buf[32];                                           

    memset(buf, '\0', 32);
    int     ret = 0;                                     
    int     state = 0;                                       
    
    if (pid == 0)                                               // FILS
    {
        prctl(1, 1);
        ptrace(0, 0, NULL, NULL);                             
        puts("Give me some shellcode, k");
        gets(buf);
    }
    else                                                        // PERE
    {
        while (true) {
            wait(&state);
            if ((state & 127) == 0){
                    puts("child is exiting...");
                    return (0);
            }
            ret = ptrace(3, pid, 44, NULL)
            if (ret == 11) {                         // exec()
               puts("no exec() for you");
               kill(pid, SIGKILL);
            }            
        }
    }
    return (0);
}
