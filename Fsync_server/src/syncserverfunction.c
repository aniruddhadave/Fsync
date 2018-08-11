#include"header.h"



/* READ DATA FROM SOCKET */
/*
int read_data(int fd, char *str)
{
        int bytes_read = 0;
        char buf[5];
        int len = 0;
        memset(buf, 0, 5);
        while(1)
        {
                bytes_read = read(fd, buf, 4);
                if ( 0 > bytes_read )
                {
                        return FAILURE;
                }
                if ( '$' == buf[bytes_read - 1])
                {
                        strncat(str,buf,bytes_read-1);
                        break;
                }
                else
                {
                        strncat(str,buf,bytes_read);
                }
                bytes_read = 0;
                memset(buf, 0, 5);
        }
        len = strlen(str);
        str[len] = '\0';
        return SUCCESS;
}


*/



/* Write Data to  Socket  */
/*
int write_data( int fd, char *str)
{
        int written = 0;
        int bytes_written =0;
        int len = 0;
        len = strlen(str);
        str[len] = '$';
        len = len + 1;
        while(1)
        {
                written = write(fd, (str + bytes_written) , len - bytes_written);
                if ( 0 > written)
                {
                        return FAILURE;
                }
                if ( 0 == written)
                {
                        break;
                }
                bytes_written += written;
                written = 0;

        }
        return SUCCESS;
}
*/
/* Write Data to File  */

int write_filesync( int fd, char *str)
{
        int written = 0;
        int bytes_written =0;
        int len = 0;
	if ( str == NULL )
	{
		return SUCCESS;
	}
        len = strlen(str);
        while(1)
        {
                written = write(fd, (str + bytes_written) , len - bytes_written);
                if ( 0 > written)
                {
                        return FAILURE;
                }
                if ( 0 == written)
                {
                        break;
                }
                bytes_written += written;
                written = 0;

        }
        return SUCCESS;
}




/*removes user directory from string sent by client and stores in dir*/

int getclientdirectory(char *string, char *dir)
{
	int index = 0;
	int ptr = 0;
	int len = 0;
	char *temp = NULL;
	memset(dir, '\0', DATA_SMALL);
	for(index =0; index < strlen(string); index++)
	{
		if(string[index] == '*' && string[index+1] == '*')
		{
			ptr = index + 2;
			len = strlen(dir);
			dir[len] = '\0';
			temp = (char *)malloc(strlen(&string[ptr])+1);
			strcpy(temp,&string[ptr]);
			memset(string, 0, DATA_LARGE);
			strcpy(string,temp);
			free(temp);
			break;
		}
		else
		{
			strncat(dir,&string[index],1);
		}
	}	
			
	return SUCCESS;	
}






/* Appends user directory to get relative file path from src folder */

int getclientpath(char *dir,char *path)
{
	memset(path, 0, DATA_SMALL);
	char *backupspace = "./Dropbox/";
	strcpy(path,backupspace);
	strcat(path,dir);
	return SUCCESS;
}






/* extracts stats of a file from string and tokenises each stats and adds a node in clientlist for each file */

int createclientlist(char *string, node **headclient)
{
	int index = 0;
	int temp_index = 0;
	long int size = 0;
	long int time = 0;
        char fname[DATA_SMALL];
	memset(fname, 0, DATA_SMALL);
	char *filestat[4];
	memset(filestat, 0, 3);
	char temp[DATA_MEDIUM];
	memset(temp, 0, DATA_MEDIUM);
	int tok = 0;
	int ret = 0;

	for(index = 0, temp_index = 0; index < strlen(string); index++)
	{
		if ( string[index] != '*')
		{
			temp[temp_index] = string[index];
			temp_index++;
		}
		else
		{
			temp[temp_index] = '\0';
//			printf("temp file stat: %s\n",temp);
			tok = 0;
			filestat[tok] = strtok(temp, " ");
		 	while ( NULL != filestat[tok])
		       	{
				tok = tok + 1;
				filestat[tok] = strtok(NULL, " ");
			}
			tok = 0;
			strcpy(fname,filestat[0]);
			size = atoi(filestat[1]);
			time = atoi(filestat[2]);
			ret = addfilenode(headclient, size, time, fname);
			if ( FAILURE == ret )
			{
				printf("error inserting node\n");
				return FAILURE;
			}
			temp_index = 0;
			memset(temp, 0, DATA_MEDIUM);
		}
			
	}
	return SUCCESS;
}








/* scans  all files present in userdirectory and finds its stats and creates link list of server files stat */

int createserverlist(char *path, node **headserver, node *headclient, char  *filename)
{
	int ret = 0;
	struct dirent **namelist;
	char filestring[DATA_LARGE];
	memset(filestring, 0, DATA_LARGE);
	char filestat[DATA_LARGE];
	memset(filestat, 0, DATA_LARGE);
	char *tempname = ".nfs";
	ret = scandir(path,&namelist,0,alphasort);
        if (ret < 0)
        {
               perror("scandir");
        }
        if ( 2 == ret )
        {
		
		ret = getallfiles(headclient, filename); // get all file names 
		if ( FAILURE == ret )
		{
			printf("error getting all filenames\n");
			return FAILURE;
        	}
	}
	else
        {
                while(1)
                {       ret--;
                        if (ret == 1)
                                break;
			if (strncmp(tempname, namelist[ret]->d_name, 4))
			{
                        	strcat(filestring,namelist[ret]->d_name);
                        	strcat(filestring," ");
			}
                        free(namelist[ret]);
			namelist[ret] = NULL;
			
		}
		free(namelist[0]);
		free(namelist[1]);
                free(namelist);
		namelist = NULL;
        
        	printf("The file names at server side is %s\n",filestring);

        	ret = getserverfileparameter(filestring, path, filestat);
        	if ( ret == FAILURE )
        	{
                	perror("Error getting file parameters\n");
        	}
        	printf("The file parameters at server side %s\n",filestat);
		ret = createclientlist(filestat, headserver);
	
		if ( FAILURE ==  ret)
		{
			perror("error writing data");
			return FAILURE;
		}
	}	
	memset(filestat, 0, DATA_LARGE);
	return SUCCESS;
}






/* finds file stat for each filename in the userdirectory at server */

int getserverfileparameter(char *filenames,char *dir,char *statresult)
{
        int index = 0;
        int ret = 0;
        int len = 0;
        char fname[DATA_SMALL];
        memset(fname,0,DATA_SMALL);
        for (index = 0; index < strlen(filenames); index++)
        {

                if ( filenames[index] == ' ')
                {
                        len = strlen(fname);
                        fname[len] = '\0';
                        ret = addserverfilestat(fname,dir,statresult);
                        if ( FAILURE == ret )
                        {
                                return FAILURE;
                        }
                        memset(fname, '\0', DATA_SMALL);
                        continue;
                }
                else
                {

                        strncat(fname,&filenames[index],1);
                }

        }

        return SUCCESS;
}


/*finds the file stat of fname and stores the result in statresult*/


int addserverfilestat(char *fname,char *dir, char *statresult)
{

        struct stat buf;
        memset(&buf, '\0', sizeof(buf));
        int ret = 0;
        char path[DATA_MEDIUM];
	memset(path, 0, DATA_MEDIUM);
        strcpy(path,dir);
        strcat(path,"/");
        strcat(path,fname);
        ret = stat(path, &buf) ;
        if ( -1 == ret )
        {
                return FAILURE;
        }
        if ( S_ISREG(buf.st_mode))
        {
                char temp[DATA_SMALL];
                memset(temp, 0, DATA_SMALL);
                strcat(statresult, fname);
                strcat(statresult, " ");
                sprintf(temp, "%ld", buf.st_size);
                strcat(statresult, temp);
                memset(temp, 0, DATA_SMALL);
                strcat(statresult, " ");
                sprintf(temp, "%ld", buf.st_mtime);
                strcat(statresult, temp);
                strcat(statresult, "*");
        }
        else
        {
                return SUCCESS;
        }
        return SUCCESS;
}


/* adds the node with file stat to link list */

int addfilenode(node **head, long int size, long int time,  char *name)
{
        node *new = NULL;
	new = (node *)malloc(sizeof(node));
	if ( NULL == new )
	{
		printf("error inserting the node\n");
		return FAILURE;
	}
	new->next = NULL;
	strcpy(new->fname,name);
	new->fsize = size;
	new->mtime = time;
	if ( *head == NULL)
	{
		*head = new;
	}
	else
	{
		new->next = *head;
		*head = new;
	}
	return SUCCESS;
}




/*frees to linklist for next client */

int free_list(node *head)
{
	node *p = NULL;
	if ( head == NULL)
	{
		printf("Link list already free\n");
		return FAILURE;
	}
	while ( NULL != head)
	{
		p = head -> next;
		free(head);
		head = p;
	}

		
		
	return SUCCESS;
}





/* displays the content of the node */

void displayfilestat(node *head)
{

	while( head!=NULL )
	{ 
  		printf("%s,%ld,%ld -> ", head->fname,head->fsize, head->mtime);
  		head=head->next;
	}
	printf("\n");
}



/*appends all file name to a string */

int getallfiles( node *head, char *filenames)
{

	memset(filenames, 0, DATA_LARGE);
	while( NULL != head )
	{
		strcat(filenames, head->fname);
		strcat(filenames," ");
		head = head->next;
	}

	return SUCCESS;
}




int comparelist(node *headserver, node *headclient, char *modfile, char *rmfile)
{
	node *pc = headclient;
	node *ps = headserver;
	node *ptr = NULL;
	int ret = 0;
	while( NULL != pc )
	{
		ret = searchfile(headserver, pc->fname, &ptr);
		if ( FAILURE == ret)
		{
			printf("error searching file\n");
			return FAILURE;
		}
		if (NULL == ptr)
		{
			strcat(modfile, pc->fname);
			strcat(modfile, " ");
			pc = pc->next;
		}
		else if( ptr->fsize != pc->fsize || ptr->mtime < pc->mtime )
		{
			strcat(modfile, pc->fname);
			strcat(modfile, " ");
			pc = pc -> next;
		}
		else
		{
			pc = pc->next;
		}
	}

	while ( ps != NULL )
	{
		
		ret = searchfile(headclient, ps->fname, &ptr);
                if ( FAILURE == ret)
                {
                        printf("error searching file\n");
                        return FAILURE;
                }
                if (NULL == ptr)
                {
                        strcat(rmfile, ps->fname);
                        strcat(rmfile, " ");
                }
		ps = ps -> next;
	}
				
	
	return SUCCESS;
}


int searchfile(node *head, char *name, node **ret )
{
	node *p = head;
	while ( NULL != p )
	{
		if ( !strcmp(p->fname, name) )
		{
			break;
		}
		else
		{
			p = p->next;
		}
	}
	*ret = p;
	return SUCCESS;
}

int removefile (char *rmfiles, char *dir)
{
	char path[DATA_MEDIUM];
   	int index = 0;
	int ret = 0;
	int len = 0;
	char fname[DATA_SMALL];
	memset(fname, 0, DATA_SMALL);
	memset(path, 0, DATA_MEDIUM);
	for (index = 0; index < strlen(rmfiles); index++)
        {

                if ( rmfiles[index] == ' ')
                {
                        len = strlen(fname);
                        fname[len] = '\0';
		        strcpy(path,dir);
        		strcat(path,"/");
        		strcat(path,fname);
			ret = remove(path);
                        if ( 0  != ret )
                        {	
                                return FAILURE;
                        }
                        memset(fname, '\0', DATA_SMALL);
		        continue;
                }
                else
                {

                        strncat(fname,&rmfiles[index],1);
                }

			
	}
	
	return SUCCESS;
}
	

/*updates file contents of files to be synchronized */

int updatefiles (char *filecontent, char *path)
{
	char *namevalue[DATA_MEDIUM];
	memset(namevalue, 0, DATA_MEDIUM);
	int index = 0;
	int count = 0;
	int fd = 0;
	char fname[DATA_SMALL];
	memset(fname, 0, DATA_SMALL);
	int ret = 0;
	namevalue[index] = strtok(filecontent, "^");
	while (NULL != namevalue[index])
	{
		index = index + 1;
		namevalue[index] = strtok(NULL, "^");
	}

	count = index;
	
	for(index = 0; index < count ; index += 2)
	{
		strcat(fname, path);
		strcat(fname, "/");
		strcat(fname, namevalue[index]);
		fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
		if(fd < 0)
	        {
        	        printf("Cant open File %s for WRITING \n", fname);
               		perror("Opening File error");
                	return FAILURE;
        	}
		ret = write_filesync(fd, namevalue[index+1]);
		if (FAILURE == ret)
		{
			printf("Error writing on file %s \n",fname);
			return FAILURE;
		}
		memset(fname, 0, DATA_SMALL);
		
	}
	return SUCCESS;
	
}


int syncserver(connfd)

{		
		
		char modfile[DATA_LARGE];
        	char string[DATA_LARGE];
		char dir[DATA_SMALL];
		char path[DATA_SMALL];
		char rmfile[DATA_LARGE];
		char message1[DATA_SMALL]; // empty msg;
		char message2[DATA_SMALL];
		memset(message1, 0, DATA_SMALL);
		memset(message2, 0, DATA_SMALL);
		strcpy(message1,"^%^");
		strcpy(message2, "Synchronization Successfull");

		char message3[DATA_SMALL]; // #sync# username 
		memset(message3, 0, DATA_SMALL);
		strcpy(message3,"sync"); 

		memset(dir, 0, DATA_SMALL);	
		memset(path, 0, DATA_SMALL);
		//memset(string, 0, DATA_LARGE);
		//memset(rmfile, 0, DATA_LARGE);
		//memset(modfile, 0, DATA_LARGE);
		memset(dir, 0, DATA_SMALL);
		memset(path, 0, DATA_SMALL);
		//memset(buffer, 0, DATA_SMALL);
		//memset(filecontent, 0, DATA_LARGE);
		int ret = 0;
		node *headclient = NULL;
		node *headserver = NULL;
		char filecontent[DATA_LARGE];
		memset(filecontent, 0, DATA_LARGE);
		memset(modfile,0, DATA_LARGE);
        	memset(string,0, DATA_LARGE);
		memset(rmfile,0, DATA_LARGE);
		

		/*ret = read_data(connfd,message3);
		if ( FAILURE == ret)
		{
			return FAILURE;
		}
		*/
		printf("inside syncserverfunction\n");
		ret = write_data(connfd,message3);
		if (FAILURE == ret )
		{		
			return FAILURE;
		}
		
		printf("acknowledeged sync wrote sync to socket\n");
		printf("waiting for client to send file stat\n");
		ret = read_data(connfd, string);
		if ( FAILURE == ret )
		{
			perror("Server error reading socket");
			exit(1);
		}
		printf("received string:\n%s\n",string);
		if ( !strcmp(string,message1))
		{
			return SUCCESS;
		}
                ret = getclientdirectory(string,dir);
		if ( FAILURE == ret )
		{
			perror("Error finding userspace");
			exit(1);
		}
		printf("username of client is: %s\n",dir);
		ret = getclientpath(dir, path);
		 
		if ( FAILURE == ret )
		{
			perror("Error finding userspace");
			exit(1);
		}
		printf("the client backup directory is at path:\n%s\n",path);
		printf("The client files stat is %s\n",string);	         
		ret = createclientlist(string,&headclient);
			
		if ( FAILURE == ret )
		{
			perror("Error extracting file stat into list");
			exit(1);
		}
	        displayfilestat(headclient);	
		printf("***client string is processed****\n");
		ret = createserverlist(path, &headserver, headclient, modfile);
		
		if ( FAILURE == ret )
		{
			perror("Error finding userspace");
			exit(1);
		}
               
		if ( NULL != headserver) 
		{
			ret = comparelist(headserver, headclient, modfile, rmfile);
			if ( FAILURE == ret )
			{
				printf("error comparing files stat\n");
				exit(1);
			}
		}
		
		displayfilestat(headserver);
                printf("file to be requested is %s\n", modfile);
                printf("file to be deleted are %s\n", rmfile);
		
		if (NULL != rmfile)
		{
			ret = removefile(rmfile,path);
			if ( FAILURE == ret )
			{
				printf("error removing file at server\n");
				exit(1);
			}
		}

		if ( NULL != modfile )
		{		
			ret = write_data(connfd, modfile);
                	if ( FAILURE == ret )
                	{
                        	perror("Server error writing socket");
                        	exit(1);
               		}	
		}
		else
		{
			ret = write_data(connfd, message1);
                	if ( FAILURE == ret )
                	{
                        	perror("Server error writing msg  socket");
                        	exit(1);
                	}
		}
		
		ret = read_data(connfd, filecontent);
		if ( FAILURE == ret )
		{
			perror("error reading file content from socket");
			exit(1);
		}

		printf("The file content received is\n%s\n", filecontent);

		ret = updatefiles(filecontent, path);
		if ( FAILURE == ret )
		{
			perror("Error updating file");
			exit(1);
		}
		else
		{
			ret = write_data(connfd, message2);
			if ( FAILURE == ret )
			{
				perror("error writing success messsage\n");
				exit(1);
			}
		}

 
                if ( NULL != headclient )
		{
		
			ret = free_list(headclient);
				
			if ( FAILURE ==  ret)
			{
				perror("error freeing list");
				exit(1);
			}
			headclient = NULL;
			printf("clientlist freed\n");
		}
		if ( NULL != headserver )
		{
			ret = free_list(headserver);
			if( FAILURE == ret)
			{
				perror("error freeing server list\n");
				exit(1);
			}
			headserver = NULL;
			printf("server list freed\n");

		}
        

	return SUCCESS; 

}



