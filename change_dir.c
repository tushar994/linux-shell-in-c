#include "headers.h"

int change_dir(char* argument[], int n, char* starting_working_directory){
    if(n==1){
        chdir(starting_working_directory);
        return 0;
    }
    else if(n==2){
        if(argument[1][0]=='~'){
            int len = strlen(argument[1]);
            int len2 = strlen(starting_working_directory);
            char final[len+len2+5];
            for(int i=0;i<len2;i++){
                final[i]= starting_working_directory[i];
            }
            // printf("final is : %s\n",final);
            for(int i=1;i<len;i++){
                final[i+len2-1] = argument[1][i];
            }
            final[len+len2-1]= '\0';
            // printf("final is : %s\n",final);
            int error = chdir(final);
            if(error<0){
                perror(argument[1]);
                return 1;
            }
            return 0;
        }
        
        else{
            int len = strlen(argument[1]);
            
            int eroor = chdir(argument[1]);
            if(eroor<0){
                perror(argument[1]);
                return 1;
            }
            return 0;
        }
    }
    else
    {   
        printf("you ahve given too many arguments");
        return 1;
    }
    
}