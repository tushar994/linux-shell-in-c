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
    int return_val = 0;
    if(forkReturn <0){
        perror(path[0]);
        return 1;
    }
    else if (forkReturn == 0){
        setpgid(0,0);
        
        execvp(path[0],path);

        fprintf(stderr,"Not a valid command\n");
        exit(1);

        return 1;
    }
    else{
        strcpy(fg_command,path[0]);
        for(int i=1;i<n;i++){
            strcat(fg_command,path[i]);
        }
        *current_fg_pid = forkReturn;
        // printf("parent: %d\n",getpid());
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(STDIN_FILENO, forkReturn);
        pid_t okay = waitpid(forkReturn, &status, WUNTRACED);
        tcsetpgrp(STDIN_FILENO, getpgrp());
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        // printf("%d\n",*bruhbruh);
        if(WIFSTOPPED(status)){
            add_bg(forkReturn,path,n);
            return_val=1;
        }
        // printf("%d\n",WIFEXITED(status));
        if(WIFEXITED(status)!=1){
            // printf("bruh\n");
            return_val=1;
        }
        if(WEXITSTATUS(status)!=0){
            // printf("bruh22\n");
            return_val=1;
        }
        *current_fg_pid = -1;
        fg_command[0] = '\0';
    }   
    return return_val; 
}