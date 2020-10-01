#include "headers.h"

void handler_child(int sig)
{
    // printf("handler\n");
    int w_st;
    pid_t pid = waitpid(-1, &w_st, WNOHANG);

    if(pid==-1 ){
        // perror("sigchild");
        // return;
    }

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
        fprintf(stderr, "%s with pid %d exited with code %d\n", copy->command, copy->pid, WEXITSTATUS(w_st));

        struct bg_process* next = copy->next;
        struct bg_process* previous = copy->previous;

        // printf("what\n");
        // if(previous!=NULL){
            previous->next = next;
        // }
        if(next!=NULL){
            next->previous = previous;
        }
        free(copy);
    }
}

// for ^C
void handler_c(int sig)
{
    printf("hdhe\n");
    if(*current_fg_pid != -1){
        kill(*current_fg_pid,sig);
        *current_fg_pid = 1;
        return;
    }
    else{
        return; 
    }
}

// for ^Z
void handler_z(int sig){
    // printf("hdhe\n");
    if(*current_fg_pid == -1){
        return;
    }
    char* pass[1];
    pass[0] = (char*)malloc(1000*sizeof(char));
    strcpy(pass[0],fg_command);
    // printf("%d\n",*current_fg_pid);
    add_bg(*current_fg_pid,pass,1);

    free(pass[0]);
    if(setpgid(*current_fg_pid,0)<0){
        perror("setting");
    } 
    kill(*current_fg_pid,sig);


    *current_fg_pid=-1;
    // fg_command[0] = '\0';
}


int define_all_signals(){
    signal(SIGCHLD, handler_child);
    signal(SIGINT,handler_c);
    // signal(SIGTSTP, handler_z);
    signal(SIGTSTP, handler_z);
}