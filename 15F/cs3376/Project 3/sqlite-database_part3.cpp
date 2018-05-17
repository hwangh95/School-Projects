#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

void tokenizer(ifstream &file, vector<vector<string>> &data);
void sqlExec(sqlite3 *db, string sql, string param);

int main(int argc, char* argv[])
{
	sqlite3 *db;
	string param;
	int rc;
	string sql;
	ofstream output;
	output.open("output.txt");
	output.close();
	vector<vector<string>> data;//generic data vector
	
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
	sqlite3_close(db);
	return 0;
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
		ofstream output;
		output.open("output.txt",ios::app);
		if(!output){cerr<< "Can't write file: " << strerror(errno)<<endl;output.close();exit(1);}
		output<<param+":"<<endl;
		
		if (!param.compare("artist"))
		{
			output<<left<<setw(10)<<"id"\
				<<"name"<<endl;
			output<<"----      ------------------"<<endl;
			do
			{
				output<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<sqlite3_column_text(query,1)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("cd"))
		{
			output<<left<<setw(10)<<"id"\
				<<setw(40)<<"title"\
				<<setw(15)<<"artist_id"\
				<<"catalog"<<endl;
			output<<"----      ------------------                      ---------      -----------"<<endl;
			do
			{
				output<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<setw(40)<<sqlite3_column_text(query,1)\
					<<setw(15)<<sqlite3_column_text(query,2)\
					<<sqlite3_column_text(query,3)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("track"))
		{
			output<<left<<setw(10)<<"cd_id"\
				<<setw(15)<<"track_id"\
				<<"title"<<endl;
			output<<"------    ---------      ------------------"<<endl;
			do
			{
				output<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<setw(15)<<sqlite3_column_text(query,1)\
					<<sqlite3_column_text(query,2)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}
}
