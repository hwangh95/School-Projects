all: client server

client: socketclient.cpp
	g++ socketclient.cpp  -std=c++11 -lsqlite3 -o client -g
server: socketserver.cpp
	g++ socketserver.cpp  -std=c++11 -lsqlite3 -o server -g
clean:
	rm client server log.txt
clean-db:
	rm music.db
