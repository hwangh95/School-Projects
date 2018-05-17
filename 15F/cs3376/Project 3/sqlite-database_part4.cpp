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

void sqlExec(sqlite3 *db, string sql, string param, vector<string> &output);

int main(int argc, char* argv[])
{
	sqlite3 *db;
	string table,param;
	int rc;
	string sql;
	vector<string>output;
	
	rc = sqlite3_initialize();
	if(rc!=SQLITE_OK){fprintf(stderr, "Cannot initialize connection: %s\n", sqlite3_errmsg(db)); sqlite3_shutdown();exit(1);}
	rc = sqlite3_open("song.db", &db);
	if(rc){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}
	else{fprintf(stderr, "Opened database successfully\n");}
	sql = "SELECT name FROM sqlite_master WHERE type=\"table\" AND name <> \"sqlite_sequence\"";
	sqlExec(db,sql,"",output);
	if (output.empty())
	{
		cout<<"The database is empty"<<endl;
		return -1;
	}
	do
	{
		cout<<"The table names: "<<endl;
		for(int i=0; i<output.size(); ++i)
			cout << output[i] << '\n';
		cout<<"Enter the table name: ";
		getline(cin, table);
	}while(find(output.begin(), output.end(),table) == output.end());

	output.clear();	
	sql = "PRAGMA table_info(" + table + ");";
	sqlExec(db,sql,"",output);
	vector<string> record;
	string input;
	if (!table.compare("artist"))
	{
		cout<<"Insert id: ";
		getline(cin, input);
		record.push_back(input);
	
		cout<<"Enter the new name: ";
		getline(cin, input);
		record.push_back(input);
	
		sql = "UPDATE artist SET name=\""+ record[1]+ "\" WHERE id=" + record[0] + ";";
		sqlExec(db,sql,"",record);
	}

	else if (!table.compare("cd"))
	{
		cout<<"Insert id: ";
		getline(cin, input);
		record.push_back(input);
	
		do
		{
			for(int i=1; i<4; ++i)
				cout << output[i] << '\n';
			cout<<"Which field you like to edit?: ";
			getline(cin, param);
		}while(find(output.begin(), output.end(),param) == output.end());
	
		cout<<"Enter the new " + param + ": ";
		getline(cin, input);
		record.push_back(input);
	
		if (!param.compare("artist_id"))
			sql = "UPDATE cd SET " + param + "="+ record[1]+ " WHERE id=" + record[0] + ";";
		else
			sql = "UPDATE cd SET " + param + "=\""+ record[1]+ "\" WHERE id=" + record[0] + ";";
		sqlExec(db,sql,"",record);
	}

	else if (!table.compare("track"))
	{
		cout<<"Insert cd_id: ";
		getline(cin, input);
		record.push_back(input);
	
		cout<<"Insert track_id: ";
		getline(cin, input);
		record.push_back(input);

		cout<<"Enter the new title: ";
		getline(cin, input);
		record.push_back(input);
	
		sql = "UPDATE track SET title=\""+ record[2]+ "\" WHERE cd_id=" + record[0] + " AND track_id=" + record[1] + ";";
		sqlExec(db,sql,"",record);
	}
	cout<<table+":\n";
	sql = "SELECT * FROM "+ table + ";";
	sqlExec(db,sql,table,record);
	sqlite3_close(db);
	return 0;
}

void sqlExec(sqlite3 *db, string sql, string param, vector<string> &output)
{
	int rc;
	sqlite3_stmt *query;
	rc = sqlite3_prepare_v2(db,sql.c_str(),-1, &query, NULL);
	if (rc!=SQLITE_OK){fprintf(stderr, "Cannot prepare: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	rc = sqlite3_step(query);
	if (rc!=SQLITE_ROW&&rc!=SQLITE_DONE){fprintf(stderr, "Cannot step: %s\n", sqlite3_errmsg(db));sqlite3_close(db);exit(1);}

	if (!sql.substr(0,sql.find_first_of(" ")).compare("SELECT"))
	{
		if (!param.compare(""))
		{
			do	
			{
				if (!sqlite3_column_text(query,0))
					return;
				output.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(query,0))));
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("artist"))
		{
			cout<<left<<setw(10)<<"id"\
				<<"name"<<endl;
			cout<<"----      ------------------"<<endl;
			do
			{
				cout<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<sqlite3_column_text(query,1)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("cd"))
		{
			cout<<left<<setw(10)<<"id"\
				<<setw(40)<<"title"\
				<<setw(15)<<"artist_id"\
				<<"catalog"<<endl;
			cout<<"----      ------------------                      ---------      -----------"<<endl;
			do
			{
				cout<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<setw(40)<<sqlite3_column_text(query,1)\
					<<setw(15)<<sqlite3_column_text(query,2)\
					<<sqlite3_column_text(query,3)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
		else if (!param.compare("track"))
		{
			cout<<left<<setw(10)<<"cd_id"\
				<<setw(15)<<"track_id"\
				<<"title"<<endl;
			cout<<"------    ---------      ------------------"<<endl;
			do
			{
				cout<<left<<setw(10)<<sqlite3_column_text(query,0)\
					<<setw(15)<<sqlite3_column_text(query,1)\
					<<sqlite3_column_text(query,2)<<endl;
				rc = sqlite3_step(query);
			}while(rc == SQLITE_ROW);
		}
	}
	else if (!sql.substr(0,sql.find_first_of(" ")).compare("PRAGMA"))
	{
		do	
		{
			if (!sqlite3_column_text(query,1))
				return;
			output.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(query,1))));
			rc = sqlite3_step(query);
		}while(rc == SQLITE_ROW);
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_finalize(query);
	}
}
