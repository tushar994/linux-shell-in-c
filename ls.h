#ifndef __LS_H
#define __LS_H

int list(char* path[], int n,char* starting_working_directory);
int char_output(char to_print, int okay);
int check_if_dir(char* name);
int print_hardlinks(char* name);
int print_user_group(char* name);
int print_size(char* name);
int print_date(char*name);
int check_la(int* flag_l, int* flag_a, int index, char* path[]);
int check_if_path( int index, char* path[]);
int print_it(int flag_a, int flag_l);

#endif