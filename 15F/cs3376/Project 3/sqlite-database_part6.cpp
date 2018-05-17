#include "artist.hpp"
#include "cd.hpp"
#include "track.hpp"
#include <sqlite3.h> 
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char* argv[])
{
	sqlite3 *db;
	int rc;
	string sql;
	Artist artist(0,"Name");
	Cd cd(0, "Title", 0, "Catalog");
	Track track(0, 0, "Title");
	cout<<"Sample data"<<endl;
	cout<<"Artist artist(0,\"Name\");"<<endl;
	cout<<"Cd cd(0, \"Title\", 0, \"Catalog\");"<<endl;
	cout<<"Track track(0, 0, \"Title\");"<<endl;
	cout<<endl;
	cout<<"artist:\n"<<"id="<<artist.getId()<<"\nname="<<artist.getName()<<endl<<endl;
	cout<<"cd:\n"<<"id="<<cd.getId()<<"\ntitle="<<cd.getTitle()<<"\nartist_id="<<cd.getArtistId()<<"\ncatalog="<<cd.getCatalog()<<endl<<endl;
	cout<<"track:\n"<<"cd_id="<<track.getCdId()<<"\ntrack_id="<<track.getTrackId()<<"\ntitle="<<track.getTitle()<<endl;
	return 0;
}
