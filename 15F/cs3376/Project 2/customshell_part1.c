#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
char symbol[1024] = "$$";

char *getinput(char *, size_t);
void parse(char *, char **);

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
		while(*buf == ' '|| *buf == '\t' || *buf == '\n')
			*buf++ = '\0';
		*args++ = buf;
		while(*buf != ' '&& *buf != '\t' && *buf != '\n' && *buf != '\0')
			buf++;		
	}
}
