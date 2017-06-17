#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

//error
#include "../error/error.h"

#define ini_comment 	0
#define ini_section 	1
#define ini_parameter 	2
#define ini_other	 	3

/* Trim left and right white space
* Source : http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
*/
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( str + len - 1 != endp )
            *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp )
            *str = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }


    return str;
}



/* getSectionParameterValue return the value of a parameter in a target section
* Input  : pPath file path of the ini file
* Input  : section name
* Input	 : parameter name
* Output : value of parameter from section
* Return : error code
*/

f_return getSectionParameterValue(char *pPath, char *section, char *parameter, char *value)
{
	int fileLen=0;
	char buffer[FILENAME_MAX];
	int validsection = 0;
	int linetype = 0;											// 0 = commentaire | 1 = section | 2 = paramètre
	char *token=NULL;
	char valid_data_found=0;
	f_return err;
	char* current_string = malloc(255*sizeof(char));
	
// Chargement du fichier
	FILE *fp;
	fp = fopen(pPath,"r");
	
	if(fp == NULL)
	{
		perror("Error opening file");
		return err;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		fileLen=ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fread(buffer, fileLen, 1, fp); 
		fclose(fp);
	}

	token = strtok(trim(buffer), "\n");
	strncpy(current_string, token, strlen(token)	);
// printf("\n|%s|->[%s] : lenght : %d",token,current_string,strlen(token));
	

	while(token!=NULL)
	{

	
	 current_string = malloc(strlen(token));
	
	// Remove comment in the lines
		if((int)strchr(token,';') != 0)
		{
			 int pos = (int)strchr(token,';') - (int)token;
			 strncpy(current_string,token,pos);
			 // printf("\n %s->%d", trim(current_string),strlen(trim(current_string)));
		}

		else
		{
			strncpy(current_string, token, strlen(token)	);
		}
/// Skip lines with errors (contains both [,],=)
	
	
	// check if it's good section

	if(strchr(current_string,'[')!=NULL && strchr(current_string,']') != NULL)
		{

			linetype = ini_section;	
			char*	section_name 	= malloc(strlen(current_string) + 1); 
			int 	word_lenght		= strchr(current_string,']') - strchr(current_string,'[') - 1 ; 
			int 	word_start 		= (int)strchr(current_string,'[') + 1;
			
			strncpy(section_name,strchr(current_string,'[') + 1,word_lenght);					
			//When parameter section is found
			if(strcmp( trim(section_name),trim(section))==0)
				validsection = 1;
			else
				validsection = 0;
			
		}
	
	// check if it's good parameter
	
	if(validsection == 1)
		{
			if(strchr(current_string,'=')!=NULL)
			{
			char*	param_name 			= 	malloc(strlen(current_string) + 1);
			char*	param_value			=	malloc(strlen(current_string) + 1);
			int 	param_lenght		=  	(current_string + strlen(current_string)) - strchr(current_string,'=');
			int  	param_start 		= 	(int)strchr(current_string,'=') + 1;	
			
			strncpy(param_value,strchr(current_string,'=')+1,param_lenght);
			strncpy(param_name,current_string,strchr(current_string,'=') - current_string);
			// printf("\n > |%s| - |%s|",trim(param_name),trim(parameter));
		
	// return the value	
			if(strcmp( trim(param_name),trim(parameter)	)==0)
				{
					/// retourner la valeur
					valid_data_found = 1;
					strcpy(value,param_value);
					// printf("\nvalue %s->%s",param_name,value);
					err = seterror(0,0,"OK");
					return err;
				}
			}
		}
	

	token = strtok(NULL, "\n");

	current_string = NULL;

	}
	
	if(validsection == 0)
	{
		err = seterror(1,-1,"Section not found");
		return err;
	}

	if(valid_data_found == 0)
	{
		
		err = seterror(2,-1,"parameter not found");
		return err;
	}
	


}


float getSPVf(char *pPath, char *section, char *parameter)
{
char* value = malloc(255*sizeof(char));

	f_return result = getSectionParameterValue(pPath,section,parameter,value);

		if(result.errorstate != -1)
		{
			return atof(trim(value));
		}
		else
		{
			return atof(trim("0"));
		}

}

int getSPVi(char *pPath, char *section, char *parameter)
{
char* value = malloc(255*sizeof(char));

	f_return result = getSectionParameterValue(pPath,section,parameter,value);

		if(result.errorstate != -1)
		{
			return atoi(trim(value));
		}

}

char* getSPVc(char *pPath, char *section, char *parameter)
{
char* value = malloc(255*sizeof(char));

	f_return result = getSectionParameterValue(pPath,section,parameter,value);
	
		if(result.errorstate != -1)
		{
			
			return (trim(value));
			
		}

}