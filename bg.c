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
    // setpgid(copy->pid, getpid());
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    // printf("%d\n",copy->pid);
    if(tcsetpgrp(STDIN_FILENO, copy->pid)<0){
        perror("fg");
        return 1;
    };
    *current_fg_pid = copy->pid;
    strcpy(fg_command,copy->command);
    kill(copy->pid, SIGCONT);
    char* pass[1];
    pass[0] = (char*)malloc(1000*sizeof(char));
    strcpy(pass[0],fg_command);
    pid_t okay = waitpid(copy->pid, &status, WUNTRACED);
    // printf("exitted with %d\n",status);
    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    if(WIFSTOPPED(status)){
        add_bg(*current_fg_pid,pass,1);
    }
    *current_fg_pid=-1;
    fg_command[0] = '\0';
    free(pass[0]);
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

void add_bg(int pid, char* command[] , int n){
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
    // printf("okay\n");
    // for(int i=0;i<n;i++){
    //     printf("%s ",command[i]);
    // }
    int total_length = 0;
    int lengthbruh = strlen(command[0]);
    total_length+= lengthbruh;
    strcpy(new->command, command[0]);
    new->command[lengthbruh] = ' ';
    total_length++;
    // printf("okay2\n");
    for(int i=1;i<n;i++){
        lengthbruh = strlen(command[i]);
        // printf("second : %d\n",lengthbruh);
        strcat(new->command,command[i]);
        total_length+=lengthbruh+1;
        new->command[total_length]= ' ';
    }
    // printf("%d\n",total_length);
    new->command[total_length] = '\0';
    // printf("%s\n",new->command);
    
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
        // printf("n is ; %d\n",n);
        add_bg(forkReturn,path,n-1);
        // setpgid(forkReturn,0);
        
        // kill(forkReturn, EOF );
        // pid_t okay = waitpid(forkReturn, &status, 0);
        // printf("what\n");
    }
    return 0; 
}