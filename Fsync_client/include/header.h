/***********************************************************************************************************************
*
*	FILENAME	:	header.h
*
*	DESCRIPTION	:	This file contains header files, macros and function prototypes used in the program.
*
*	REVISION HISTORY:
*
*	DATE			NAME			REFERENCE			REASON			
*	22/04/2014		Saket Sharad						Initial Creation
*
*	Copyright @ Aricent 2014
*
***********************************************************************************************************************/

/*********************************************** HEADER FILES *********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<dirent.h>
#include<time.h>
#include <fcntl.h>
#include <sys/stat.h>

/***************************************************** MACROS *********************************************************/
#define SUCCESS 0
#define FAILURE 1
#define DATA_SMALL 50
#define DATA_MEDIUM 500
#define DATA_LARGE 10000
#define BUFFSIZE 10
#define SERVER_IP "10.203.161.16"
#define SERVER_PORT 36669
#define EMPTY 2

#define EXIT 100
#define INVALID 999
 
#define REGISTRATION 101
#define SIGNIN 102
#define SYNC 201
#define VIEW 202
#define DOWNLOAD 301
#define DELETE 302

/*********************************************** FUNCTION PROTOTYPES *******************************************************/
int write_data(int, char *);
int read_data(int, char *);
int write_file(FILE **, char *);
int read_file(FILE **, char *);
int menu_flash(int, char *);
int menu_signin(int, char *);
int menu_view(int, char *);
void remove_newline(char *);

int syncclient(int sd, char *username);
int getfileparameter(char *,char *,char *);
int addfilestat(char *, char *,char *);
int concatfile(char **S, int argc, char *argv[], char *path) ;
int getfiles(char *, char **, char *path);
int getclientdirectoryfiles(int sd, char* path, char *filestring);
