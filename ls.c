#include "headers.h"

// returns 1 if it is a dir else 0
int check_if_dir(char* name){
    struct stat file1Stat;

    int okay1 = stat(name, &file1Stat);
    if(okay1==0 && S_ISDIR(file1Stat.st_mode)){
        printf("d");
        return 1;
    }
    else{
        printf("-");
        return 1;
    }
}


int char_output(char to_print, int okay){
    if(okay){
        printf("%c",to_print);
    }
    else{
        printf("-");
    }
    return 1;

}

int print_permissions(char* name){
    struct stat fileStat;
    int okay1 = stat(name, &fileStat);

    char_output('r',(fileStat.st_mode & S_IRUSR));
    char_output('w',(fileStat.st_mode & S_IWUSR));
    char_output('x',(fileStat.st_mode & S_IXUSR));    

    //for group
    char_output('r',(fileStat.st_mode & S_IRGRP));
    char_output('w',(fileStat.st_mode & S_IWGRP));
    char_output('x',(fileStat.st_mode & S_IXGRP));

    //for others 
    char_output('r',(fileStat.st_mode & S_IROTH));
    char_output('w',(fileStat.st_mode & S_IWOTH));
    char_output('x',(fileStat.st_mode & S_IXOTH));
    return 0;
}
int print_hardlinks(char* name){
    struct stat fileStat;
    int okay1 = stat(name, &fileStat);

    printf("%d\t", fileStat.st_nlink);
    return 0;
}
int print_user_group(char* name){
    struct stat fileStat;
    int okay1 = stat(name, &fileStat);
     //For the user id. 
    // statBuf.st_gid;
    struct passwd *pws;
    pws = getpwuid(fileStat.st_uid);
    printf("%s\t",pws->pw_name);
    struct group *pws2;
    // printf("bruh\n");
    pws2 = getgrgid(fileStat.st_gid);
    printf("%s\t",pws2->gr_name);
    return 0;
}

int print_size(char* name){
    struct stat fileStat;
    int okay1 = stat(name, &fileStat);
    
    printf("%lld\t",fileStat.st_size);
    return 0;
}

int print_date(char*name){
    struct stat attrib;
    stat(name, &attrib);
    char time[100];
    strftime(time, 50, "%b %d %H:%M", localtime(&attrib.st_mtime));
    printf ("%s\t", time);
    return 0;
}

int check_la(int* flag_l, int* flag_a, int index, char* path[]){
    if(path[index][0]=='-'){
        int len = strlen(path[index]);
        if(len==2){
            if(path[index][1]=='a'){
                *flag_a = 1;
            }  
            else if(path[index][1]=='l'){
                *flag_l=1;
            }
        }
        else if(len==3){
            if( (path[index][1]=='a' && path[index][2]=='l') || (path[index][2]=='a' && path[index][1]=='l') ){
                *flag_l = 1;
                *flag_a = 1;
            }
        }
    }
    return 1;
}



int list(char* path[], int n,char* starting_working_directory){
    struct dirent *de;  // Pointer for directory entry 
    char starting_working[1024];
    getcwd(starting_working, sizeof(starting_working));


    int flag_a = 0;
    int flag_l= 0;
    int valid = 1;

    



    // checking for flags
    if(n>1){
    // for the ".. -l" or ". -l" case (its so gay OMG)
        check_la(&flag_l, &flag_a, 1, path);
        if(n>2){
            check_la(&flag_l, &flag_a, 2, path);
            int broo = strlen(path[1]);
            if(broo<=2 && path[1][0]=='.'){
                if(broo==2 && path[1][1]=='.'){
                    chdir("..");
                }
                if(n>3){
                    check_la(&flag_l, &flag_a, 3, path);
                }
            }
        }
    }
    if(n>4){
        printf("wrong number of arguments");
        return 1;
    }

    // printf("l: %d, a: %d\n",flag_l,flag_a);
    // to get the path if it exists
    if(n>1){
        int path_len = strlen(path[n-1]);
        if(path_len>3 || path[n-1][0]!='-'){
            char* okay[2];
            okay[1] = path[n-1];
            change_dir(okay,2,starting_working_directory);
        }
    }

    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // for readdir() 
    while ((de = readdir(dr)) != NULL){
        // first we see if we need to print it
        if( !(!flag_a && de->d_name[0]=='.')  ){
            if(flag_l){
                // checks if its a directory and if it is, then it prints a d else prints a -
                check_if_dir(de->d_name);
                // prints permissions
                print_permissions(de->d_name);
                printf("\t");
                // prints the number of hard links
                print_hardlinks(de->d_name);

                // print username and groupname
                print_user_group(de->d_name);

                // print size of file
                print_size(de->d_name);

                // print date of last modification
                print_date(de->d_name);
            }




            //if we need l then each one gets it's own thing
            // if(flag_l){
                printf("%s\n", de->d_name);
                // printf("\n");
            // }
            // else{
                // printf("%s\t", de->d_name);
                // printf("\t");
            // }
        }
    } 
    printf("\n");
    // printf("flag_l : %d, flag_a : %d\n",flag_l,flag_a);
    closedir(dr);     
    chdir(starting_working_directory);
    return 0; 
};

