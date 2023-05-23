CFLAGS = -O3 -g
SOURCES = deps/cmds.c deps/cmprss.c deps/timecalc.c
DEPS = -lavformat -lavcodec -lavutil -lswscale 

main: main.c $(SOURCES)
	$(CC) $(CFLAGS) -O3	-o main main.c $(SOURCES) $(DEPS)
