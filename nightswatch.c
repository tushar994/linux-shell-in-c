#include "headers.h"




int delay(int seconds){
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
    int okay;
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) {
        okay = has_q_pressed();
        if(okay){
            return 1;
        }
    }
    return 0;
}



int handle_night(char* path[], int n,char* starting_working_directory,char* command){
    if(n==2){
        if(strcmp(path[1],"interrupt")==0){
            nighwatch_interrupt(5);
        }
        else if(strcmp(path[1],"newborn")==0){
            nighwatch_newborn(5);
        }
        else{
            fprintf(stderr,"not a valid command\n");
            return 1;
        }
    }
    else if(n==4){
        if(strcmp(path[1],"-n")!=0){
            fprintf(stderr,"not a right option\n");
            return 1;
        }
        int number = 0;
        int number_len = strlen(path[2]);
        for(int i=0;i<number_len;i++){
            number*=10;
            int digit = (int)path[2][i] - (int)'0';
            if(digit>9||digit<0){
                fprintf(stderr,"not a valid number");
                return 1;
            }
            number+=digit;
            // number*=10;
        }
        printf("%d\n",number);
        if(strcmp(path[3],"interrupt")==0){
            nighwatch_interrupt(number);
        }
        else if(strcmp(path[3],"newborn")==0){
            nighwatch_newborn(number);
        }
        else{
            fprintf(stderr,"not a valid command\n");
            return 1;
        }
    }
    else{
        fprintf(stderr,"wrong number of arguments\n");
        return 1;
    }
    // printf("exitted\n");
    return 0;

}


int has_q_pressed(){
    struct timeval tm;
    // set it to a total of 0 so that select does not wait
    tm.tv_sec = 0;
    tm.tv_usec = 0;


    fd_set fds;
    // initials it to represent STDIN?
    FD_ZERO(&fds);
    FD_SET(0, &fds);

    int return_val = select(1,&fds,NULL,NULL,&tm);
    if(return_val){
        char c = fgetc(stdin);
        // char d = fgetc(stdin);
        if(c=='q'){
            return 1;
        }
    }
    else if (return_val==-1){
        perror("q_press");
    }
    return 0;

}


int nighwatch_interrupt(int wait){
    FILE* f_read = fopen("/proc/interrupts", "r");
    if(f_read==NULL){
        perror("interrupts");
        return 1;
    }
    char store[1024];
    // prints the CPUS
    fgets(store, 1000, f_read);

    char* words;
    words = strtok(store, " ");
            
    while(words!=NULL){
        printf("%s\t",words);
        words = strtok(NULL, " ");
    }
    printf("\n");
    fclose(f_read);
    while(1){
        FILE* f_read = fopen("/proc/interrupts", "r");
        if(f_read==NULL){
            perror("interrupts");
            return 1;
        }
        while (fgets(store, 1000, f_read) != NULL){

            words = strtok(store, " ");
            if(strcmp(words, "1:") == 0){
                words = strtok(NULL, " ");
                while(words!=NULL && (words[0]>='0' && words[0]<='9')){
                    printf("%s\t",words);
                    words = strtok(NULL, " ");
                }
                printf("\n");
            }
        }
        fclose(f_read);
        int continue_maybe = delay(wait*1000);
        if(continue_maybe){
            return 0;
        }
    }
}


int nighwatch_newborn(int wait){
    while(1){
        FILE* f_read = fopen("/proc/loadavg", "r");
        if(f_read==NULL){
            perror("interrupts");
            return 1;
        }
        char store[1024];
        // prints the CPUS
        fgets(store, 1000, f_read);
        char* words[10];
        words[0] = strtok(store, " ");
        int index = 1;
        while(words[index-1]!=NULL){
            words[index] = strtok(NULL, " ");
            index++;
        }
        // for(int i=0;i<index-1;i++){
        //     printf("%s ",words[i]);
        // }

        printf("%s",words[index-2]);
        int continue_maybe = delay(wait*1000);
        if(continue_maybe){
            return 0;
        }
    }
}