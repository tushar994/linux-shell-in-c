#ifndef __LS_H
#define __LS_H

int list(char* path[], int n,char* starting_working_directory);
int char_output(char to_print, int okay);
int check_if_dir(char* name);
int print_hardlinks(char* name);
int print_user_group(char* name);
int print_size(char* name);
int print_date(char*name);

#endif