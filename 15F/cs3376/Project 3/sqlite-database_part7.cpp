#include "artist.hpp"
#include "cd.hpp"
#include "track.hpp"
#include <sqlite3.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

void sqlExec(sqlite3 *db, string sql, string param);

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
	cout<<"Objects created"<<endl;
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
				cout<<"artist:\n"<<"id="<<artist.getId()<<"\nname="<<artist.getName()<<endl<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("cd"))
		{
			do
			{
				Cd cd(sqlite3_column_int(query,0),string(reinterpret_cast<const char*>(sqlite3_column_text(query,1))),sqlite3_column_int(query,2),string(reinterpret_cast<const char*>(sqlite3_column_text(query,3))));
				cout<<"cd:\n"<<"id="<<cd.getId()<<"\ntitle="<<cd.getTitle()<<"\nartist_id="<<cd.getArtistId()<<"\ncatalog="<<cd.getCatalog()<<endl<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("track"))
		{
			do
			{
				Track track(sqlite3_column_int(query,0),sqlite3_column_int(query,1),string(reinterpret_cast<const char*>(sqlite3_column_text(query,2))));
				cout<<"track:\n"<<"cd_id="<<track.getCdId()<<"\ntrack_id="<<track.getTrackId()<<"\ntitle="<<track.getTitle()<<endl<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}	
}
