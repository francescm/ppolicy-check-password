#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "portable.h"
#include "slap.h"



int init_module()
{
    return 0;
}


int check_password(char *pPasswd, char **ppErrStr, Entry *pEntry)
{
    char pwqr=0;
    char retmsg[255];
    char *message;
    
    int has_digit = 0;
    int has_alpha = 0;
    char digit[11];
    strcpy(digit, "0123456789");
    char alpha[60];
    strcpy(alpha, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    char valid[20];
    strcpy(valid, "/%$£^&@()*+-=;:,_^");
        
    int i;
    int j;
    char bad_char;
    //    bad_char = (char *)malloc(3);

    for (i=0; i < strlen(pPasswd); i++)
      {
	int found = 0;
	char this_char = pPasswd[i];
	for (j=0; j < strlen(digit); j++)
	  {
	    if (this_char == digit[j])
	      {
		has_digit = 1;
		found = 1;
	      }
	  }
	for (j=0; j < strlen(alpha); j++)
	  {
	    if (this_char == alpha[j])
	      {
		has_alpha = 1;
		found = 1;
	      }
	  }
	for (j=0; j < strlen(valid); j++)
	  {
	    if (this_char == valid[j])
	      {
		found = 1;
	      }
	    else
	      {
		bad_char = this_char;
	      }
	  }
	if ( 0 == found )
	  {
	    pwqr = 1;
	    char inv_char_msg[80];
	    sprintf(inv_char_msg, "1: invalid char: %c", bad_char);

	    strcpy(retmsg , inv_char_msg);
	  }
      }

    if ( 0 == has_digit )
      {
	pwqr = 1; 
	strcpy(retmsg , "2: no digit");
      }
    if ( 0 == has_alpha )
      {
	pwqr = 1; 
	strcpy(retmsg , "3: no alpha");	    
      }


  /* Allocate  */
    message = (char *)malloc(strlen(retmsg)+1);
  /* Copy the contents of the string. */

    message = strdup(retmsg);

    *ppErrStr = message;

    return pwqr;
}

