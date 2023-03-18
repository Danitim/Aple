CC = gcc
CFLAGS = 
SOURCES = cmds.c cmprss.c timecalc.c
DEPS = -lavformat -lavcodec -lavutil -lswscale 

main: main.c $(SOURCES)
	$(CC) -o main main.c $(SOURCES) $(DEPS)
