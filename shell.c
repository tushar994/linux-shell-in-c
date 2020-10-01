#include "headers.h"

// int current_fg_pid = -1;


int exec_command(char* input, char* starting_working_directory){
    // printf("what\n");
    char* words[1000];
    int length_current_command = strlen(input); 
    words[0] = strtok(input, " ");
    // index will ccontain the number of arguments +1
    int index = 1;
    while(words[index-1]!=NULL){
        words[index] = strtok(NULL, " ");
        index++;
    }
    if(words[0]==NULL){}
    else if(length_current_command==1){}
    else if(strcmp(words[0],"quit")==0){
        int outcome = exit_shell();
        if(outcome==0){
            exit(0);
        }
        else{
            fprintf(stderr,"couldnt quit\n");
        }
        // return 0;
    }
    else if(strcmp(words[0],"overkill")==0){
        int outcome = exit_shell();
        if(outcome!=0){
            fprintf(stderr,"couldnt kill all processes");
        }
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
        printf("bruh\n");
        list(words, index-1,starting_working_directory);
    }
    else if(strcmp(words[0],"pinfo")==0){
        print_pinfo(words, index-1, 0);
    }
    else if(strcmp(words[0],"nightswatch")==0){
        // printf("bruh\n");
        handle_night(words,index-1,starting_working_directory,input);
    }
    else if(strcmp(words[0],"setenv")==0){
        to_set_env(words,index-1);
    }
    else if(strcmp(words[0],"unsetenv")==0){
        to_unset_env(words,index-1);
    }
    else if(strcmp(words[0],"jobs")==0){
        // printf("jobs\n");
        jobs(words,index-1);
    }
    else if(strcmp(words[0],"kjob")==0){
        kjob(words,index-1);
    }
    else if(strcmp(words[0],"fg")==0){
        bring_fg(words,index-1);
    }
    else if(strcmp(words[0],"bg")==0){
        bring_bg(words,index-1);
    }
    
    else if(strcmp(words[0],"history")==0){
        int number = 10;
        int valid = 1;
        if(index-1>1){
            if(strlen(words[1])>2){
                valid = 0;
                fprintf(stderr,"not the right argument");
            }
            else if(strlen(words[1])==1){
                int ones = (int)words[1][0] - (int)'0';
                
                if(ones<0 || ones>9){
                    valid = 0;
                    fprintf(stderr,"not the right argument");
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
                    fprintf(stderr,"not the right argument");
                }
                else if(ones<0 || ones>9){
                    valid = 0;
                    fprintf(stderr,"not the right argument");
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
            bg(words, index-1,starting_working_directory,input);
        }
        
    }
}



int main(){
    our_gpid = (int*)malloc(sizeof(int));
    *our_gpid = getpid();
    second_dir_has_happened = (int*)malloc(sizeof(int));
    *second_dir_has_happened = 0;

    int Standard_in = dup(STDIN_FILENO);
    int Standard_out = dup(STDOUT_FILENO);
    current_fg_pid = (int*)malloc(sizeof(int));
    *current_fg_pid = -1;
    first_bg = (struct bg_process*)malloc(sizeof(struct bg_process));
    first_bg->previous = NULL;
    first_bg->next = NULL;
    first_bg->pid = -10;
    // first_bg->command = NULL;
    char starting_working_directory[1024];
    getcwd(starting_working_directory, 1024);
    // printf("%s",starting_working_directory);
    char okay[100];
    char* commmand_line_line;
    commmand_line_line = (char *)malloc(1000*sizeof(char));
    size_t linecap = 0;
    char* input[100];
    define_all_signals();
    // input = (char *)malloc(1000*sizeof(char));
    while(1){
        // printf("starting\n");
        print_prompt(starting_working_directory);

        int linelen = getline(&commmand_line_line, &linecap, stdin);
        if(linelen==-1){
            int outcome = exit_shell();
            if(outcome==0){
                return 0;
            }
            else{
                printf("couldn't exit\n");
            }
        }
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
        // input contains the the command separated by semicolons
        // now we take each of these, and seperate by 


        char* piped[1000];
        for(int w = 0;w<input_index;w++){
            piped[0] = strtok(input[w], "|");
            int piped_size=1;
            while(piped[piped_size-1]!=NULL){
                piped[piped_size] = strtok(NULL,"|");
                piped_size++;
            }
            piped_size--;
            // piped comtains the commands broken by pipes
            for(int pip=0;pip<piped_size;pip++){
                int flag[2];
                pipe(flag);
                if(pip<piped_size-1){
                    dup2(flag[1],STDOUT_FILENO);
                }
                else{
                    dup2(Standard_out,STDOUT_FILENO);
                }
                close(flag[1]);
                char input_file[1000],output_file[1000];
                int input_flag=0;
                int output_flag=0;
                


                int status = redirection(piped[pip],input_file, output_file, &input_flag, &output_flag);
                if(status!=1){
                    exec_command(piped[pip],starting_working_directory);
                }

                dup2(flag[0],STDIN_FILENO);
                close(flag[0]);

            }
            dup2(Standard_in,STDIN_FILENO);
            dup2(Standard_out,STDOUT_FILENO);
        }
    }
}