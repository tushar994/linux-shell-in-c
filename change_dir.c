#include "headers.h"

int change_dir(char* argument[], int n, char* starting_working_directory){
    if(n==1){
        char ccwd[1024];
        getcwd(ccwd, 1024);
        chdir(starting_working_directory);
        if(*second_dir_has_happened==0){
            *second_dir_has_happened=1;
        }
        strcpy(last_working_dir_that_was_in,ccwd);
        return 0;
    }
    else if(n==2){
        if(argument[1][0]=='~'){
            char ccwd[1024];
            getcwd(ccwd, 1024);
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
            strcpy(last_working_dir_that_was_in,ccwd);
            *second_dir_has_happened=1;
            return 0;
        }
        else if(argument[1][0]=='-'){
            if(strlen(argument[1])>1){
                fprintf(stderr,"invalid path\n");
                return 1;
            }
            if(*second_dir_has_happened==0){
                fprintf(stderr,"cant do this at the current stage\n");
                return 1;
            }
            char ccwd[1024];
            getcwd(ccwd, 1024);
            chdir(last_working_dir_that_was_in);
            print_pwd(starting_working_directory);
            printf("\n");
            strcpy(last_working_dir_that_was_in,ccwd);
            return 0;
        }
        
        else{
            char ccwd[1024];
            getcwd(ccwd, 1024);
            int len = strlen(argument[1]);
            
            int eroor = chdir(argument[1]);
            if(eroor<0){
                perror(argument[1]);
                return 1;
            }
            *second_dir_has_happened = 1;
            strcpy(last_working_dir_that_was_in,ccwd);
            return 0;
        }
    }
    else
    {   
        fprintf(stderr,"you have given too many arguments");
        return 1;
    }
    
}