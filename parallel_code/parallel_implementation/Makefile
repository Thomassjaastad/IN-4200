CC = mpicc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = parallel_main_15306  
OBJS = parallel_main_15306.o parallel_functions.o
all: simple-jpeg $(PROJ)

parallel_main_15306: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

simple-jpeg:
	$(MAKE) -C ../simple-jpeg

obj-clean:
	$(RM) *.o

exec-clean:
	$(RM) $(PROJ)

autosave-clean:
	$(RM) *~

clean:
	$(MAKE) obj-clean
	$(MAKE) exec-clean
	$(MAKE) autosave-clean
	$(MAKE) -C ../simple-jpeg clean
