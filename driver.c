/*Henrik Vendel*/
/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */               
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "parser.h"

/**********************************************************************/
/* The main function (the driver)                                     */
/**********************************************************************/
int main()
{
   parser() ? printf(" \n Parse Successful! \n")
            : printf(" \n Parse Failed! \n");
            
   return 1;
   }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
