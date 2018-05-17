all: part1 part2 part3 part4 part5 part6 part7 part8

part1: sqlite-database_part1.cpp
	g++ -std=c++11 -o part1 sqlite-database_part1.cpp -g

part2: sqlite-database_part2.cpp
	g++ -std=c++11 -o part2 sqlite-database_part2.cpp -g -lsqlite3

part3: sqlite-database_part3.cpp
	g++ -std=c++11 -o part3 sqlite-database_part3.cpp -g -lsqlite3

part4: sqlite-database_part4.cpp
	g++ -std=c++11 -o part4 sqlite-database_part4.cpp -g -lsqlite3

part5: sqlite-database_part5.cpp
	g++ -std=c++11 -o part5 sqlite-database_part5.cpp -g -lsqlite3

part6: sqlite-database_part6.cpp
	g++ -std=c++11 -o part6 sqlite-database_part6.cpp artist.cpp track.cpp cd.cpp -g -lsqlite3

part7: sqlite-database_part7.cpp
	g++ -std=c++11 -o part7 sqlite-database_part7.cpp artist.cpp track.cpp cd.cpp -g -lsqlite3

part8: sqlite-database_part8.cpp
	g++ -std=c++11 -o part8 sqlite-database_part8.cpp artist.cpp track.cpp cd.cpp -g -lsqlite3

clean:
	rm -r ./part*
