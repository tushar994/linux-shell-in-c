#include "headers.h"
// void handler_cdhf(int sig)
// {
//     *current_fg_pid = -1;
//     printf("%d\n",getpid());
//     if(getpid()==*our_gpid){
//         printf("what1\n");
//         return;
//     }
//     else{
//         printf("what\n");
//         raise(sig);
//     }
// }

int fg(char* path[], int n,char* starting_working_directory){
    path[n] = NULL;
    int status;
    pid_t forkReturn = fork();
    // printf("bruhhhh");
    if(forkReturn <0){
        perror(path[0]);
        return 1;
    }
    else if (forkReturn == 0){
        // setpgid(0,0);
        // printf("child: %d\n",getpid());
        // signal(SIGINT,handler_cdhf);
        // signal(SIGTSTP, handler_z);
        // signal(SIGTSTP, handler_cdhf);
        // printf("%s\n",path[1]);
        execvp(path[0],path);
        // printf("%s\n",path[0]);
        fprintf(stderr,"Not a valid command\n");
        exit(0);

        return 0;
    }
    else{

        strcpy(fg_command,path[0]);
        *current_fg_pid = forkReturn;
        // printf("parent: %d\n",getpid());
        // signal(SIGTTOU, SIG_IGN);
        // signal(SIGTTIN, SIG_IGN);
        // tcsetpgrp(STDIN_FILENO, forkReturn);
        pid_t okay = waitpid(forkReturn, &status, WUNTRACED);
        // tcsetpgrp(STDIN_FILENO, getpgrp());
        // signal(SIGTTOU, SIG_DFL);
        // signal(SIGTTIN, SIG_DFL);
        *current_fg_pid = -1;
        fg_command[0] = '\0';

    }   
    return 0; 
}