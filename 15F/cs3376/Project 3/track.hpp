#ifndef _TRACK_HPP
#define _TRACK_HPP

#include <string>
using namespace std;
class Track
{
	int cd_id;
	int track_id;
	string title;
	public:
		Track(int i,int j, string str);
		void setCdId(int i);
		void setTrackId(int i);
		void setTitle(string str);
		int getCdId() const;
		int getTrackId() const;
		string getTitle() const;
};
#endif
