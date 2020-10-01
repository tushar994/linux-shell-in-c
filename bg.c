#include "headers.h"


// struct bg_process{
//     struct bg_process* prev;
//     struct bg_process* next;
//     int pid;
//     char command[1024];
// };
int bring_bg(char* path[], int n){
    if(n!=2){
        fprintf(stderr,"Not the right number of arguments\n");
        return 1;
    }
    struct bg_process* copy = first_bg;
    int index = 1;
    copy = copy->next;
    if(copy==NULL){
        fprintf(stderr,"not a valid job number\n");
        return 1;
    }
    int index_no = 0;
    int size1 = strlen(path[1]);
    for(int i=0;i<size1;i++){
        index_no*=10;
        index_no += (int)path[1][i] - (int)'0';
    }
    while(index!=index_no){
        copy = copy->next;
        index++;
        if(copy==NULL){
            fprintf(stderr,"not a valid job number\n");
            return 1;
        }
    }
    kill(copy->pid, SIGCONT);
    return 0;
}
int bring_fg(char* path[], int n){
    if(n!=2){
        fprintf(stderr,"Not the right number of arguments\n");
        return 1;
    }
    struct bg_process* copy = first_bg;
    int index = 1;
    copy = copy->next;
    if(copy==NULL){
        fprintf(stderr,"not a valid job number\n");
        return 1;
    }
    int index_no = 0;
    int size1 = strlen(path[1]);
    for(int i=0;i<size1;i++){
        index_no*=10;
        index_no += (int)path[1][i] - (int)'0';
    }
    while(index!=index_no){
        copy = copy->next;
        index++;
        if(copy==NULL){
            fprintf(stderr,"not a valid job number\n");
            return 1;
        }
    }

    int status;
    copy->previous->next =copy->next;
    if(copy->next!=NULL){
        copy->next->previous = copy->previous;
    }
    setpgid(copy->pid, getpid());
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if(tcsetpgrp(STDIN_FILENO, copy->pid)<0){
        perror("fg");
        return 1;
    };
    *current_fg_pid = copy->pid;
    strcpy(fg_command,copy->command);
    kill(copy->pid, SIGCONT);

    pid_t okay = waitpid(copy->pid, &status, WUNTRACED);
    *current_fg_pid=-1;
    fg_command[0] = '\0';
    // printf("exitted with %d\n",status);
    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    // printf("leaves this \n");
    return 0;
}
int kjob(char* path[], int n){
    if(n!=3){
        fprintf(stderr,"Not the right number of arguments\n");
        return 1;
    }
    struct bg_process* copy = first_bg;
    int index = 1;
    copy = copy->next;
    if(copy==NULL){
        fprintf(stderr,"not a valid job number\n");
        return 1;
    }
    int index_no = 0;
    int size1 = strlen(path[1]);
    for(int i=0;i<size1;i++){
        index_no*=10;
        index_no += (int)path[1][i] - (int)'0';
    }
    int size2 = strlen(path[2]);
    int signal=0;
    for(int i=0;i<size2;i++){
        signal*=10;
        signal += (int)path[2][i] - (int)'0';
    }
    while(index!=index_no){
        copy = copy->next;
        index++;
        if(copy==NULL){
            fprintf(stderr,"not a valid job number\n");
            return 1;
        }
    }
    kill(copy->pid, signal);
    return 0;

}
int jobs(char* path[], int n){
    struct bg_process* copy = first_bg;
    int index = 1;
    copy = copy->next;
    while(copy!=NULL){
        printf("[%d] ",index);
        index++;
        char* bruh[2];
        bruh[1] = (char*)malloc(100*sizeof(char));
        sprintf(bruh[1], "%d", copy->pid);
        print_pinfo(bruh, 2, 1);
        printf("%s [%d]\n", copy->command,copy->pid);
        copy = copy->next;
    }
}

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
        
        // kill(forkReturn, EOF );
        // pid_t okay = waitpid(forkReturn, &status, 0);
        // printf("what\n");
    }
    return 0; 
}