OBJ=echoserver echoclient
all:$(OBJ)
echoserver:echoserver.c
	gcc -o $@ -g $<
echoclient:echoclient.c
	gcc -o $@ -g $<
clean:
	rm -f $(OBJ)
