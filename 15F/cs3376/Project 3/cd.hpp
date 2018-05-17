#ifndef _CD_HPP
#define _CD_HPP

#include <string>
using namespace std;
class Cd
{
	int id;
	string title;
	int artist_Id;
	string catalog;
	public:
		Cd(int i, string str1,int j,string str2);
		void setId(int i);
		void setTitle(string str);
		void setArtistId(int i);
		void setCatalog(string str);
		int getId() const;
		string getTitle() const;
		int getArtistId() const;
		string getCatalog() const;
};
#endif
