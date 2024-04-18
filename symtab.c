/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT                                */
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
#define TABSIZE 1024 /* symbol table size           */
#define NAMELEN 20   /* name length                 */

typedef char tname[NAMELEN];

static tname name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int size[TABSIZE];
static int addr[TABSIZE];

static int numrows = 0; /* number of rows in the ST    */
static int startp = 0;  /* start position program in ST*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char *get_name(int ftref) { return name[ftref]; }
static toktyp get_role(int ftref) { return role[ftref]; }
static toktyp get_type(int ftref) { return type[ftref]; }
static int get_size(int ftref) { return size[ftref]; }
static int get_addr(int ftref) { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char *fpname) { strcpy(name[ftref],
                                                       fpname); }
static void set_role(int ftref, toktyp frole) { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype) { type[ftref] = ftype; }
static void set_size(int ftref, int fsize) { size[ftref] = fsize; }
static void set_addr(int ftref, int faddr) { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static void addrow(char *fname, toktyp frole, toktyp ftype,
                   int fsize, int faddr)
{
   set_name(numrows, fname);
   set_role(numrows, frole);
   set_type(numrows, ftype);
   set_size(numrows, fsize);
   set_addr(numrows, faddr);

   numrows++;
}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/
static void initst()
{
   addrow(tok2lex(predef), typ, predef, 0, 0);
   addrow(tok2lex(undef), typ, predef, 0, 0);
   addrow(tok2lex(error), typ, predef, 0, 0);
   addrow(tok2lex(integer), typ, predef, 4, 0);
   addrow(tok2lex(boolean), typ, predef, 4, 0);
   addrow(tok2lex(real), typ, predef, 8, 0);
}

/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char *fpname)
{
   for (int i = startp; i < numrows; i++)
   {
      if (!strcmp(get_name(i), fpname))
      {
         return i;
      }
   }
   return nfound;
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_symrow(int ftref)
{
   for (int i = startp; i < ftref; i++)
   {
      printf("%11s %10s %10s %9i %9i\r", get_name(i), tok2lex(get_role(i)), tok2lex(get_type(i)), get_size(i), get_addr(i));
   }
}

void p_symtab()
{
   printf("\r________________________________________________________\r");
   printf("\n THE SYMBOL TABLE\n");
   printf("\r________________________________________________________\r");
   printf("%11s %10s %10s %9s %9s\r", "NAME", "ROLE", "TYPE", "SIZE", "ADDR");
   printf("________________________________________________________\r");
   p_symrow(numrows);
   printf("________________________________________________________\r");
   printf("\r STATIC STORAGE REQUIRED is %i BYTES\r", get_size(startp));
   printf("\r________________________________________________________\r");
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
void addp_name(char *fpname)
{
   
   initst();
   startp = numrows;
   addrow(fpname, program, program, 0, 0);
   
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char *fpname)
{
   addrow(fpname, var, nfound, 0, 0);
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char *fpname)
{
   if (get_ref(fpname) != nfound)
   {
      return 1;
   }
   return 0;
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/
void setv_type(toktyp ftype)
{
   int typeSize = 0;
   if (ftype == integer || ftype == boolean)
   {
      typeSize = 4;
   }
   else if (ftype == real)
   {
      typeSize = 8;
   }
   for (int i = startp; i < numrows; i++)
   {
      if (get_type(i) == nfound)
      {

         set_type(i, ftype);
         set_size(i, typeSize);
         set_size(startp, get_size(startp) + typeSize);
         if (i == startp + 1)
         {
            set_addr(i, 0);
         }
         else
            set_addr(i, (get_size(i - 1) + get_addr(i - 1)));
      }
   }
}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char *fpname)
{
   if (find_name(fpname))
   {
      return (get_type(get_ref(fpname)));
   }
   return undef;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/