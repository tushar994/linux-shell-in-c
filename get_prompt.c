#include "headers.h"

// this file is used to get the username and the systemname
// the given functions return char pointers that lead to te appropriate name
char* find_username() {
    // this prints the username
    char *buf;
    buf= (char *)malloc(100*sizeof(char));
    buf= getlogin();
    printf("%s",buf);
    // free(buf);
    return buf;
}

// this printst he systemname
int find_systemname(){
    struct utsname buffer;
    // char *buf;
    // buf= (char *)malloc(100*sizeof(char));
    // gethostname(buf,100);
    int k = uname(&buffer);
    // gethostname(buf, sizeof(buf));
    // buf = buffer.sysname;
    // printf("%s",buf);
    printf("%s",buffer.nodename);

    return 1;
}


// this implements pwd command
void print_pwd(char* starting_working_directory){
    char cwd[1024];
    // gets the current directory
    getcwd(cwd, sizeof(cwd));
    // gets the sizes
    int size1 = strlen(cwd);
    int size2 = strlen(starting_working_directory);

    if(size1<size2){
       printf(" %s",cwd); 
    }
    else{
        int contains =1;
        for(int i=0;i<size2;i++){
            if(cwd[i]!=starting_working_directory[i]){
                contains = 0;
                break;
            }
        }
        // if the cwd contains ~
        if(contains){
            if(size1==size2){
                printf("~");
            }
            else{
                printf(" ~%s",cwd+size2);
            }
        }
        else{
            printf(" %s",cwd); 
        }
    }
}



// this uses the above functions to print the prompt
void print_prompt(char* starting_working_directory){
    
    find_username();
    printf("@");
    find_systemname();

    print_pwd(starting_working_directory);
    // printf(" %s",starting_working_directory);
    printf(" >\n");
}

