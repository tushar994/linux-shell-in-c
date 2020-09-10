#include "headers.h"




int main(){
    
    first_bg = (struct bg_process*)malloc(sizeof(struct bg_process));
    first_bg->previous = NULL;
    first_bg->next = NULL;
    first_bg->pid = -1;
    // first_bg->command = NULL;
    char starting_working_directory[1024];
    getcwd(starting_working_directory, 1024);
    // printf("%s",starting_working_directory);
    char okay[100];
    char* commmand_line_line;
    commmand_line_line = (char *)malloc(1000*sizeof(char));
    size_t linecap = 0;
    char* input[100];
    // input = (char *)malloc(1000*sizeof(char));
    while(1){
        // printf("starting\n");
        print_prompt(starting_working_directory);

        int linelen = getline(&commmand_line_line, &linecap, stdin);
        int length_input = strlen(commmand_line_line);

        commmand_line_line[length_input-1] = '\0';

        add_history(commmand_line_line,starting_working_directory);
        input[0] = strtok(commmand_line_line,";");
        int input_index = 1;
        while(input[input_index-1]!=NULL){
            input[input_index] = strtok(NULL,";");
            input_index++;
        }
        input_index--;
        for(int w = 0;w<input_index;w++){
            char* words[1000];
            words[0] = strtok(input[w], " ");
            // index will ccontain the number of arguments +1
            int index = 1;
            while(words[index-1]!=NULL){
                words[index] = strtok(NULL, " ");
                index++;
            }
            if(length_input==1){}
            else if(strcmp(words[0],"exit")==0){
                int outcome = exit_shell();
                if(outcome==0){
                    return 0;
                }
                else{
                    printf("couldnt exit\n");
                }
                // return 0;
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
                // if(k==1){
                //     printf("too many arguments");
                // }
            }
            else if(strcmp(words[0],"ls")==0){
                list(words, index-1,starting_working_directory);
            }
            else if(strcmp(words[0],"pinfo")==0){
                print_pinfo(words, index-1);
            }
            else if(strcmp(words[0],"nightswatch")==0){
                // printf("bruh\n");
                handle_night(words,index-1,starting_working_directory,input[w]);
            }
            
            else if(strcmp(words[0],"history")==0){
                int number = 10;
                int valid = 1;
                if(index-1>1){
                    if(strlen(words[1])>2){
                        valid = 0;
                        printf("not the right argument");
                    }
                    else if(strlen(words[1])==1){
                        int ones = (int)words[1][0] - (int)'0';
                        
                        if(ones<0 || ones>9){
                            valid = 0;
                            printf("not the right argument");
                        }
                        else{
                            number =  ones;
                        }
                    }
                    else{
                        int tens = (int)words[1][0] - (int)'0';
                        int ones = (int)words[1][1] - (int)'0';
                        if(tens<0 || tens>9){
                            valid = 0;
                            printf("not the right argument");
                        }
                        else if(ones<0 || ones>9){
                            valid = 0;
                            printf("not the right argument");
                        }
                        else{
                            number = tens*10 + ones;
                        }

                    }
                }
                if(valid){
                    print_history( number, starting_working_directory);
                }
            }
            else{
                if(strcmp(words[index-2] , "&")!=0){
                    // for(int i=0;i<2;i++){
                    //     printf("%s\n",words[i]);
                    // }
                    // printf("fg");
                    // printf("%s\n",input);
                    fg(words, index-1,starting_working_directory);
                }
                else{
                    // printf("back\n");
                    bg(words, index-1,starting_working_directory,input[w]);
                }
                
            }
        }
    }
}