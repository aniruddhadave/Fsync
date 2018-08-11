
/***************************************************************************************************************************
			Invoke the Executable as server
*****************************************************************************************************************************/

#include "header.h"

/******************************************************************************************************************************
		FUNTION NAME 	 		: 	func_tokenise()
		FUNCTION DESCRIPTION		: 	tokenises the command word coming from client and returns the respective macro
							for doing the further operations
		ARGUMENTS			: 	one - char array type
		RETURN TYPE			: 	int
******************************************************************************************************************************/

int func_tokenise( char buffer[] )
{
	char store_buffer[DATA_SMALL] ;
	char *token = NULL;
	token = strtok( buffer , " " );
	strcpy( store_buffer, token );

	if ( strcmp(store_buffer , "#register#" ) == 0 )
	{
		return (REGISTRATION) ;
	}

	else if ( strcmp(store_buffer , "#signin#" ) == 0 )
	{
		return (SIGNIN) ;
	}

	else if ( strcmp(store_buffer , "#sync#" ) == 0 )
	{
		return (SYNC) ;
	}

	else if ( strcmp(store_buffer , "#view#" ) == 0 )
	{
		return (VIEW) ;
	}

	else if ( strcmp(store_buffer , "#download#" ) == 0 )
	{
		return (DOWNLOAD) ;
	}

	else if ( strcmp(store_buffer , "#delete#" ) == 0 )
	{
		return (DELETE) ;
	}

}

/********************************************************************************************************
		FUNTION NAME 	 		: 	view()
		FUNCTION DESCRIPTION		: 	To display the contents of the user folder
		ARGUMENTS			: 	one - char array type
		RETURN TYPE			: 	int
*********************************************************************************************************/

int view(int connfd, char buffer[])
{
    printf("inside view function");

    FILE *fp = NULL ;
    char store_buff[DATA_MEDIUM] ;
    char file_buff[DATA_MEDIUM] ;
    char command[DATA_MEDIUM] ;
    char file_names[DATA_MEDIUM] ;
    char file_path[DATA_MEDIUM];

    int ret;
    memset( file_names  ,  0  ,  DATA_MEDIUM ) ;
    memset( command ,  0 ,  DATA_MEDIUM ) ;
    memset( file_buff  ,  0 ,  DATA_MEDIUM ) ;
    memset( store_buff  , 0 ,  DATA_MEDIUM ) ;
    memset( file_path, 0 , DATA_MEDIUM);

    char *token = NULL;
    token = strtok( buffer , " ") ;
    token = strtok( NULL , " " ) ;

    strcpy( store_buff , token) ;		/* taken username in store_buff */

    strcpy( command , "ls ./Dropbox/" ) ;
    strcat( command , store_buff );
    strcat( command , " > " ) ;
    strcat(command, "./Dropbox/");
    strcat( command , store_buff );


    strcat( command , "/aaaaaa" );

    system( command );

    strcpy(file_path,"./Dropbox/");
    strcat(file_path , store_buff );
    strcat(file_path , "/aaaaaa");
    fp = fopen(file_path,"r");

    if(NULL == fp)
    {
	perror("unable to open file\n");
	exit(0);
    }


     ret = read_file(  &fp  ,  file_names  );

     ret = write_data(  connfd  ,  file_names+7  );


    fclose(fp);

return SUCCESS;
}

/*****************************************************************************************************
		FUNTION NAME 	 		: 	download
		FUNCTION DESCRIPTION		: 	To send the user the file it wants to send over socket
		ARGUMENTS			: 	connection descriptor and character buffer
		RETURN TYPE			: 	int
******************************************************************************************************/
int download(int connfd, char buffer[])
{
    	printf("inside download()\n");

    	FILE *fp = NULL;
	char *token = NULL;
	char username[DATA_MEDIUM];
	char filename[DATA_MEDIUM];
	char command[DATA_MEDIUM];
	char copy[DATA_MEDIUM];
	char file_content[DATA_LARGE]; 	/* modified by amlan */
	int ret;

	memset( file_content , 0 , DATA_LARGE ) ;
	memset( copy , 0 , DATA_MEDIUM ) ;
	memset( command , 0 , DATA_MEDIUM ) ;
	memset( username , 0 , DATA_MEDIUM ) ;
	memset( filename , 0 , DATA_MEDIUM ) ;


	token = strtok( buffer , " " );
	token = strtok( NULL, " ");
	strcpy( username , token );

	token = strtok( NULL , " " );
	strcpy( filename , token );

	strcpy(command, "./Dropbox/");
	strcat(command,username);
	strcat(command,"/");
	strcat(command,filename);

	fp = fopen( command , "r" ) ;
	if( NULL == fp )
	{
	    perror("unable to open file\n");
	    return FAILURE;
	}

	 ret = read_file( &fp , file_content );

	 ret = write_data( connfd , file_content );


	fclose( fp );
	return( SUCCESS );
}

/******************************************************************************************************************
		FUNTION NAME 	 		: 	delete_file
		FUNCTION DESCRIPTION		: 	To delete the particular file whose name is provided by the user
		ARGUMENTS			: 	connection descriptor and character buffer
		RETURN TYPE			: 	int
*******************************************************************************************************************/


int delete_file(int connfd, char buffer[])
{
    	printf("in delete file()\n");
    	char *token = NULL;
	char username[DATA_MEDIUM];
	char filename[DATA_MEDIUM];
	char command[DATA_MEDIUM];

	memset (username , 0 , DATA_MEDIUM );
	memset (filename , 0 , DATA_MEDIUM );
	memset (command , 0 , DATA_MEDIUM );

    	token = strtok( buffer , " " );
	token = strtok( NULL , " ");
	strcpy( username , token );
	token = strtok ( NULL , " " );
	strcpy( filename , token );

	strcpy ( command , "rm ");
	strcat ( command , "./Dropbox/");
	strcat ( command , username) ;
	strcat ( command , "/") ;
	strcat ( command , filename) ;

	int ret = system ( command ) ;
	if(ret == 0)
	{
		return FAILURE ;
	}
	else
	{
	    return SUCCESS;
	}

}
    /******************************************************** END OF FILE ******************************************************************************/
