#include "headers.h"



int main(){
    char starting_working_directory[1024];
    getcwd(starting_working_directory, sizeof(starting_working_directory));
    // printf("%s",starting_working_directory);
    char okay[100];
    char* input;
    size_t linecap = 0;
    input = (char *)malloc(100*sizeof(char));
    while(1){
        print_prompt(starting_working_directory);

        int linelen = getline(&input, &linecap, stdin);
        int length_input = strlen(input);
        input[length_input-1] = '\0';
        char* words[1000];
        words[0] = strtok(input, " ");
        // index will ccontain the number of arguments +1
        int index = 1;
        while(words[index-1]!=NULL){
            words[index] = strtok(NULL, " ");
            index++;
        }
        if(strcmp(words[0],"exit")==0){
            return 0;
        }
        else if(strcmp(words[0],"pwd")==0){
            print_pwd(starting_working_directory);
            printf("\n");
        }
        else if(strcmp(words[0],"echo")==0){
            echo_string(words, index-1, input);
        }
        else if(strcmp(words[0],"cd")==0){
            int k = change_dir(words, index-1,starting_working_directory);
            if(k==1){
                printf("too many arguments");
            }
        }
        else if(strcmp(words[0],"ls")==0){
            list(words, index-1,starting_working_directory);
        }
        else if(strcmp(words[0],"pinfo")==0){
            print_pinfo(words, index-1);
        }
        else{

            int status;
            pid_t forkReturn = fork();
            if(forkReturn <0){
                printf("invalid\n");
                // return 1;
            }
            else if (forkReturn == 0){
                // child
                execvp(words[0],words);
                return 0;
            }
            else{
                // parentWUNTRACED
                pid_t okay = waitpid(forkReturn, &status,  WNOHANG );
                printf("%d\n",okay);
            }
        }
    }
}