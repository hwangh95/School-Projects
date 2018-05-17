#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

void tokenizer(ifstream &file, vector<vector<string>> &data);

int main(int argc, char* argv[])
{
	ifstream file;
	vector<vector<string>> data;//generic data vector
	file.open("artist.txt");
	if (!file){cerr<< "Cannot open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> artist : data){
		cout << "Artist ID: " << artist[0] << ", ";
		cout << "Artist Name: " << artist[1];
		cout << endl;
	}
	file.open("cd.txt");
	if (!file){cerr<< "Cannot open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> cd : data){
		cout << "CD ID: " << cd[0] << ", ";
		cout << "CD Title: " << cd[1] << ", ";
		cout << "Artist ID: " << cd[2] << ", ";
		cout << "Catalog: " << cd[3];
		cout << endl;
	}
	file.open("track.txt");
	if (!file){cerr<< "Cannot open file: " << strerror(errno)<<endl;file.close();exit(1);}
	tokenizer(file,data);
	for (vector<string> track : data){
		cout << "CD ID: " << track[0] << ", ";
		cout << "Track ID: " << track[1] << ", ";
		cout << "Song Title: " << track[2];
		cout << endl;
	}
	return 0;
}
void tokenizer(ifstream &file, vector<vector<string>> &data)
{
	data.clear();
	string str;
	while(getline(file,str,'\n')){
		vector<string> line;
		str.erase(0, str.find_first_not_of(" \f\n\r\t\v"));//trims whitespace
		str.erase(str.find_last_not_of(" \f\n\r\t\v") + 1);
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
