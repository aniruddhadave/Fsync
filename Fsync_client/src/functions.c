/***********************************************************************************************************************
*
*	This file contains the general functions used in the program FSOS.
*
***********************************************************************************************************************/

#include "header.h"

/************************************************************************************************************
*
*	FUNCTION NAME   :       remove_newline
*
*       DESCRIPTION     :       Removes \n from end of the string
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
void remove_newline(char *text)
{
        int len = strlen(text);			/* Length of string */

        if (text[len - 1] == '\n')
        {
                text[len - 1] = '\0';
        }
}

/***********************************************************************************************************
*       FUNCTION NAME   :       write_data
*
*       DESCRIPTION     :       Writes data into the socket descriptor. Appends a '$' at the end.
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
int write_data(int sd, char *data)
{
        int bytes_write = 0;			/* Bytes written */
        int len = strlen(data);			/* Length of string to write */
        int pointer = 0;			/* Location of next chunk of data that is to be written */

        /* Appending end-of-string character */
        data[len] = '$';
        len++;

        while (1)
        {
                bytes_write = write(sd, (data + pointer), (len - pointer));
                if (bytes_write < 0)
                {
                        return FAILURE;
                }
                if (bytes_write == 0)
                {
                        break;
                }
                pointer += bytes_write;
                bytes_write = 0;
        }

        return SUCCESS;
}

/***********************************************************************************************************
*
*	FUNCTION NAME   :       read_data
*
*       DESCRIPTION     :       Reads data from the socket descriptor.
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
int read_data(int sd, char *data)
{
        int bytes_read = 0;		/* Bytes read from socket */

        char *loc = NULL;
        char buf[BUFFSIZE];		/* Read Buffer */

        while (1)
        {
                memset (buf, 0, BUFFSIZE);
                bytes_read = read(sd, buf, (BUFFSIZE - 1));
                if (bytes_read < 0)
                {
                        return FAILURE;
                }
                if (buf[bytes_read - 1] == '$')
                {
                        strcat(data, buf);
                        break;
                }
                else
                {
                        strcat(data, buf);
                }
                bytes_read = 0;
        }

        /* Replacing '$' with \0 */
        loc = strstr(data, "$");
        *loc = '\0';


        return SUCCESS;
}

/***********************************************************************************************************
*
*	FUNCTION NAME   :       get_password
*
*       DESCRIPTION     :       Gets password from the user and hides it from the screen.
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
int get_password(char *data)
{
	int index = 0;
	char temp;

	while ('\n' != temp)
	{
		temp = getchar();
		data[index] = temp;
		printf("*");
		index++;
	}
	return SUCCESS;
}

/***********************************************************************************************************
*
*	FUNCTION NAME   :       read_file
*
*       DESCRIPTION     :       Reads the content of a file and stores it in a character array.
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
int read_file(FILE **fp, char *data)
{
	char temp[DATA_SMALL];			/* To store temporary data read from file */

	while (1)
	{
		if (0 != feof(*fp))
		{
			break;
		}
		memset(temp, 0, DATA_SMALL);
		fscanf(*fp, "%s", temp);
		strcat(data, temp);
		strcat(data, "\n");
	}
	return SUCCESS;
}
/***********************************************************************************************************
*
*	FUNCTION NAME   :       write_file
*
*       DESCRIPTION     :       Writes contents of a character array to a file.
*
*       RETURNS         :       SUCCESS or FAILURE
*
***********************************************************************************************************/
int write_file(FILE **fp, char *data)
{
	fputs(data, *fp);
	return SUCCESS;
}
