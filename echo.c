#include"headers.h"

int echo_string(char* path[], int n, char* full_thing){
    
    if(n<2){
        return 0;
    }
    else{
        for(int i=1;i<n-1;i++){
            printf("%s ",path[i]);
        }
        printf("%s\n",path[n-1]);
        // printf("\n");
        return 0;
    }
}