all:server client fork_server nettest
.PHONY:clean
server:server.cpp
	g++ -o $@ -g $<
client:client.cpp
	g++ -o $@ -g $<
fork_server:fork_server.cpp
	g++ -o client -g client.cpp
nettest:nettest.c
	gcc -o client -g client.cpp
clean:
	rm -f server client fork_server nettest
