#ifndef __NIGHTSWATCH_H
#define __NIGHTSWATCH_H

int handle_night(char* path[], int n,char* starting_working_directory,char* command);
int nighwatch_interrupt(int wait);
int delay(int seconds);
int has_q_pressed();

#endif