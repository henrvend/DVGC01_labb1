/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
#include "keytoktab.h"          /* when the keytoktab is added   */
#include "lexer.h"              /* when the lexer     is added   */
/* #include "symtab.h"      */ /* when the symtab    is added   */
/* #include "optab.h"       */ /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
static int lookahead = 0;
static int is_parse_ok = 1;

static int expr();

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
// enum tvalues { program = 257, id, input, output };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                       var, id, ',', id, ',', id, ':', integer, ';',
                       begin,
                       id, assign, id, '+', id, '*', number,
                       end, '.',
                       '$'};

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
static int pget_token()
{
    static int i = 0;
    if (tokens[i] != '$')
        return tokens[i++];
    else
        return '$';
}

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if (DEBUG)
        printf("\n --------In match expected: %4s, found: %4s",
               tok2lex(t), tok2lex(lookahead));
    if (lookahead == t)
        lookahead = pget_token();
    else
    {
        is_parse_ok = 0;
        printf("\n *** Unexpected Token: expected: %4s found: %4s (in match)",
               tok2lex(t), tok2lex(lookahead));
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void program_header()
{
    if (DEBUG)
        printf("\n *** In  program_header");
    match(program);
    match(id);
    match('(');
    match(input);
    match(',');
    match(output);
    match(')');
    match(';');
}

/*var_part*/

static void type()
{
    if (lookahead == integer)
    {
        match(integer);
    }
}

static void id_list()
{
    match(id);
    if (lookahead == ',')
    {
        match(',');
        id_list();
    }
}

static void var_dec()
{
    id_list();
    match(':');
    type();
    match(';');
}

static void var_dec_list()
{
    var_dec();
    if (lookahead == id)
    {
        var_dec_list();
    }
}

static void var_part()
{
    if (DEBUG)
        printf("\n *** In  var_part");
    match(var);
    var_dec_list();
}

/*stat_part*/

static int operand()
{

    int result;
    if (lookahead == id)
    {
        match(id);
        result = id;
        return result;
    }
    else if (lookahead == number)
    {
        match(number);
        result = number;
        return result;
    }
    else
    {
        return 0;
    }
}

static int factor()
{
    int result;
    if (lookahead == '(')
    {
        match('(');
        result = expr();
        match(')');
    }
    else
    {
        result = operand();
    }
    return result;
}

static int term()
{

    int result = factor();
    if (lookahead == '*')
    {
        match('*');
        result *= term();
    }
    return result;
}

static int expr()
{

    int result = term();
    if (lookahead == '+')
    {
        match('+');
        result += expr();
    }

    return result;
}

static void assign_stat()
{
    match(id);
    match(assign);
    expr();
}

static void stat()
{
    assign_stat();
}

static void stat_list()
{
     if (DEBUG) printf("\n *** In  stat_part");
    stat();
    if (lookahead == ';')
    {
        match(';');
        stat_list();
    }
     if (DEBUG) printf("\n *** In  stat_part");
}

static void stat_part()
{
    if (DEBUG)
        printf("\n *** In  stat_part");

    match(begin);
    stat_list();
    match(end);
    match('.');
    if (DEBUG)
        printf("\n *** Out  stat_part");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    if (DEBUG)
        printf("\n *** In  parser");
    lookahead = pget_token(); // get the first token
    program_header();         // call the first grammar rule
    var_part();
    stat_part();
     if (DEBUG)
        printf("\n *** Out  parser");
    return is_parse_ok; // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
