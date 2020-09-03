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
            chdir(final);
            return 0;
        }
        // else if(argument[1][0]=='.'){
        //     chdir(argument[1]);
        // }
        // else if(strcmp(argument[1],"..")==0){
        //     char cwd[1024];
        //     // gets the current directory
        //     getcwd(cwd, sizeof(cwd));
        //     int len = strlen(cwd);
        //     int index = len-1;
        //     while(cwd[index]!='/' && index>=0){
        //         index--;
        //     }
        //     if(index==-1){
        //         // printf("invalid");
        //     }
        //     else{
        //         cwd[index] = '\0';
        //         // printf("new place is %s\n",cwd);
        //         chdir(cwd);
        //     }
        //     return 0;
        // }
        else{
            int len = strlen(argument[1]);
            // printf("%s\n",argument[1]);
            chdir(argument[1]);
            return 0;
        }
    }
    else
    {
        return 1;
    }
    
}