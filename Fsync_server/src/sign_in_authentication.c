#include"header.h"


int sign_in_authentication( char *client_string )
{


    	printf(" inside sign_in_authentication() \n ");

	int hash_index ;

	user *temp = NULL ;

	char *word = NULL ;


	//  fetching user name 

	
	word = strtok( client_string , " " ) ;

	word = strtok( NULL , " " ) ;

	 hash_index = func_hash( word ) ;


	// searching usern ame in hash table 


			temp = h_table[ hash_index ] ;

			while ( temp != NULL )
			{

			    if( strcmp( temp->user_name , word ) == 0 )
			    {

				word = strtok( NULL , " " ) ;

				if ( strcmp( temp->user_password , word ) == 0 )
				{
				    return SUCCESS ;

				}
				else 
				{
				    return FAILURE ;

				}

			    }
			    else
			    {
			
				temp= temp->next ;

			    }
			}
	

	return FAILURE ;

}
