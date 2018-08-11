#include<header.h>
// global hash table 
user *h_table[10] ;
 
 
int register_user( char *client_string )
{
    
    char *word = NULL , *name_user = NULL ;
    int avail_ret ;
    word = (char *)malloc( DATA_SMALL * sizeof( char ) ) ;
    if( NULL == word )
    {
 printf(" cannot allocate memory to variable word in check_availability () " ) ;
 return FAILURE ;
    }
    // TOKENISING TWO TIMES TO FETCH USERNAME IN WORD 
    word = strtok( client_string , " " ) ;
    puts(word);
    word = strtok( NULL , " " ) ;
   // puts(word);
    name_user = word ;
    avail_ret = check_availability( name_user ) ;

    // if user name already exist then check_availability will return FAILURE i.e 1 

    if( avail_ret )
    {
 printf( " check_availability() returned FAILURE " ) ;
 return FAILURE ;
    }

    //  creating new user 

    int h_key ;
    int sys_ret ;
    user *newuser = NULL ;
    newuser = (user *)malloc( sizeof( user ) ) ;
    if( NULL == newuser )
    {
 printf(" cannot allocate memory to variable word in check_availability () " ) ;
 return FAILURE ;
    }

    newuser->user_name = (char*)malloc( DATA_SMALL * sizeof( char ) ) ;

    if( NULL == newuser->user_name )
    {
 printf(" cannot allocate memory to variable word in check_availability () " ) ;
 return FAILURE ;
    }
    newuser->user_password = (char*)malloc( DATA_SMALL * sizeof( char ) ) ;
    if( NULL == newuser->user_password )
    {
 printf(" cannot allocate memory to variable word in check_availability () " ) ;
 return FAILURE ;
    }
	
    strcpy( newuser->user_name , word ) ;
  //  printf("username %s",newuser->user_name);
    
    // fetching password sent by client 
    word = strtok( NULL , " " ) ;
    strcpy( newuser->user_password , word ) ;
    h_key = func_hash( newuser->user_name ) ;
 
    // insertion module 

 if ( h_table[ h_key ] == NULL )
 {
  h_table[ h_key ] = newuser ;
  newuser->next = NULL ;
 }
 else
 {
  newuser->next = h_table[ h_key ] ;
  h_table[ h_key ] = newuser ;
 }
 int dir_ret ;
 dir_ret = create_user_directory( name_user ) ;
 if( dir_ret == FAILURE ) 
 { 
     return FAILURE ;
 }
 return SUCCESS ;
}
 
 
 
 
 

 // folder creation module

int create_user_directory( char *name_user )
{
 char  make_dir[25] ;
 memset( make_dir , 0 , 25 ) ;
 strcpy( make_dir , "mkdir ./Dropbox/") ;
 // fetching username again 
/*
 word = strtok( client_string ," ") ;
 printf(" word fetched \n");
 puts(word ) ;
 word = strtok( NULL , " " ) ;
 printf(" word fetched for user name \n");
 puts(word ) ;
 printf(" word fetched for user name \n");

 strcpy( dir_name , word ) ;
*/
 strcat(make_dir , name_user  ) ;
 int sys_ret ;
 sys_ret = system( make_dir ) ;
 if( sys_ret == -1 )
 {
     printf(" making directory for user failed ...!! \n " );
     return FAILURE ;
 }
 return SUCCESS ;
}
 
 
 
 
 

 // check availability o fuser name function 
 
int check_availability( char *user_name )
{

 int h_key ;
 user *temp = NULL ;
 h_key = func_hash( user_name ) ;

   temp = h_table[ h_key ] ;
   while ( temp != NULL )
   {
       if( strcmp( temp->user_name , user_name ) == 0 )
       {
    return FAILURE ;
       }
       else
       {
   
    temp= temp->next ;
       }
   }
 
 return SUCCESS ;
}
 
 
 
 
//   hashing function 
 
 int func_hash (
  char name[] /* The string whose hash_key of the ascii values of the characters is to be generated. */
  )
 {
     int 
  ascii_sum = 0 , /* Summation of the ascii values of the characters of the string. */
     index = 0 , /* index of the loop */
     hash_key = 0 ; /* The return value - hash value of the sum of the strings */

     while (name[index] != '\0')
     {
  ascii_sum = ascii_sum + (int)name[index] ;
  index ++ ;
     }
     hash_key = ascii_sum % MODULOUS ;
     return (hash_key) ;
 }
 
 
