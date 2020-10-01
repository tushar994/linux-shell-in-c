# tushell

## what is this?
This is a linux shell implemented in c

## Functionality
supports ';', piping and redirection (<, > , >>).

signals CTRL+Z and CTRL+C are handled.

The built in commands are :-- `ls (-al)`, `echo`, `pwd`, `cd`, `pinfo`, `history`, `nightswatch`(only `interrupt`), `setenv var [value]`, `unsetenv var`, `jobs` `kjob <job number> <signal number>` , `fg <job number>`, `bg <job number>`, `overkill`, `quit`.



it prints out a prompt in the form of `<username@system_name:curr_dir>`

can create background process using an `&` at the end of the command

displays `<command name> with pid <pid> exited with code <exit code>` when a background process ends


## RUNNING
to run
```shell
$ make
$ ./tushell
```

## how to exit
to exit 
```shell
$ exit
```
inside the program. This kills all bbackground processes created by the program and then terminates. ^C might not do the same and can sometimes give errors

## Files Wise code breakdown

- `shell.c` : this is where the shell runs from.
- `bg.c` : where background processes are created, and where jobs, kjob, fg, bg are implemented.
- `fg.c` : where the foreground process runs from
- `change_dir.c` : where cd is implemented
- `echo.c` : where echo is implemented.
- `exit.c` : where the quit and overkill is implemented
- `get_prompt.c` : where pwd and printing the prompt is implemented
- `history.c` : this creates a file called `history_tush.txt` WHEREVER THE TERMINAL IS RUN FROM. So if you run it from two different locations, it gives different histories for them.
- `ls.c` : where ls is outputted from. flags -l,-a,-la,-al are implemented. It outputs everything in a column (even if -l is not specified).
- `nightswatch.c` : where nightswatch is implemented. it exits the loop with "q\n". by default it executes every 5 seconds.
- `pinfo.c` : this is where pinfo is implemented


## in-build commands

-`pwd`, `cd`, `echo` are in-built. teh command `cd -` is valid and goes to the previous working directory.
- `ls [-la]` is in-built. Even if `[-l]` is not specified, it outputs the file names in a column.
- `pinfo [pid(optional)]` is in-built.
- `history [number (optional)]` is in-built. by default it gives the last 10. if it has to give n previous commands, but it has stored less commands, then it outputs all the commands it can output. Number has to be a two-digit (or one-digit) number.







