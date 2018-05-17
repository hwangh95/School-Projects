#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sqlite3.h> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <time.h>
using namespace std;


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 45991 /* server port – you need to change this */
#define LISTENQ 8 /*maximum number of client connections*/

int main (int argc, char **argv)
{
	int listenfd, connfd, n;
	pid_t childpid;
	socklen_t clilen;
	char buf[MAXLINE];
	struct sockaddr_in cliaddr, servaddr;
	sqlite3 *db;
	string param;
	int rc;
	
	ofstream file;
	file.open("log.txt",ofstream::app);
	if(!file){cerr<< "Can't write file: " << strerror(errno)<<endl;file.close();exit(1);}
	rc = sqlite3_initialize();
	if(rc!=SQLITE_OK){fprintf(stderr, "Cannot initialize connection: %s\n", sqlite3_errmsg(db)); file<<time(0)<<endl;file<<sqlite3_errmsg(db)<<endl;sqlite3_shutdown();exit(1);}
	rc = sqlite3_open("music.db", &db);
	if(rc){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));file<<time(0)<<endl;file<<sqlite3_errmsg(db)<<endl;sqlite3_close(db);exit(1);}
	else{fprintf(stderr, "Opened database successfully\n");}

	//Create a socket for the soclet
	//If sockfd<0 there was an error in the creation of the socket
	if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		file<<time(0)<<endl;
		file<<"Problem in creating the socket"<<endl;
		perror("Problem in creating the socket");
		file.close();
		sqlite3_close(db);
		exit(2);
	}

	//preparation of the socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	//bind the socket
	bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//listen to the socket by creating a connection queue, then wait for clients
	listen (listenfd, LISTENQ);
	printf("%s\n","Server running...waiting for connections.");

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		//accept a connection
		connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("%s\n","Received request...");
			if ( (childpid = fork ()) == 0 ) {       //if it’s 0, it’s child process
			printf ("%s\n","Child created for dealing with client requests");
			//close listening socket
			close (listenfd);

			while ((n = recv(connfd, buf, MAXLINE,0)) > 0)  {
				buf[n] = '\0';
				string sql = buf;
				if (!sql.compare("EXIT\n"))
				{
					file<<time(0)<<endl;
					file<<"Exiting"<<endl;
					cout<<"Exiting"<<endl;
					sqlite3_close(db);
					close(connfd);
					exit(0);
				}
				if (sql.find("artist")!=string::npos)
					param = "artist"; 
				else if (sql.find("cd")!=string::npos)
					param = "cd"; 
				else if (sql.find("track")!=string::npos)
					param = "track"; 
				sqlite3_stmt *query;
				rc = sqlite3_prepare_v2(db,sql.c_str(),-1, &query, NULL);
				if (rc!=SQLITE_OK){fprintf(stderr, "Cannot prepare: %s\n", sqlite3_errmsg(db));sqlite3_close(db);file<<time(0)<<endl;file<<sqlite3_errmsg(db)<<endl;send(connfd, sqlite3_errmsg(db),MAXLINE,0);exit(1);}

				rc = sqlite3_step(query);
				if (rc!=SQLITE_ROW&&rc!=SQLITE_DONE){fprintf(stderr, "Cannot step: %s\n", sqlite3_errmsg(db));sqlite3_close(db);file<<time(0)<<endl;file<<sqlite3_errmsg(db)<<endl;send(connfd, sqlite3_errmsg(db),MAXLINE,0);exit(1);}

				stringstream output;
				output.str("");
				string tmp;
				file<<time(0)<<endl;
				cout<<sql;
				output<<sql;
				if (!sql.substr(0,sql.find_first_of(" ")).compare("SELECT"))
				{
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
				tmp = output.str();
				send(connfd, tmp.c_str(), MAXLINE, 0);
				output.str("");
				file<<tmp<<endl;
			}
			if (n < 0){
				file<<time(0)<<endl;
				file<<"Read error"<<endl;
				printf("%s\n", "Read error");
			}
			file.close();
			exit(0);
		} 
		close(connfd);
	}
	sqlite3_close(db);
}
