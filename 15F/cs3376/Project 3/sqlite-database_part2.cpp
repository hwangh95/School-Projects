#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

void tokenizer(ifstream &file, vector<vector<string>> &data);
void sqlExec(sqlite3 *db, string sql);
int main(int argc, char* argv[])
{
	sqlite3 *db;
	int rc;
	string sql;

	ifstream file;
	vector<vector<string>> data;//generic data vector
	
	rc = sqlite3_initialize();
	if(rc!=SQLITE_OK){fprintf(stderr, "Cannot initialize connection: %s\n", sqlite3_errmsg(db)); sqlite3_shutdown();exit(1);}
	rc = sqlite3_open("song.db", &db);
	if(rc){fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}
	

	sql = "CREATE TABLE IF NOT EXISTS artist(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(100) NOT NULL);";
	sqlExec(db,sql);
	
	file.open("artist.txt");
	if (!file){cerr<< "Cannot open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> artist : data){
		sql = "INSERT OR REPLACE INTO artist(id, name) values(\"" + artist[0] + "\", \"" + artist[1]+"\");";
		sqlExec(db,sql);
		cout<<sql<<endl;
	}

	sql = "CREATE TABLE IF NOT EXISTS cd(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, title VARCHAR(70) NOT NULL, artist_id INTEGER NOT NULL, catalog VARCHAR(30) NOT NULL);";
	sqlExec(db,sql);
	
	file.open("cd.txt");
	if (!file){cerr<< "Cannot open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> cd : data){
		sql = "INSERT OR REPLACE INTO cd(id, title, artist_id, catalog) values(" + cd[0] + ", \"" + cd[1] + "\", " + cd[2] + ", \"" + cd[3] + "\");";
		sqlExec(db,sql);
		cout<<sql<<endl;
	}

	sql = "CREATE TABLE IF NOT EXISTS track(cd_id INTEGER NOT NULL, track_id INTEGER NOT NULL, title VARCHAR(70) NOT NULL, PRIMARY KEY(cd_id, track_id));";
	sqlExec(db,sql);

	file.open("track.txt");
	if (!file){cerr<< "Can't open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> track : data){
		sql = "INSERT OR REPLACE INTO track(cd_id, track_id, title) values(" + track[0] + ", " + track[1] + ", \"" + track[2] + "\");";
		sqlExec(db,sql);
		cout<<sql<<endl;
	}
	sqlite3_close(db);
	data.clear();
}
void tokenizer(ifstream &file, vector<vector<string>> &data)
{
	data.clear();
	string str;
	while(getline(file,str,'\n')){
		vector<string> line;
		str.erase(0, str.find_first_not_of(" \f\n\r\t\v"));//trims whitespace
		str.erase(str.find_last_not_of(" \f\n\r\t\v") + 1 );
		int current;
		int next = -1;
		do{
			current = next + 1;
			next = str.find_first_of("|", current);
			line.push_back(str.substr(current, next - current));
		}
		while(next!= string::npos);
		data.push_back(line);
	}
	file.close();
}

void sqlExec(sqlite3 *db, string sql)
{
	int rc;
	sqlite3_stmt *query;
	rc = sqlite3_prepare_v2(db,sql.c_str(),-1, &query, NULL);
	if (rc!=SQLITE_OK){fprintf(stderr, "Cannot prepare: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	rc = sqlite3_step(query);
	if (rc!=SQLITE_ROW&&rc!=SQLITE_DONE){fprintf(stderr, "Cannot step: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}
}
