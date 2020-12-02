all:server client
.PHONY:clean
server:server.cpp
	g++ -o server -g server.cpp
client:client.cpp
	g++ -o client -g client.cpp
clean:
	rm -f server client
