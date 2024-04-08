/**********************************************************************/
/* lab 1 DVG C01 - Happy Hacker version  Donald Ross  060422          */
/**********************************************************************/
/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#define DEBUG 1
/**********************************************************************/
/* define tokens + keywords                                           */
/**********************************************************************/
enum tvalues { program = 257, id, input, output, var, integer };
/**********************************************************************/
/* Global Data Objects                                                */
/**********************************************************************/
int  lookahead=0;
int  is_parse_ok=1;
int tokens[] = {program, id, '(', input, ',', output, ')', ';', 
                var, id, ',', id, ',', id, ':', integer, ';', '$' };
/**********************************************************************/
/* Get the nexttoken from the buffer  (simulate the lexer)            */
/**********************************************************************/
int get_token()
{  
   static int i=0;
   if (tokens[i] != '$') return tokens[i++]; else return '$';
   }
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
void match(int t)
{
   if(DEBUG) printf("\n --------In match expected: %3d, found: %3d", 
                     t, lookahead);
   if (lookahead == t) lookahead = get_token();
   else {
      is_parse_ok=0;
      printf("\n *** Unexpected Token: expected: %3d found: %3d",
              t, lookahead);
      }
   }
/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");
   match(program); match(id); match('('); match(input);
   match(','); match(output); match(')'); match(';');
   }

void var_part(){
    if (DEBUG) printf("\n *** In  program_header");
    match(var);
}

int prog()
{
   if (DEBUG) printf("\n *** In  program");
   lookahead = get_token();        // get the first token
   program_header();               // call the first grammar rule
   var_part();
   return is_parse_ok;             // status indicator
   }
/**********************************************************************/
/* The main function (the driver)                                     */
/**********************************************************************/
int main()
{   
   return prog() ? printf(" \n Parse Successful! \n")
                 : printf(" \n Parse Failed! \n");
   }
/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
