#include "headers.h"




int add_history(char* command, char* starting_working_directory){
    
    char path[1000];
    strcpy(path,starting_working_directory);
    strcat(path, "/history_tush.txt");
    
    int f_read = open(path, O_RDWR | O_CREAT, 0600); 
    if(f_read<0){
        perror("history");
        return 1;
    }
    long long int size = lseek(f_read, 0, SEEK_END);
    lseek(f_read, 0, SEEK_SET);

    if(size<0){
        perror("r1");
        return 1;
    }

    char contents[size+5];
    read(f_read, contents, size);

    int lines = 0;
    for(int i=0;i<size;i++){
        if(contents[i]=='\n'){
            lines++;
        }
    }
    // lines++;
    if(lines<20){
        lseek(f_read, 0, SEEK_END);
        int size2 = strlen(command);
        write (f_read, command, size2);
        write(f_read,"\n",1);
        close(f_read);

    }
    else{
        close(f_read);
        int f_read = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
        // printf("mor\n");
        int index =0;
        while(contents[index]!='\n'){
            index++;
        }
        index++;
        size -= index;
        write (f_read, contents + index, size);
        
        int size2 = strlen(command);
        write (f_read, command, size2);
        write(f_read,"\n",1);
        close(f_read);

    }


    return 0;



}
int print_history( int n,char* starting_working_directory){
    char path[1000];
    strcpy(path,starting_working_directory);
    strcat(path, "/history_tush.txt");
    
    int f_read = open(path, O_RDWR | O_CREAT, 0600); 
    if(f_read<0){
        perror("history");
        return 1;
    }
    long long int size = lseek(f_read, 0, SEEK_END);
    lseek(f_read, 0, SEEK_SET);

    if(size<0){
        perror("r1");
        return 1;
    }

    char contents[size+5];
    read(f_read, contents, size);

    int lines = 0;
    for(int i=0;i<size;i++){
        if(contents[i]=='\n'){
            lines++;
        }
    }

    contents[size] = '\0';
    if(lines<=n){
        printf("%s",contents);
    }
    else{
        int skip = lines-n;
        int index = 0;
        for(int i=0;i<skip;i++){
            while(contents[index]!='\n'){
                index++;
            }
            index++;
        }
        printf("%s",contents+index);
    }
    close(f_read);
}