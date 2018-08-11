
/********************************************************************************
	Manages database checks for authentication of client sends the files on
	server when client downloads updates the files when client syncs
********************************************************************************/

#include "header.h"

/*********************************************************************************
		FUNTION NAME 	 		: 	main()
		FUNCTION DESCRIPTION		: 	performs the server operation
		ARGUMENTS			: 	0
		RETURN TYPE			: 	int
*********************************************************************************/

int main()
{
	/***** VARIABLE DEFINATIONS *****/
	int	sd , connfd, ret = 0;
	struct	sockaddr_in
		server_address , client_address;
	socklen_t length ;
	char buf[DATA_SMALL] ;
	memset ( buf , 0 , DATA_SMALL ) ;


	/*****	STEP 1- CALLING SOCKET FUNCTION *****/
	sd = socket( AF_INET, SOCK_STREAM, 0 );/*socket created at server end*/
	if( sd < 0 ) {
		perror( "socket creation failed" );
		exit(0);
	}

	/***** STEP 2- SERVER'S OWN ADDRESS & PORT_NO IDENTIFICATION *****/
	memset( &server_address, 0, sizeof(server_address) );
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);		/*port number*/
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);	/*ip_address*/


	/***** STEP 3- BINDING THE SOCKET TO IP ADDRESS AND PORT_NO *****/
	ret = bind(  sd   ,  (struct sockaddr*)&server_address  ,  sizeof( server_address )  );
	if ( 0 > ret )
	{
		perror("Error in binding\n");
		exit(0);
	}

	/***** STEP 4- LISTENNING TO THE CLIENTS IN THE QUEUE *****/
	ret = listen( sd , QUEUE_MAX );
	if ( 0 != ret )
	{
		perror("Error in listennig to the clients\n");
		exit(0);
	}

	/***** THE MAIN SERVER LOOP BEGINNING *****/
	while(1)
	{
		printf("<----- Waiting for client----->\n");
		length = sizeof(client_address) ;

		/***** STEP 5- accepting from the client *****/
		connfd = accept( sd  ,  (struct sockaddr*)&client_address  ,  &length  ) ;
		if ( 0 > connfd)
		{
			perror("Error in accepting the user call\n");
			exit(0);
		}

		printf("<----- Connected to IP Address : %s----->\n",inet_ntop(  AF_INET  ,  &client_address.sin_addr  ,  buf  ,  sizeof(buf)  )  );


		while (1)
		{

			/*********************************************************************************************

						WORKING FUNCTIONALITIES IN ACCORDENCE TO USER REQUIREMENTS

			**********************************************************************************************/


			/*****	STEP 1- reading from the socket *****/
			char buffer_original[DATA_SMALL] ;
			char store[DATA_SMALL];
			char buffer_exit[DATA_SMALL];
			char buffer_for_tok[DATA_SMALL];

			char buff_reg[DATA_SMALL];
			char buff_signin[DATA_SMALL];
			char buff_view[DATA_SMALL];
			char buff_download[DATA_SMALL];
			char buff_delete[DATA_SMALL];
			char server_data[DATA_LARGE];


			memset(server_data, 0, DATA_LARGE);
			memset(buffer_original, 0, DATA_SMALL);
			memset(store , 0 , DATA_SMALL);
			memset ( buffer_exit , 0 , DATA_SMALL );
			memset(buffer_for_tok, 0, DATA_SMALL);
			memset(buff_reg, 0, DATA_SMALL);
			memset(buff_signin, 0, DATA_SMALL);
			memset(buff_view, 0, DATA_SMALL);
			memset(buff_download, 0, DATA_SMALL);
			memset(buff_delete, 0, DATA_SMALL);

			ret = read_data(  connfd ,  buffer_original ) ;


			if ( SUCCESS != ret )

			{
				perror("||SERVER|| : Unable to read from the socket \n") ;
			}

			strcpy(buffer_exit, buffer_original);

			char *tok = NULL;
			tok = strtok(buffer_exit," ");
			strcpy(store, tok);

			/*checking for word #exit#*/
			if(strcmp(store, "#exit#") == 0)
			{

			    close(connfd);
			    break;
			}
			strcpy(buffer_for_tok, buffer_original);


			ret = func_tokenise( buffer_for_tok );

			printf("ret %d\n",ret);

			switch ( ret )
			{
				case REGISTRATION	:
							printf("in reg\n");
				    		{	strcpy(buff_reg,buffer_original);

							ret = register_user(buff_reg) ;
							if(SUCCESS != ret)
							{
							    strcpy(server_data,"register_nack");
							    write_data(connfd, server_data);
							    perror("SERVER: unable to register\n");
							}
							else
							{
							    strcpy(server_data,"register_ack");


							    write_data(connfd,server_data);
							}
							break ;
						}

				case SIGNIN 		:
							printf("in signin\n");
						{	strcpy(buff_signin,buffer_original);

							ret = sign_in_authentication(buff_signin) ;
						        if(SUCCESS != ret)
							{
							    strcpy(server_data, "signin_nack");

							    write_data(connfd, server_data);
							    perror("SERVER: unable to signin");
							}
					    		else
					    		{
							    strcpy(server_data, "signin_ack");
							    write_data(connfd, server_data);
							}
							break ;
						}

				case SYNC 		:
						{       printf("entering sync functions\n");
							ret = syncserver(connfd) ;
							if (SUCCESS == ret)
							{

							    strcpy(server_data, "sync_ack");
							    write_data(connfd, server_data);
							}

							break ;
						}

				case VIEW 		:
						{	printf("in view\n");
						        strcpy(buff_view, buffer_original);
							ret = view(connfd, buff_view) ;

							if(SUCCESS != ret)
							{
							    strcpy(server_data, "view_nack");
							    write_data(connfd, server_data);
							    perror("SERVER: unable to display\n");
							}
							break ;
						}

				case DOWNLOAD		:
						{	printf("in download\n");
						    	strcpy(buff_download, buffer_original);

							ret = download(connfd, buff_download) ;

							if(SUCCESS != ret)
							{
							    strcpy(server_data, "download_nack");
							    write_data(connfd, server_data);
							    perror("SERVER: unable to run download\n");
							}
							break ;
						}

				case DELETE 		:
						{	printf("in delete\n");
						    	strcpy(buff_delete,buffer_original);
							ret = delete_file(connfd, buff_delete) ;

							if(ret != FAILURE)
							{
							    strcpy(server_data, "delete_nack");
							    ret = write_data(connfd, server_data);
							    perror("SERVER: unable to delete the file\n");
							}
							else
							{
							   strcpy(server_data, "delete_ack") ;
							   ret =  write_data(connfd, server_data);
							}

							break ;
						}

			}

			/********************************************* END ***********************************************/
		}
	}

exit(0);
}
