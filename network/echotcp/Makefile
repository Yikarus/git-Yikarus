OBJ=echoserver echoclient pollserver ping epollserver reconnect.c
all:$(OBJ)
echoserver:echoserver.c
	gcc -o $@ -g $<
echoclient:echoclient.c
	gcc -o $@ -g $<
pollserver:pollserver.c
	gcc -o $@ -g $<
ping:ping.c
	gcc -o $@ -g $<
epollserver:epollserver.c
	gcc -o $@ -g $<
reconnect:reconnect.c
	gcc -o $@ -g $<
clean:
	rm -f $(OBJ)
