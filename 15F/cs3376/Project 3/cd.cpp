#include "cd.hpp"
using namespace std;
Cd::Cd(int i, string str1, int j, string str2)
{
	id = i;
	title = str1;
	artist_Id = j;
	catalog = str2;
}

void Cd::setId(int i){
	id = i;
}
void Cd::setTitle(string str){
	title = str;
}
void Cd::setArtistId(int i){
	artist_Id = i;
}
void Cd::setCatalog(string str){
	catalog = str;
}
int Cd::getId() const{
	return id;
}
string Cd::getTitle() const{
	return title;
}
int Cd::getArtistId() const{
	return artist_Id;
}
string Cd::getCatalog() const{
	return catalog;
}
