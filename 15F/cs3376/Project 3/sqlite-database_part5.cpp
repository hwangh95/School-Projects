#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void sqlExec(sqlite3 *db, string sql, vector<string>&output);

int main(int argc, char* argv[])
{
	sqlite3 *db;
	int rc;
	string sql;
	vector<string>output;

	rc = sqlite3_initialize();
	if(rc!=SQLITE_OK){fprintf(stderr, "Cannot initialize connection: %s\n", sqlite3_errmsg(db)); sqlite3_shutdown();exit(1);}
	rc = sqlite3_open("song.db", &db);
	if(rc){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}
	else{fprintf(stderr, "Opened database successfully\n");}
	sql = "SELECT name FROM sqlite_master WHERE type=\"table\" AND name <> \"sqlite_sequence\"";
	sqlExec(db,sql,output);
	if (output.empty())
	{
		cout<<"The database is empty"<<endl;
		return -1;
	}
	for (int i = 0; i < output.size();i++)
	{
		sql = "DELETE FROM " + output[i] + ";";
		sqlExec(db,sql,output);
	}
	cout<<"Database cleared successfully"<<endl;
	sqlite3_close(db);
	return 0;
}

void sqlExec(sqlite3 *db, string sql, vector<string>&output)
{
	int rc;
	sqlite3_stmt *query;
	rc = sqlite3_prepare_v2(db,sql.c_str(),-1, &query, NULL);
	if (rc!=SQLITE_OK){fprintf(stderr, "Cannot prepare: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	rc = sqlite3_step(query);
	if (rc!=SQLITE_ROW&&rc!=SQLITE_DONE){fprintf(stderr, "Cannot step: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	if (!sql.substr(0,sql.find_first_of(" ")).compare("SELECT"))
	{
		do	
		{
			if (!sqlite3_column_text(query,0))
				return;
			output.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(query,0))));
			rc = sqlite3_step(query);
		}while(rc == SQLITE_ROW);
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}
}
