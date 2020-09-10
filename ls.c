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
    struct tm time_last,time_present;
    time_t present = time(NULL);
    stat(name, &attrib);
    char time[100];
    localtime_r(&attrib.st_mtime, &time_last);
    localtime_r(&present, &time_present);
    if(time_last.tm_year!= time_present.tm_year){
        strftime(time, 50, "%b %d  %Y", localtime(&attrib.st_mtime));
    }
    else{
        strftime(time, 50, "%b %d %H:%M", localtime(&attrib.st_mtime));
    }
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
int check_if_path( int index, char* path[]){
    if(path[index][0]=='-'){
        int len = strlen(path[index]);
        if(len==2){
            if(path[index][1]=='a'){
                return 0;
            }  
            else if(path[index][1]=='l'){
                return 0;
            }
        }
        else if(len==3){
            if( (path[index][1]=='a' && path[index][2]=='l') || (path[index][2]=='a' && path[index][1]=='l') ){
                return 0;
            }
        }
    }
    return 1;
}

// prints size of the thing
int size_of(int flag_a, int flag_l ){
    struct dirent *de;
    DIR *dr = opendir("."); 
    long long int total =0;
    // printf("reached here 2\n");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        perror("ls :"); 
        return 0; 
    } 

    // for readdir() 
    while ((de = readdir(dr)) != NULL){
        // first we see if we need to print it
        if( !(!flag_a && de->d_name[0]=='.')  ){
            if(flag_l){
                struct stat a;
                stat(de->d_name, &a);
                total += a.st_blocks;
            }
        }
    } 
    total = total/2;
    printf("total %lld\n",total);
    // printf("flag_l : %d, flag_a : %d\n",flag_l,flag_a);
    closedir(dr); 
}

int print_it(int flag_a, int flag_l ){
    struct dirent *de;
    DIR *dr = opendir("."); 
    // printf("reached here 2\n");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        perror("ls :"); 
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
}


int list(char* path[], int n,char* starting_working_directory){
      // Pointer for directory entry 
    char starting_working[1024];
    getcwd(starting_working, sizeof(starting_working));
    // printf("%s\n",starting_working);


    int flag_a = 0;
    int flag_l= 0;
    int valid = 1;


    // update flag_a, flag_l for each argument
    for(int i=1;i<n;i++){
        check_la(&flag_l,&flag_a,i,path);
    }


    int direct_given = 0;
    for(int i=1;i<n;i++){

        int path_len = strlen(path[i]);
        int to_exec = 1;
        if(check_if_path(i,path)){
            // printf("reached here\n");
            direct_given = 1;
            char* okay[2];
            okay[1] = path[i];
            to_exec = change_dir(okay,2,starting_working_directory);
        }
        // printf("%d\n",to_exec);
        if(to_exec==0){
            // opendir() returns a pointer of DIR type. 
            printf("info for : %s\n",path[i]);  
            if(flag_l){
                size_of( flag_a,  flag_l );
            } 
            print_it( flag_a,  flag_l);
            chdir(starting_working);
        }
    }
    // printf("%d\n",direct_given);
    if(direct_given==0){
        if(flag_l){
            size_of( flag_a,  flag_l );
        }
        print_it( flag_a,  flag_l);
        chdir(starting_working);
    
    }
    return 0; 
};

