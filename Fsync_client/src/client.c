/***********************************************************************************************************************
* This file contains the main function for client.
***********************************************************************************************************************/

#include "header.h"

/**********************************************************************************************************************
*
*	FUNCTION NAME	:	main
*
*	DESCRIPTION	:	Performs client operation
*
*	RETURNS		:	SUCCESS or FAILURE
*
***********************************************************************************************************************/
int main()
{
	int c_sd = 0;				/* Client Socket descriptor */
	int retval_flash = 0;			/* Return for flash screen menu */
	int retval_signin = 0;			/* Return for sign in menu */
	int retval_view = 0;			/* Return for view menu */
	int retval = 0;				/* Return value for functions */

	char username[DATA_SMALL];		/* Stores username */
	char server_ip[DATA_MEDIUM];		/* Stores server IP address */

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(server_ip, 0, DATA_MEDIUM);

	/* Creating Dropbox for user */
	retval = system("mkdir Dropbox");
	if (0 != retval)
	{
		printf("Dropbox available\n");
	}
	else
	{
		printf("New Dropbox created!\n");
	}

	/*Creating a socket for client */
	c_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sd < 0)
	{
		printf("Client socket creation error!\n");
		exit (FAILURE);
	}

	/* Storing server IP address */
	strcpy(server_ip, SERVER_IP);

	/*Inititalizing server address */
	server_address.sin_family = AF_INET;				/* IPv4 family */
	server_address.sin_port = htons(SERVER_PORT);			/* Port number */
	server_address.sin_addr.s_addr = inet_addr(server_ip);		/* IP Addr of server to connect */

	/* Connecting to server */
	retval = connect(c_sd, (struct sockaddr *)&server_address, sizeof(server_address));
	if (retval < 0)
	{
		printf("Connection to server failed!!\n");
		exit (FAILURE);
	}
	printf(">> Connected to %s <<\n", server_ip);

	/* Main working of Client */
	while (1)
	{
		system("clear");
		printf("-----------------------------------\n");
		printf("FILE SYNCHRONIZATION OVER SERVER\n");
		printf("-----------------------------------\n");
		memset(username, 0, DATA_SMALL);

		/* Flash screen */
		retval_flash = menu_flash(c_sd, username);
		if (EXIT  == retval_flash)
		{
			printf("Exiting program!\n");
			exit (SUCCESS);
		}
		else if (REGISTRATION == retval_flash)
		{
			printf("> User '%s' registered. Run the program again and Sign In.\n", username);
			exit (SUCCESS);
		}
		else if (SIGNIN == retval_flash)
		{
			/* Sign In menu */
			retval_signin = menu_signin(c_sd, username);
			if (EXIT  == retval_signin)
			{
				printf("Exiting program!\n");
				exit (SUCCESS);
			}
			else if (SYNC == retval_signin)
			{
				exit (SUCCESS);
			}
			else if (VIEW == retval_signin)
			{
				while (1)
				{
					/* View menu */
					retval_view = menu_view(c_sd, username);
					if (EXIT  == retval_view)
					{
						printf("Exiting program!\n");
						exit (SUCCESS);
					}
				} /* End of while loop */
			}

		}
	} /* End of outer while loop */












}
