#ifndef __BG_H
#define __BG_H

struct bg_process{
    struct bg_process* previous;
    struct bg_process* next;
    int pid;
    long long int job_num;
    char command[1024];
};
int bring_bg(char* path[], int n);
int bring_fg(char* path[], int n);
int kjob(char* path[], int n);
int jobs(char* path[], int n);
int bg(char* path[], int n,char* starting_working_directory,char* command);
void add_bg(pid_t pid, char* command);

#endif