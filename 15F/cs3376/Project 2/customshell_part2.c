#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdbool.h>
char symbol[1024] = "$$";

char *getinput(char *, size_t);
void parse(char *, char **);
void printParsed(char **);

int main(int argc, char **argv) {
	char buf[1024];
	char **args = (char**)malloc(50*sizeof(char*));
	while (getinput(buf, sizeof(buf))){
		memset(args, 0, sizeof(args[0][0]) * 50 * sizeof(char*));	
		parse(buf, args);
		if (*args!=NULL){
			if(strcmp(*args, "exit") == 0||strcmp(*args,"quit") == 0)
				exit(0);
			else if (strcmp(*args, "prompt") == 0){
				char *tmp = *(++args);
				if (tmp!=NULL && tmp[0] == '"' && tmp[strlen(tmp)-1] == '"'){
					strncpy(symbol, (tmp+1),strlen(tmp)-2);
				}
				continue;
			}
			else if (strcmp(*args, "list") == 0){
				system("ls");
				continue;
			}
			else
				printParsed(args);
		}
	}
	exit(EX_OK);
}

char *getinput(char *buffer, size_t buflen)
{
	printf("%s ",symbol);
	return fgets(buffer, buflen, stdin);
}

void parse(char *buf, char **args)
{
	while (*buf != '\0'){
		while(*buf ==';' || *buf == '&' || *buf == '|' || *buf == '<' || *buf == '>' || *buf == ' '|| *buf == '\t' || *buf == '\n'){
			switch (*buf){
				case '&':*args++ = "&";break;
				case ';':*args++ = ";";break;
				case '|':*args++ = "|";break;
				case '<':*args++ = "<";break;
				case '>':
					if (*(buf+1) == '>'){
						*args++ = ">>";
						*buf++ = '\0';
					}
					else
						*args++ = ">";
					break;
			}
			*buf++ = '\0';
		}
		if (*buf!='\0')
			*args++ = buf;
		while(*buf !=';' && *buf != '|' && *buf != '<' && *buf != '&' && *buf != '>' && *buf != ' '&& *buf != '\t' && *buf != '\n' && *buf != '\0')
			buf++;		
	}
}
void printParsed(char **args)
{
	int arglen = 0;
	bool file = false;
	while(*args!=NULL){
		if (strcmp(*args,";")==0){
			printf("Semicolon\n");
			arglen=0;
		}
		else if (strcmp(*args,"|")==0){
			printf("Pipe\n");
			arglen=0;
		}
		else if (strcmp(*args,"&")==0){
			printf("Ampersand\n");
			arglen=0;
		}
		else if (strcmp(*args,"<") == 0||strcmp(*args,">") == 0||strcmp(*args,">>") == 0){
			printf("File redirection: %s\n",*args);
			file = true;
		}
		else if (**args == '-'){
			printf("Options: %s\n",*args);
			arglen = 1;
		}
		else{
			if (file){
				printf("File: %s\n",*args);
				file = false;
			}
			else{
				switch(arglen){
					case 0:
						printf("Command: %s\n",*args);
						arglen++;
						break;
					case 1:
						printf("Arguments: %s ",*args++);
						while (*args != NULL && strcmp(*args,";")!=0 && strcmp(*args,"|")!=0 && strcmp(*args,"&")!=0 && strcmp(*args,"<")!=0 && strcmp(*args,">")!=0 && strcmp(*args,">>")!=0 && **args != '-')
						{	
							printf("%s ", *args);
							args++;
						}
						printf("\n");
						args--;
						break;
				}
			}
		}
		args++;
	}
}
