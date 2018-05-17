#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
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
void execute(char **);
void commandParse(char **,char ***); //turns ** into ***, the array of commands separated by pipes or other symbols
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
				execute(args);
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
void execute(char **args)
{
	pid_t pid; 
	int status;
	int *pipefd;
	char *** commands =(char***)malloc(50*sizeof(char**));

	pipe(pipefd);
	commandParse(args,commands);
	if((pid=fork()) == -1) {
		fprintf(stderr, "shell: can't fork: %s\n",strerror(errno));
		exit(EX_OSERR);
	}
	else if (pid == 0) {//child gets input from pipe
		if (fork() == 0){
			dup2(pipefd[0],0);
			close(pipefd[1]);
			execvp(**(commands+2),*(commands+2));
			fprintf(stderr, "shell: couldn't exec %s: %s\n",*args,strerror(errno));
			exit(EX_DATAERR);
		}
		else//parent gives output to pipe
		{
			dup2(pipefd[1], 1);
			close(pipefd[0]);
			execvp(**commands,*commands);
		}
	}
	if ((pid=waitpid(pid, &status, 0)) < 0)
		fprintf(stderr, "shell: waitpid error: %s\n",strerror(errno));
}
void commandParse(char ** args,char *** commands)
{	
	while (*args != NULL){
		while(*args[0] ==';' || *args[0] == '&' || *args[0] == '|' || *args[0] == '<' || *args[0] == '>'){
			char** tmp=malloc(sizeof(char*));
			tmp[0] = *args;
			*commands++ = tmp;
			
/*			switch (*args[0]){
				case '&':*commands++ = {"&"};break;
				case ';':*commands++ = {";"};break;
				case '|':*commands++ = {"|"};break;
				case '<':*commands++ = {"<"};break;
				case '>':
					if (strcmp(*args,">>") == 0){
						*commands++ = {">>"};
					}
					else
						*commands++ = {">"};
					break;
			}*/
			*args++ = NULL;
		}
		if (*args!=NULL)
			*commands++ = args;
		while(*args != NULL&&*args[0] !=';' && *args[0] != '&' && *args[0] != '|' && *args[0] != '<' && *args[0] != '>')
			args++;
	}
}
