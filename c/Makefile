
CFLAGS= -O -g -ggdb -fPIC
OBJS= putils.o program_options.o popts_test.o

phony:  $(OBJS)
	$(CC) -g -ggdb -fPIC -o test $(OBJS)
	$(CC) -shared -o libputils_popts.so $(OBJS)

clean:
	rm -fr *.o
	rm -fr test
     
