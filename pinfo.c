#include "headers.h"

int print_pinfo(char* path[], int n){
    char stat_path[1000];
    strcpy(stat_path,"/proc/");
    if(n==1){
        char cat[100];
        int pid = getpid();
        sprintf(cat, "%d", pid); 
        strcat(stat_path,cat);
    }
    else if(n==2){
        strcat(stat_path,path[1]);
    }
    else{
        printf("invalid number of arguments\n");
        return 1;
    }

    strcat(stat_path, "/stat");
    FILE* stat_line = fopen(stat_path, "r"); 
    // printf("%s\n",stat_path);
    if(stat_line==NULL){
        perror("pinfo: ");
        return 1;
    }

    char stat_info[1000];

    char* okay = fgets(stat_info, 1000, stat_line );
    if(okay==NULL){
        perror("pinfo");
        return 1;
    }

    char* words[100];
    words[0] = strtok(stat_info, " ");
    // index will ccontain the number of arguments +1
    int index = 1;
    while(words[index-1]!=NULL){
        words[index] = strtok(NULL, " ");
        index++;
    }

    printf("pid --- %s\n",words[0]);
    printf("Process Status -- %s\n",words[2]);
    printf("memory -- %s\n", words[22]);

    int len = strlen(stat_path);
    stat_path[len-4] = '\0';

    strcat(stat_path,"exe");


    char path_exe[1000];

    int size_exe = readlink(stat_path, path_exe, 1000);
    if(size_exe <0){
        perror("pinfo: ");
        return 1;
    }
    path_exe[size_exe]='\0';
    printf("Executable Path -- %s\n",path_exe);
    return 0;

}