all:server client processserver
.PHONY:clean
server:server.cpp
	g++ -o server -g server.cpp
client:client.cpp
	g++ -o client -g client.cpp
processserver:processserver.cpp
	g++ -o processserver -g processserver.cpp
clean:
	rm -f server client processserver
