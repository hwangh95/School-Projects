#ifndef _ARTIST_HPP
#define _ARTIST_HPP

#include <string>
using namespace std;
class Artist
{
	int id;
	string name;
	public:
		Artist(int i, string str);
		void setId(int i);
		void setName(string str);
		int getId() const;
		string getName() const;
};
#endif
