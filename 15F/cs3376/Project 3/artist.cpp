#include "artist.hpp"

using namespace std;
Artist::Artist(int i, string str)
{
	id = i;
	name = str;
}

void Artist::setId(int i){
	id = i;
}
void Artist::setName(string str){
	name = str;
}
int Artist::getId() const{
	return id;
}
string Artist::getName() const{
	return name;
}
