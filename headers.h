#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h> 
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <time.h> 
#include <sys/select.h>

#include "get_prompt.h"
#include "echo.h"
#include "change_dir.h"
#include "ls.h"
#include "pinfo.h"
#include "bg.h"
#include "fg.h"
#include "history.h"
#include "exit.h"
#include "nightswatch.h"

struct bg_process* first_bg;

#endif