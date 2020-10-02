#include "headers.h"



int to_set_env(char* path[], int n){
    if(n!=2 && n!=3){
        fprintf(stderr,"incorrect number of aruments\n");
    }
    else{
        char bruh[1000];
        if(n==2){
            if(setenv(path[1], "\0", 1)==-1){
                perror("setenv");
                return 1;
            }
        }
        else{
            if(setenv(path[1], path[2], 1)==-1){
                perror("setenv");
                return 1;
            }
        }
    }
    return 0;
}

int to_unset_env(char* path[], int n){
    if(n!=2){
        fprintf(stderr,"incorrect number of aruments\n");
        return 1;
    }
    else{
        if(unsetenv(path[1])==-1){
            perror("unsetenv");
            return 1;
        }
    }
    return 0;
}