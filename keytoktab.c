/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
        {"id", 	        id},
	{"number", 	number},
	{":=", 	        assign},
	{"undef", 	undef},
	{"predef", 	predef},
	{"tempty", 	tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	input},
	{"output", 	output},
	{"var", 	var},
	{"begin", 	begin},
	{"end", 	end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	real},
	{"KERROR", 	nfound}
	} ;
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{  
	printf("\n	The Program Keywords\n");
    printf("________________________________________________________\n");
    int size = sizeof(keywordtab) / sizeof(tab) - 1;
	printf("\nsizeof(keywordtab)%ld, sizeofTab:%ld, size: %d\n", sizeof(keywordtab), sizeof(tab), size);
	
	for(int i = 0; i < size; i++) {
        printf("%10s %4d\n", keywordtab[i].text, keywordtab[i].token);
    }

    printf("________________________________________________________\n");
    printf("	The Program Tokens\n");
	printf("________________________________________________________\n");
	size = sizeof(tokentab) / sizeof(tab) - 1;
    printf("\nsizeof(tokentab)%ld, sizeofTab:%ld, size: %d\n", sizeof(tokentab), sizeof(tab), size);
    for(int i = 0; i < size; i++) {
        printf("%10s %4d\n", tokentab[i].text, tokentab[i].token);
    }

    printf("________________________________________________________\n");
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
	/* Non-keyword tokens */
	int size=sizeof(tokentab) / sizeof(tab);

   	for(int i = 0; i < size; i++) {
        if(!strcmp(tokentab[i].text, fplex)) {
            return tokentab[i].token;
        }
    }

	/* Keyword tokens */
	size = sizeof(keywordtab) / sizeof(tab);
	for(int i = 0; i < size; i++) {
        if(!strcmp(keywordtab[i].text, fplex)) {
            return keywordtab[i].token;
        }
    }

	if(isalpha(*fplex)){
		return id;
	} 
	else{
		return number;
	} 
		
   }

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
   for(int i = 0; i < sizeof(keywordtab) / sizeof(tab); i++) {
		if(!strcmp(keywordtab[i].text, fplex)) {
			return keywordtab[i].token;
		}
	}
	return id;
   }

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
	/*non keyword tokens*/
	int size = sizeof(tokentab) / sizeof(tab);

   	for(int i = 0; i < size; i++) {
        if(tokentab[i].token == ftok) {
            return tokentab[i].text;
        }
    }

	/* keyword tokens */
	size = sizeof(keywordtab) / sizeof(tab);

	for(int i = 0; i < size; i++) {
        if(keywordtab[i].token == ftok) {
            return keywordtab[i].text;
        }
    }

    return "TERROR"; 
   }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
