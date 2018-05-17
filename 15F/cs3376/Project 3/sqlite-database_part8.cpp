#include "artist.hpp"
#include "cd.hpp"
#include "track.hpp"
#include <sqlite3.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
using namespace std;

void sqlExec(sqlite3 *db, string sql, string param);
static map <int,Artist>artistMap;
static map <int,Cd>cdMap;
static map <pair<int,int>,Track>trackMap;

int main(int argc, char* argv[])
{
	sqlite3 *db;
	int rc;
	string sql,param;

	rc = sqlite3_initialize();
	if(rc!=SQLITE_OK){fprintf(stderr, "Cannot initialize connection: %s\n", sqlite3_errmsg(db)); sqlite3_shutdown();exit(1);}
	rc = sqlite3_open("song.db", &db);
	if(rc){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}
	else{fprintf(stderr, "Opened database successfully\n");}

	param = "artist";
	sql = "SELECT * FROM  " + param + ";";
	sqlExec(db,sql,param);
	param = "cd";
	sql = "SELECT * FROM  " + param + ";";
	sqlExec(db,sql,param);
	param = "track";	
	sql = "SELECT * FROM  " + param + ";";
	sqlExec(db,sql,param);
	
	for(auto it = artistMap.cbegin(); it != artistMap.cend();++it)
	{
		cout<<"Id: "<<it->first<<" Artist("<<it->second.getId()<< "," << it->second.getName()<<")"<<endl; 
	}
	for(auto it = cdMap.cbegin(); it != cdMap.cend();++it)
	{
		cout<<"Id: "<<it->first<<" Cd("<<it->second.getId()<< "," << it->second.getTitle()<< "," << it->second.getArtistId() << "," << it->second.getCatalog()<<")"<<endl; 
	}
	for(auto it = trackMap.cbegin(); it != trackMap.cend();++it)
	{
		cout<<"Id: ("<<it->first.first << "," << it->first.second <<") Track("<<it->second.getCdId()<< "," << it->second.getTrackId() << ","  << it->second.getTitle()<<")"<<endl; 
	}
	sqlite3_close(db);
	return 0;
}

void sqlExec(sqlite3 *db, string sql, string param)
{
	int rc;
	sqlite3_stmt *query;
	rc = sqlite3_prepare_v2(db,sql.c_str(),-1, &query, NULL);
	if (rc!=SQLITE_OK){fprintf(stderr, "Cannot prepare: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	rc = sqlite3_step(query);
	if (rc!=SQLITE_ROW&&rc!=SQLITE_DONE){fprintf(stderr, "Cannot step: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	if (!sql.substr(0,sql.find_first_of(" ")).compare("SELECT"))
	{
		if (!param.compare("artist"))
		{
			do
			{
				Artist artist(sqlite3_column_int(query,0),string(reinterpret_cast<const char*>(sqlite3_column_text(query,1))));
				artistMap.insert(pair<int,Artist>(artist.getId(),artist));
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("cd"))
		{
			do
			{
				Cd cd(sqlite3_column_int(query,0),string(reinterpret_cast<const char*>(sqlite3_column_text(query,1))),sqlite3_column_int(query,2),string(reinterpret_cast<const char*>(sqlite3_column_text(query,3))));
				cdMap.insert(pair<int,Cd>(cd.getId(),cd));
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("track"))
		{
			do
			{
				Track track(sqlite3_column_int(query,0),sqlite3_column_int(query,1),string(reinterpret_cast<const char*>(sqlite3_column_text(query,2))));
				trackMap.insert(pair<pair<int,int>,Track>(pair<int,int>(track.getCdId(),track.getTrackId()),track));
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}	
}
