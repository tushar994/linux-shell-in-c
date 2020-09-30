#include "headers.h"

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

        // printf("%s\n",path[1]);
        execvp(path[0],path);
        // printf("%s\n",path[0]);
        printf("Not a valid command\n");
        exit(0);

        return 0;
    }
    else{
        strcpy(fg_command,path[0]);
        *current_fg_pid = forkReturn;
        pid_t okay = waitpid(forkReturn, &status, 0);
        *current_fg_pid = -1;
        fg_command[0] = '\0';

    }   
    return 0; 
}