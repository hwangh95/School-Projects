#include "track.hpp"

using namespace std;
Track::Track(int i, int j, string str)
{
	cd_id = i;
	track_id = j;
	title = str;
}

void Track::setCdId(int i){
	cd_id = i;
}
void Track::setTrackId(int i){
	track_id = i;
}
void Track::setTitle(string str){
	title = str;
}
int Track::getCdId() const{
	return cd_id;
}
int Track::getTrackId() const{
	return track_id;
}
string Track::getTitle() const{
	return title;
}
