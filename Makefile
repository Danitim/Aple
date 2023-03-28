CFLAGS = -O3
SOURCES = cmds.c cmprss.c timecalc.c
DEPS = -lavformat -lavcodec -lavutil -lswscale 

main: main.c $(SOURCES)
	$(CC) $(CFLAGS) -o main main.c $(SOURCES) $(DEPS)
