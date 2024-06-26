/**********************************************************************/
/* Henrik Vendel                                                      */
/* Adam Brattström                                                    */
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
#include "keytoktab.h" /* when the keytoktab is added   */
#include "lexer.h"     /* when the lexer     is added   */
#include "symtab.h"    /* when the symtab    is added   */
#include "optab.h"     /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int lookahead = 0;
static int is_parse_ok = 1;

static toktyp expr();

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
// enum tvalues { program = 257, id, input, output };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
/*static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                       var, id, ',', id, ',', id, ':', integer, ';',
                       begin,
                       id, assign, id, '+', id, '*', number,
                       end, '.',
                       '$'};
*/
/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
/*static int pget_token()
{
    static int i = 0;
    if (tokens[i] != '$')
        return tokens[i++];
    else
        return '$';
}*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void success()
{
    if (is_parse_ok)
    {
        printf("PARSE SUCCESSFUL!\n");
    }
    
    printf("________________________________________________________");
}

/*Error handling*/

static void error_id()
{
    printf("SYNTAX:   ID expected found %s\r", get_lexeme());
    is_parse_ok = 0;
}
static void error_symbol(char *symbol)
{
    printf("SYNTAX:   Symbol expected %s found %s\r", symbol, get_lexeme());
    is_parse_ok = 0;
}
static void error_operand() 
{
    printf("SYNTAX:   Operand Expected\r");
    is_parse_ok = 0;
}
static void error_type() 
{ 
    printf("SYNTAX:   Type name expected found %s\r", get_lexeme());
    is_parse_ok = 0; 
}
static void error_undeclared(){
    printf("SEMANTIC: ID NOT declared: %s\r", get_lexeme());
    is_parse_ok = 0; 
}
static void error_duplicate(){
    printf("SEMANTIC: ID already declared: %s\r", get_lexeme());
    is_parse_ok = 0;
}
static void error_assign(char * left, char *right){
    printf("SEMANTIC: Assign types: %s := %s\r", left, right);
    is_parse_ok = 0;
}
static void error_empty_file(){
    printf("SYNTAX:   Input file is empty\r");
    is_parse_ok = 0;
}
static void error_symbol_after_end(){
    printf("SYNTAX:   Extra symbols after end of parse!\r");
    is_parse_ok =0;
}


/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if (DEBUG)
    {
        printf("\n --------In match expected: %4s, found: %4s",
               tok2lex(t), tok2lex(lookahead));
    }

    if (lookahead == t)
        lookahead = get_token();

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
    if (DEBUG) printf("\n *** In  program_header");

    (lookahead == program)  ? match(program)    : error_symbol("program");
    if(lookahead == id){
        addp_name(get_lexeme());
        match(id); 
    } else{
        error_id();
        addp_name("???");
    }      
    (lookahead == '(')      ? match('(')        : error_symbol("(");
    (lookahead == input)    ? match(input)      : error_symbol("input");
    (lookahead == ',')      ? match(',')        : error_symbol(",");
    (lookahead == output)   ? match(output)     : error_symbol("output");
    (lookahead == ')')      ? match(')')        : error_symbol(")");
    (lookahead == ';')      ? match(';')        : error_symbol(";");

    if (DEBUG) printf("\n *** Out  program_header");
}

/*var_part*/

static void type()
{
    if (DEBUG) printf("\n *** In  type");

    if (lookahead == integer)
    {
        match(integer);
        setv_type(integer);
    }
    else if (lookahead == real)
    {
        match(real);
        setv_type(real);
    }
    else if (lookahead == boolean)
    {
        match(boolean);
        setv_type(boolean);
    }
    else
    {
        error_type();
        setv_type(error);
    }

    if (DEBUG) printf("\n *** Out  type");
}

static void id_list()
{
    if (DEBUG)
    {
        printf("\n *** In  id_list");
    }
    if (lookahead == id)
    {
        if(!find_name(get_lexeme())){
            addv_name(get_lexeme());
            
        }else{
            error_duplicate();
        }
        match(id);
    }
    else
    {
        error_id();
    }

    if (lookahead == ',')
    {
        match(',');
        id_list();
    }

    if (DEBUG) printf("\n *** Out  id_list");
}

static void var_dec()
{
    if (DEBUG) printf("\n *** In  var_dec");
    
    id_list();
    (lookahead==':')    ? match(':')    : error_symbol(":");
    type();
    (lookahead == ';')  ? match(';')    : error_symbol(";");
    
    if (DEBUG) printf("\n *** Out  var_dec");

}

static void var_dec_list()
{
    if (DEBUG) printf("\n *** In  var_dec_list");
    
    var_dec();
    if (lookahead == id) var_dec_list();

    if (DEBUG) printf("\n *** Out  var_dec_list");

}

static void var_part()
{
    if (DEBUG) printf("\n *** In  var_part");

    (lookahead == var)  ? match(var)    : error_symbol("var");
    var_dec_list();

    if (DEBUG) printf("\n *** Out  var_part");
}

/*stat_part*/

static toktyp operand()
{
    if (DEBUG) printf("\n *** In  operand");

    toktyp result;
    if (lookahead == id)
    {
        if(!find_name(get_lexeme())){
           error_undeclared();
        }
        result = get_ntype(get_lexeme());
        match(id);
        return result;
    }
    else if (lookahead == number)
    {
        match(number);
        result = integer;
        return result;
    }
    else
    {
        error_operand();
        return error;
    }

    if (DEBUG) printf("\n *** Out  operand");
}

static toktyp factor()
{
    if (DEBUG) printf("\n *** In  factor");

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

    if (DEBUG) printf("\n *** Out  factor");

}

static toktyp term()
{
    if (DEBUG) printf("\n *** In  term");

    int result = factor();
    if (lookahead == '*')
    {
        match('*');
        result = get_otype('*', result, term());
    }
    return result;

    if (DEBUG) printf("\n *** Out  term");
}

static toktyp expr()
{
    if (DEBUG) printf("\n *** In  expr");

    toktyp result = term();
    if (lookahead == '+')
    {
        match('+');
        result = get_otype('+', result, expr());
    }

    if (DEBUG) printf("\n *** Out  expr");
    
    return result;
}

static void assign_stat()
{

    if (DEBUG) printf("\n *** In  assign_stat");

    toktyp right;
    toktyp left = error;

    if(lookahead == id){
        if(!find_name(get_lexeme())){
            error_undeclared();
        }else{
            left=get_ntype(get_lexeme());
        }
        match(id);
    }else{
        error_id();
    }
    
    (lookahead == assign)   ? match(assign): error_symbol(":=");
    
    right = expr();
    if(left != right){
        error_assign(tok2lex(left), tok2lex(right));
    }

    if (DEBUG) printf("\n *** Out  assign_stat");
}

static void stat()
{
    if (DEBUG) printf("\n *** In  stat");

    assign_stat();

    if (DEBUG) printf("\n *** Out  stat");
}

static void stat_list()
{
    if (DEBUG) printf("\n *** In  stat_list");

    stat();

    if (lookahead == ';')
    {
        match(';');
        stat_list();
    }

    if (DEBUG) printf("\n *** Out  stat_list");

}

static void stat_part()
{
    if (DEBUG) printf("\n *** In  stat_part");

    (lookahead == begin)    ? match(begin)  : error_symbol("begin");
    stat_list();
    (lookahead == end)      ? match(end)    : error_symbol("end");
    (lookahead == '.')      ? match('.')    : error_symbol(".");
    if (lookahead!='$') {
        error_symbol_after_end();
        printf("          ");
        while(lookahead!='$'){
            printf("%s ",get_lexeme(lookahead));
            match(lookahead);
        }
        printf("\r");
        
    } 

    if (DEBUG) printf("\n *** Out  stat_part");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    if (DEBUG) printf("\n *** In  parser");

    lookahead = get_token(); // get the first token
    if(lookahead < 0 || lookahead == '$'){
        error_empty_file();
    }else{
        program_header();        // call the first grammar rule
        var_part();
        stat_part();
        success();
        
    }
   
    p_symtab();
    
    if (DEBUG) printf("\n *** Out  parser");

    return is_parse_ok; // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
