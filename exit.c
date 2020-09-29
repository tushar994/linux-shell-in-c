#include "headers.h"

int exit_shell(){
    struct bg_process* copy = first_bg;
    copy = copy->next;
    int flag = 0;
    while(copy!=NULL){
        pid_t pid = copy->pid;
        if(kill( pid ,SIGKILL)==0){
            printf("%s with pid %d has been killed\n",copy->command,copy->pid);
            struct bg_process* next = copy->next;
            struct bg_process* previous = copy->previous;

            previous->next = next;
            if(next!=NULL){
                next->previous = previous;
            }
        }
        else{
            perror("exitting");
            flag = 1;
        }
        // printf("pid is %d\n",copy->pid);
        copy = copy->next;
    }
    return flag;
}