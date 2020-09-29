main:
	gcc -o tushell -g -std=gnu99 shell.c get_prompt.c echo.c change_dir.c ls.c pinfo.c fg.c bg.c history.c exit.c nightswatch.c envset.c
