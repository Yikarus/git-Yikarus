OBJ=echoserver echoclient pollserver
all:$(OBJ)
echoserver:echoserver.c
	gcc -o $@ -g $<
echoclient:echoclient.c
	gcc -o $@ -g $<
pollserver:pollserver.c
	gcc -o $@ -g $<
clean:
	rm -f $(OBJ)
