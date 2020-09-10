#ifndef __BG_H
#define __BG_H

struct bg_process{
    struct bg_process* previous;
    struct bg_process* next;
    int pid;
    char command[1024];
};
int bg(char* path[], int n,char* starting_working_directory,char* command);
void add_bg(pid_t pid, char* command);

#endif