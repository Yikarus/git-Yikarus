OBJ=sig thread mutex
all:$(OBJ)
.PHONY:clean
sig:sig.c
	gcc -o $@ -g $<
thread:thread.c
	gcc -o $@ -g $< -lpthread
mutex:mutex.c
	gcc -o $@ -g $< -lpthread
clean:
	rm -f $(OBJ)
