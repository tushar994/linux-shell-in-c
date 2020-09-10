#include "headers.h"


// struct bg_process{
//     struct bg_process* prev;
//     struct bg_process* next;
//     int pid;
//     char command[1024];
// };


void add_bg(pid_t pid, char* command){
    struct bg_process* copy = first_bg;
    while(copy->next!=NULL){
        copy = copy->next;
    }
    struct bg_process* new;
    new = (struct bg_process*)malloc(sizeof(struct bg_process));

    new->previous = copy;
    copy->next = new;
    new->next = NULL;
    new->pid = pid;
    strcpy(new->command, command);
    
}


void handler(int sig)
{
    int w_st;
    pid_t pid = waitpid(-1, &w_st, WNOHANG);

    struct bg_process* copy = first_bg;
    while(copy!=NULL){
        if(copy->pid== pid){
            break;
        }
        copy = copy->next;
    }
    // write(STDOUT_FILENO, "bruh1\n ", strlen("bruh1\n "));
    if(copy!=NULL){
        // write(STDOUT_FILENO, "bruh\n ", strlen("bruh\n "));
        fprintf(stderr, "%s with pid %d exited with code %d\n", copy->command, copy->pid, w_st);
        struct bg_process* next = copy->next;
        struct bg_process* previous = copy->previous;

        previous->next = next;
        if(next!=NULL){
            next->previous = previous;
        }
        // printf("what\n");
        free(copy);
        // printf("what\n");
    }

}

int bg(char* path[], int n,char* starting_working_directory,char* command){
    // printf("background\n");
    path[n-1] = NULL;
    int status;
    pid_t forkReturn = fork();
    if(forkReturn <0){
        perror(path[0]);
        return 1;
    }
    else if (forkReturn == 0){
        setpgid(0, 0);

        // printf("%s\n",path[1]);
        if (execvp(path[0], path) < 0){
            perror(path[0]);
        }

        _exit(0);
    }
    else{
        setpgid(forkReturn,0);
        tcsetpgrp(STDIN_FILENO, getpid());
        add_bg(forkReturn,command);
        // setpgid(forkReturn,0);
        signal(SIGCHLD, handler);
        // kill(forkReturn, EOF );
        // pid_t okay = waitpid(forkReturn, &status, 0);
        // printf("what\n");
    }
    return 0; 
}