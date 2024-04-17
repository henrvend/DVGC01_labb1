/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE 30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int pbuf = 0; /* current index program buffer  */
static int plex = 0; /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
   int i = 0;
   while ((buffer[i] = fgetc(stdin)) != EOF)
   {
      i++;
   }
   buffer[i++] = '$';
   buffer[i] = '\0';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer()
{
   printf("%s\n", buffer);
   printf("________________________________________________________\n");
   printf(" THE PROGRAM TEXT\n");
   printf("%s", buffer);
   printf("\n________________________________________________________\n");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
   lexbuf[plex] = buffer[pbuf];
   pbuf++;
   plex++;
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
   if (pbuf == 0)
   {
      get_prog();
      pbuffer();
   }
   
   memset(lexbuf, 0, LEXSIZE);
   plex = 0;

   /*skip spaces*/
   while (isspace(buffer[pbuf]))
   {
      pbuf++;
   }
   get_char();

   /*check if digit*/
   if (isdigit(lexbuf[0]))
   {
      while (isdigit(buffer[pbuf]))
      {
         get_char();
      }
      return number;
   }

   /*check if alfabetic*/
   if (isalpha(lexbuf[0]))
   {
      while (isalnum(buffer[pbuf]))
      {
         get_char();
      }
      return key2tok(lexbuf);
   }

   /*check if special char (:=)*/
   if ((lexbuf[0] == ':') && (buffer[pbuf] == '='))
   {
      get_char();
   }

   return lex2tok(lexbuf);
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char *get_lexeme()
{
   return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
