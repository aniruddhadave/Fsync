/***********************************************************************************************************************
* Functions used for menu interface.
***********************************************************************************************************************/

#include "header.h"

/**********************************************************************************************************************
*
*	FUNCTION NAME	:	menu_flash
*
*	DESCRIPTION	:	Implements interface for the flash screen.
*
*	RETURNS		:	Operation performed- EXIT, REGISTRATION or SIGNIN
*
***********************************************************************************************************************/
int menu_flash(int sd, char *username)
{
	int choice = 0;
	int retval = 0;

	char client_data[DATA_MEDIUM];
	char password[DATA_SMALL];
	char new_data[DATA_LARGE];

	printf("\n--------------------\n0 -- Exit\n1 -- Register\n2 -- Sign In\n--------------------\nChoice: ");
	scanf("%d", &choice);

	/* Exit */
	if (0 == choice)
	{
		memset(client_data, 0, DATA_MEDIUM);
		strcpy(client_data, "#exit#");
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}

		return EXIT;
	}
	/* Registration */
	else if (1 == choice)
	{
		/* Get username */
		printf("\n----------------------------\n");
		printf("** New User Registration **\n");
		printf("----------------------------\n");
		printf("Enter username: ");
		scanf("%s", username);

		/* Get password */
		while ('\n' != getchar());
		printf("Choose password: ");
		scanf("%s", password);


		/* Request to server for registration */
		memset(client_data, 0, DATA_MEDIUM);
		strcpy(client_data, "#register#");
		strcat(client_data, " ");
		strcat(client_data, username);
		strcat(client_data, " ");
		strcat(client_data, password);
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}
		else
		{
			memset(new_data, 0, DATA_LARGE);
			retval = read_data(sd, new_data);
			if (FAILURE == retval)
			{
				printf("Read error!\n");
				return FAILURE;
			}

			if (0 == strcmp(new_data, "register_ack"))
			{
				memset(client_data, 0, DATA_MEDIUM);
				strcpy(client_data, "#exit#");
				retval = write_data(sd, client_data);
				if (FAILURE == retval)
				{
					printf("Write error!\n");
					return FAILURE;
				}
				return REGISTRATION;
			}
			else
			{
				printf("> Username not available! Try a different username...\n\n");
			}
		}
	}
	/* Sign In */
	else if (2 == choice)
	{
		/* Get username */
		printf("\n----------------------------\n");
		printf("** Existing user Sign In **\n");
		printf("----------------------------\n");
		printf("Username: ");
		scanf("%s", username);
		/* Get password */
		while ('\n' != getchar());
		printf("Password: ");
		scanf("%s", password);

		/* Request to server for signin */
		memset(client_data, 0, DATA_MEDIUM);
		strcpy(client_data, "#signin#");
		strcat(client_data, " ");
		strcat(client_data, username);
		strcat(client_data, " ");
		strcat(client_data, password);
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}
		else
		{
			memset(new_data, 0, DATA_LARGE);
			retval = read_data(sd, new_data);
			if (FAILURE == retval)
			{
				printf("Read error!\n");
				return FAILURE;
			}

			if (0 == strcmp(new_data, "signin_ack"))
			{
				printf("> User Authenticated!\n");
				return SIGNIN;
			}
			else
			{
				printf("> WRONG USERNAME or PASSWORD!!\n\n");
			}
		}
	}
	else
	{
		printf("Invalid choice!\n");
	}
	return SUCCESS;
}

/**********************************************************************************************************************
*
*	FUNCTION NAME	:	menu_signin
*
*	DESCRIPTION	:	Implements interface for the screen that comes on 'Signin'
*
*	RETURNS		:	Operation performed- EXIT, VIEW.
*
***********************************************************************************************************************/
int menu_signin(int sd, char *username)
{
	int choice = 0;
	int retval = 0;

	char client_data[DATA_LARGE];
	char new_data[DATA_LARGE];

	while (1)
	{
		/* Clearing the screen */
	//	system("clear");

		printf("-----------------------------------\n");
		printf("FILE SYNCHRONIZATION OVER SERVER\n");
		printf("-----------------------------------\n");
		printf("\nCurrent User: %s\n", username);
		printf("--------------------\n0 -- Exit\n1 -- Sync\n2 -- View\n--------------------\nChoice: ");
		scanf("%d", &choice);

		/* Exit */
		if (0 == choice)
		{
			memset(client_data, 0, DATA_MEDIUM);
			strcpy(client_data, "#exit#");
			printf("Sending: %s\n", client_data);
			retval = write_data(sd, client_data);
			if (FAILURE == retval)
			{
				printf("Write error!\n");
				return FAILURE;
			}

			return EXIT;
		}
		/* Synchronization */
		else if (1 == choice)
		{
			memset(client_data, 0, DATA_LARGE);
			memset(new_data, 0, DATA_LARGE);
			strcpy(client_data, "#sync#");
			strcat(client_data, " ");
			strcat(client_data, username);
			write_data(sd, client_data);

			retval = syncclient(sd, username);

			printf("Sending: %s\n", client_data);
			if (FAILURE == retval)
			{
				printf("Write error!\n");
				return FAILURE;
			}
			else
			{
				memset(new_data, 0, DATA_LARGE);
				retval = read_data(sd, new_data);
				if (FAILURE == retval)
				{
					printf("Read error!\n");
					return FAILURE;
				}

				if (0 == strcmp(new_data, "sync_ack"))
				{
					printf("> Synchronization done!\n");
					return SYNC;
				}
				else
				{
					printf("> Sync failed!\n");
				}
			}

		}
		/* View */
		else if (2 == choice)
		{
			memset(client_data, 0, DATA_LARGE);
			memset(new_data, 0, DATA_LARGE);
			strcpy(client_data, "#view#");
			strcat(client_data, " ");
			strcat(client_data, username);
			write_data(sd, client_data);
			printf("Sending: %s\n", client_data);
			if (FAILURE == retval)
			{
				printf("Write error!\n");
				return FAILURE;
			}
			else
			{
				memset(new_data, 0, DATA_LARGE);
				system("clear");
				printf("---------------------\n");
				printf("Files in Dropbox\n");
				printf("---------------------\n");

				retval = read_data(sd, new_data);
				if (FAILURE == retval)
				{
					printf("Read error!\n");
					return FAILURE;
				}
				/* Display file list */
				printf("%s\n", new_data);
				return VIEW;
			}
		}
		else
		{
			printf("Invalid choice!\n");
		}
	} /* End of while loop */

}

/**********************************************************************************************************************
*
*	FUNCTION NAME	:	menu_view
*
*	DESCRIPTION	:	Implements interface for the screen which comes after 'View' selection.
*
*	RETURNS		:	Operation performed- EXIT, FAILURE,
*
***********************************************************************************************************************/
int menu_view(int sd, char *username)
{
	int choice = 0;
	int retval = 0;

	char client_data[DATA_LARGE];
	char new_data[DATA_LARGE];
	char file_path[DATA_MEDIUM];
	char filename[DATA_SMALL];

	FILE *fp = NULL;

	/* Clear screen */
//	system("clear");

	printf("-----------------------------------\n");
	printf("FILE SYNCHRONIZATION OVER SERVER\n");
	printf("-----------------------------------\n");
	printf("\nCurrent User: %s\n", username);
	printf("--------------------\n0 -- Exit\n1 -- View\n2 -- Download\n3 -- Delete\n--------------------\nChoice: ");
	scanf("%d", &choice);

	/* Exit */
	if (0 == choice)
	{
		memset(client_data, 0, DATA_MEDIUM);
		strcpy(client_data, "#exit#");
		printf("Sending: %s\n", client_data);
		retval = write_data(sd, client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}

		return EXIT;
	}
	/* View */
	else if (1 == choice)
	{
		memset(client_data, 0, DATA_LARGE);
		memset(new_data, 0, DATA_LARGE);
		strcpy(client_data, "#view#");
		strcat(client_data, " ");
		strcat(client_data, username);
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}
		else
		{
			memset(new_data, 0, DATA_LARGE);
			system("clear");
			printf("---------------------\n");
			printf("Files in Dropbox\n");
			printf("---------------------\n");

			retval = read_data(sd, new_data);
			if (FAILURE == retval)
			{
				printf("Read error!\n");
				return FAILURE;
			}
			/* File list gets displayed */
			printf("%s\n", new_data);
		}
	}
	/* Download */
	else if (2 == choice)
	{
		while ('\n' != getchar());
		printf("\n----------------------------\n");
		printf("** File Download **\n");
		printf("----------------------------\n");
		printf("Enter file to download: ");
		memset(filename, 0, DATA_SMALL);
		fgets(filename, DATA_SMALL, stdin);
		remove_newline(filename);
		memset(client_data, 0, DATA_LARGE);
		strcpy(client_data, "#download#");
		strcat(client_data, " ");
		strcat(client_data, username);
		strcat(client_data, " ");
		strcat(client_data, filename);
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}
		else
		{
			memset(new_data, 0, DATA_LARGE);
			retval = read_data(sd, new_data);
			if (FAILURE == retval)
			{
				printf("Read error!\n");
				return FAILURE;
			}

			if (0 == strncmp(new_data, "download_nack", 13))
			{
				printf("> File not found!\n");
			}
			else
			{
				/* Save file */
				memset(file_path, 0, DATA_MEDIUM);
				strcpy(file_path, "./Dropbox/");
				strcat(file_path, filename);
				fp = fopen(file_path, "w");
				if (NULL == fp)
				{
					printf("Error creating download file!\n");
					return FAILURE;
				}
				retval = write_file(&fp, new_data);
				if (FAILURE == retval)
				{
					printf("Error writing to file!\n");
					return FAILURE;
				}
				printf("> File downloaded!\n");
			}
		}
	}
	/* Delete */
	else if (3 == choice)
	{
		while ('\n' != getchar());
		printf("\n----------------------------\n");
		printf("** File Delete **\n");
		printf("----------------------------\n");
		printf("Enter file to delete: ");
		memset(filename, 0, DATA_SMALL);
		memset(client_data, 0, DATA_SMALL);
		fgets(filename, DATA_SMALL, stdin);
		remove_newline(filename);
		strcpy(client_data, "#delete#");
		strcat(client_data, " ");
		strcat(client_data, username);
		strcat(client_data, " ");
		strcat(client_data, filename);
		retval = write_data(sd, client_data);
		printf("Sending: %s\n", client_data);
		if (FAILURE == retval)
		{
			printf("Write error!\n");
			return FAILURE;
		}
		else
		{
			memset(new_data, 0, DATA_LARGE);
			retval = read_data(sd, new_data);
			if (FAILURE == retval)
			{
				printf("Read error!\n");
				return FAILURE;
			}

			if (0 == strncmp(new_data, "delete_ack", 10))
			{
				printf("> File deleted!\n");
			}
			else
			{
				printf("> File not found!\n");
			}
		}
	}
	else
	{
		printf("Invalid choice!\n");
	}

	return SUCCESS;
}
