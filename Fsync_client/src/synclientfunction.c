#include"header.h"

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


/* get file parameters of each client file and store in statresult*/

int getfileparameter(char *filenames,char *dir,char *statresult)
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
			ret = addfilestat(fname,dir,statresult);
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




/* concatenates file parameter of each file in statresult */

int addfilestat(char *fname,char *dir, char *statresult)
{

	struct stat buf;
	memset(&buf, '\0', sizeof(buf));
        int ret = 0;
	char path[DATA_SMALL];
	memset(path, 0, DATA_SMALL);	
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




/* get file contents of the files requested by server and stores in string */

int getfiles(char *filelist, char **string, char *path)
{
	char *ptr[DATA_MEDIUM];
        memset(ptr, 0, DATA_MEDIUM);
        int ret = 0;
	int index = 0;
	int count = 0;
	ptr[index] = strtok(filelist, " ");
	while(NULL != ptr[index] )
	{
		index = index + 1;
		ptr[index] = strtok( NULL, " " );
	}
	count = index;
	
        ret = concatfile(string, count, ptr,path);
	
	return SUCCESS;
}




/* concats file names and all file contents of requested files  and stores in s*/ 

int concatfile(char **s , int argc , char *argv[], char *path)
{
        int fd = 0;
        char buf[DATA_MEDIUM] ;
        memset(buf, '\0', DATA_MEDIUM);
        int index = 0;
        *s = (char*)malloc(DATA_MEDIUM);
	memset(*s, 0, DATA_MEDIUM);
        int readd =0;
        int bytes_read = 0;
	int len = 0;
	char fname[DATA_MEDIUM];
	memset(fname, 0, DATA_MEDIUM);
	
        for(index = 0 ; index < argc; index++)
        {

		strcpy(fname,path);
		strcat(fname, "/");
		strcat(fname,argv[index]);
			
                fd = open(fname , O_RDONLY ) ;
                if ( fd < 0)
                {
                        printf("error opening file %s\n",fname);
                        perror("Reason:");
                        continue ;
                }
		
		strcat(*s,argv[index]);
		strcat(*s,"^^");
		len = strlen(argv[index])  + 5;
                while ((bytes_read  = read(fd , buf , DATA_MEDIUM)) > 0 )
                {

                        readd += bytes_read + len;

                        if( readd < DATA_MEDIUM )
                        {
                                 strncat(*s,buf,bytes_read);
                        }
                        else
                        {
                                *s = realloc(*s ,readd);
                                strncat(*s,buf,bytes_read);
                        }
                        memset(buf, '\0' , DATA_MEDIUM );
                }
		strcat(*s, "^^");
                memset(buf, '\0' , DATA_MEDIUM) ;
		memset(fname, 0, DATA_MEDIUM);
                close(fd);
         } 
         return SUCCESS;
}


/************getclientdirectoryfiles*******/


int getclientdirectoryfiles(int sd, char *path, char  *filestring)
{
	int ret = 0;
	struct dirent **namelist = NULL;
	char *tempname = ".nfs";
	char emptymsg[DATA_SMALL];
	memset(emptymsg, 0, DATA_SMALL);
	strcpy(emptymsg, "^%^");

	ret = scandir(path,&namelist,0,alphasort);
        if (ret < 0)
        {
               perror("scandir");
        }

	if ( 2 == ret )
	{
		printf("Synchronization Folder is empty\n");
		ret = write_data(sd, emptymsg);
		if ( FAILURE == ret )
		{
			perror("error writing on socket");
		}
		return EMPTY;
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
		namelist[0] = NULL;
		namelist[1] = NULL;
                free(namelist);
		namelist = NULL;
        
	}
	return SUCCESS;
}












/*********syncclient***********************/

int syncclient(int sd, char *username)
{	

        char string[DATA_LARGE];
	int ret = 0;
	char sendfilelist[DATA_MEDIUM];
	memset(sendfilelist, 0, DATA_MEDIUM);
	char *storagespace = "./Dropbox";
	char dir[DATA_SMALL];
	memset(dir, 0, DATA_SMALL);
        memset(string, 0, DATA_LARGE);
	char *filestat = NULL;
	filestat = (char *)malloc(DATA_LARGE);
	memset(filestat, 0, DATA_LARGE);
	char *filedata = NULL;
	char message[DATA_SMALL];
	memset(message, 0, DATA_SMALL);

	strcpy(filestat,username);
	strcat(filestat,"**");
	char ack[DATA_SMALL];
	memset(ack, 0, DATA_SMALL);

	
	strcat(dir,storagespace);
	//strcat(dir,username);

	ret = read_data(sd, ack);
	if ( FAILURE == ret )
	{
		return FAILURE;
	}

	ret = getclientdirectoryfiles(sd,dir,string);
	if ( FAILURE == ret )
	{
		printf("error scanning directory\n");
		return FAILURE;
	}
	if ( EMPTY == ret )
	{	
		free(filestat);
		return SUCCESS;
	}

/*	ret = scandir(dir,&namelist,0,alphasort);
	if (ret < 0)
	{
               perror("scandir");
	}
	if ( 2 == ret )
	{
		printf("Synchronization Folder is empty\n");
		ret = write_data(sd, emptymsg);
		if ( FAILURE == ret )
		{
			perror("error writing on socket");
		}
		return 0;
	}

        else
 	{
                while(1) 
		{       ret--;
			if (ret == 1)
				break;
                	//printf("%s\n", namelist[ret]->d_name);
			strcat(string,namelist[ret]->d_name);
			strcat(string," ");
                	free(namelist[ret]);
			

                }
                free(namelist);
        }

		*/

	printf("%s\n",string);
	ret = getfileparameter(string, dir, filestat);
	if ( ret == FAILURE )
	{
		perror("Error getting file parameters\n");
		return FAILURE;
	}
	printf("%s\n",filestat);

	ret = write_data(sd, filestat);
        if ( FAILURE == ret )
	{
		perror("socket write error at client");
		return FAILURE;
	}
	ret = read_data(sd, sendfilelist);
	if ( FAILURE == ret )
        {
                perror("socket reading error at client");
                return FAILURE;
        }
	printf("the requested file is %s\n", sendfilelist);
	
	ret = getfiles(sendfilelist, &filedata, dir);
	if ( FAILURE == ret )
	{
		perror("error reading from files\n");
		return FAILURE;
	}

		
	ret = write_data(sd, filedata);
	if ( FAILURE == ret)
	{
		perror("error writing data on socket");
		return FAILURE;
	}
	
	ret = read_data(sd, message);
	if ( FAILURE == ret)
	{
		perror("error reading success message");
		return FAILURE;
	}
	printf("%s\n",message);
	free(filedata);
	free(filestat);
	filedata = NULL;
	filestat = NULL;
	
	return SUCCESS;
}
	
