#include "headers.h"

int redirection(char* command, char* input_file, char* output_file, int* input, int* output){
    char* tokens[100];
    char copy_of_command[1000];
    strcpy(copy_of_command,command);
    tokens[0] = strtok(copy_of_command," ");
    int index = 1;
    while(tokens[index-1]!=NULL){
        tokens[index] = strtok(NULL," ");
        index++;
    }
    
    index--;

    // printf("%s\n",command);
    for(int i=0;i<index;i++){
        int len = strlen(tokens[i]);
        // printf("%d\n",i);
        if(tokens[i][0]=='<'){
            if(len==1){
                if(tokens[i+1]==NULL){
                    input_file[0] = '\0';
                }
                else{
                    strcpy(input_file,tokens[i+1]);
                }
            }
            else{
                strcpy(input_file,tokens[i]+1);
            }
            // printf("goes into %d\n",i);
            *input = 1;
            int f_input = open(input_file, O_RDONLY); 
                    
            if(f_input==-1){
                perror("input file");
                return 1;
            }
            else{
                if(dup2(f_input, STDIN_FILENO)==-1){
                    perror("input");
                    return 1;
                }
            }
        }
        if(len>=2 && tokens[i][0]=='>' && tokens[i][1]=='>'){
            if(len==2){
                if(tokens[i+1]==NULL){
                    output_file[0] = '\0';
                }
                else{
                    strcpy(output_file,tokens[i+1]);
                }   
            }
            else{
                strcpy(output_file,tokens[i]+2);
            }
            *output = 2;
        }
        else if(tokens[i][0]=='>'){
            if(len==1){
                if(tokens[i+1]==NULL){
                    output_file[0] = '\0';
                }
                else{
                    strcpy(output_file,tokens[i+1]);
                }   
            }
            else{
                strcpy(output_file,tokens[i]+1);
            }
            *output = 1;
        }   
        if(*output==1 || *output==2){
            int f_output;
            if(*output==1){
                f_output = open(output_file, O_TRUNC | O_WRONLY | O_CREAT , 0644); 
            }
            else if(*output==2){
                f_output = open(output_file,  O_WRONLY | O_CREAT| O_APPEND , 0644);

            }
            if(f_output==-1){
                perror("output file");
                return 1;
            }
            else{
                if(dup2(f_output, STDOUT_FILENO)==-1){
                    perror("output");
                    return 1;
                }
            }
        }
        // printf("%d\n",i);
    }
    // printf("%s\n",command);
    int total_len = strlen(command);

    for(int i=0;i<total_len;i++){
        int prev = 0;
        if(command[i]=='<' || command[i]=='>'){
            // printf("going : %d\n",i);
            if(command[i+1]=='\0'|| command[i+1]==' ' || (command[i]=='>' && command[i+1]=='>')){
                while(command[i]!=' ' && command[i]!='\0'){
                    command[i]= ' ';
                    i++;
                }
                while(command[i]==' ' && command[i]!='\0'){
                    i++;
                }
                while(command[i]!=' ' && command[i]!='\0'){
                    command[i]=' ';
                    i++;
                }
                
            }
        }
    }
    // printf("%s\n",command);
}