#include "headers.h"

void handler_child(int sig)
{
    printf("handler\n");
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
        printf("here\n");

        // write(STDOUT_FILENO, "bruh\n ", strlen("bruh\n "));
        fprintf(stderr, "%s with pid %d exited with code %d\n", copy->command, copy->pid, w_st);
        printf("what\n");
        struct bg_process* next = copy->next;
        struct bg_process* previous = copy->previous;

        printf("what\n");
        // if(previous!=NULL){
            previous->next = next;
        // }
        if(next!=NULL){
            next->previous = previous;
        }
        free(copy);
    }
    printf("handler\n");
}





int define_all_signals(){
    signal(SIGCHLD, handler_child);
}