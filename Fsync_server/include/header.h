/******************************************************************************

			HEADER INCLUDIONS

*******************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<dirent.h>
#include<time.h>


/********************************************************************************

			MACRO DEFINATIONS

*********************************************************************************/

#define SUCCESS 0
#define FAILURE 1
#define DATA_SMALL 50
#define DATA_MEDIUM 500
#define DATA_LARGE 10000
#define BUFFSIZE 10
#define SERVER_IP 10.203.161.8
#define SERVER_PORT 36669
#define EMPTY 2
 
#define INVALID 999
 
#define REGISTRATION 101
#define SIGNIN 102
#define SYNC 201
#define VIEW 202
#define DOWNLOAD 301
#define DELETE 302
#define QUEUE_MAX 5
#define MODULOUS 10

/**********************************************************************************

  			STRUCTURE DECLARATION

**********************************************************************************/
typedef struct user
{
        char *user_name ;

	    char *user_password ;

	        struct user *next;

}user;

typedef struct lnode
{
	char fname[DATA_MEDIUM];
	long int mtime;
	long int fsize;
	struct lnode *next;
}node;


extern user *h_table[10] ;


/**********************************************************************************

			FUNCTIONS DECLARATIONS

***********************************************************************************/
/*reading and writing from socket*/
int write_data(int, char *);
int read_data(int, char *);

/*tokenising the buffer*/
int func_tokenise( char buffer[]);

/*viewing, downloading and deleting file*/
int view(int, char buffer[]);
int download(int, char buffer[]);
int delete_file(int, char buffer[]);

/*reading and writing from file*/
int write_file(FILE **, char *);
int read_file(FILE **, char *);

/*authentication and signin part*/
int register_user( char* ) ;
int check_availability( char * ) ;
extern int func_hash( char [] ) ;
int create_user_directory( char * ) ;
int sign_in_authentication( char * ) ;

/* sync */
int syncserver(int connfd);
void s_handler(int sig);
int getclientdirectory(char *string, char *dir);
int getclientpath(char *dir, char *path);
int createclientlist(char *, node **);
int createserverlist(char *, node **, node*, char *);
int addfilenode(node **head, long int , long int, char *);
int free_list(node *head);
void dislayfilestat(node *head);
int getserverfileparameter(char *, char *, char *);
int addserverfilestat(char *, char *, char *);
int getallfiles(node *headclient,char *);
int comparelist(node *head, node *head1, char *, char *);
int searchfile(node *head, char *name, node **ret);
int removefile(char *rmfiles, char *path);
int updatefiles(char *filecontent, char *path);
int write_filesync(int fd, char *);










