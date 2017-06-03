%{
#include <string.h>
#include "util.h"
#include "absyn.h"
#include "tiger.tab.h"
#include "errormsg.h"
#include "reserved.h"

int charPos=1;
int commentDepth=0;

/* String handling variables/functions */
#define BUFSIZE 8192
#define RESERVEDSIZE 17

char strbuf[BUFSIZE+1];
char *strptr = NULL;
unsigned int strlength = 0;

char replaceMsg[128] = "";
extern bool parseErrors;
extern int EM_errorPos;
extern IntList linePos;
extern int lineNum;

Reserved reserved[RESERVEDSIZE] = {
    { "while", WHILE}, {"for", FOR}, {"to", TO}, {"break", BREAK}, {"let", LET},
	{"in", IN}, {"end", END}, {"function", FUNCTION}, {"var", VAR}, {"type", TYPE},
	{"array", ARRAY}, {"if", IF}, {"then", THEN}, {"else", ELSE},{"do", DO},
	{"of", OF}, {"nil", NIL}
};
Nearest_List L_nearest = NULL;

void setup(void)
{
	*strbuf = '\0';
	strlength = 0;
}

char *teardown()
{
	char *s = checked_malloc(strlen(strbuf)+1);
	strcpy(s, strbuf);
	return s;
}

void appendstr(char *str)
{
	if ((strlength + strlen(str)) < BUFSIZE) {
		strcat(strbuf, str);
		strlength += strlen(str);
	}
}

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}
%x COMMENT STR
%%
[ \t]	{adjust(); continue;}
(\n|\r\n)  {adjust(); EM_newline(); continue;}
"*"   {adjust(); return TIMES;}
"/"   {adjust(); return DIVIDE;}
"/*"  {adjust(); BEGIN(COMMENT); commentDepth++;}
<COMMENT>{
	"/*" {adjust(); commentDepth++;}
	"*/" {adjust(); if (--commentDepth == 0) BEGIN(INITIAL);}
	[^\n] {adjust();}
	(\n|\r\n)	{adjust(); EM_newline();}
}
"while" {adjust(); return WHILE;}
"for"   {adjust(); return FOR;}
"to"	{adjust(); return TO;}
"break" {adjust(); return BREAK;}
"let"	 {adjust(); return LET;}
"in"  {adjust(); return IN;}
"end"   {adjust(); return END;}
"function" {adjust(); return FUNCTION;}
"var"   {adjust(); return VAR;}
"type"  {adjust(); return TYPE;}
"array" {adjust(); return ARRAY;}
"if"	  {adjust(); return IF;}
"then"  {adjust(); return THEN;}
"else"  {adjust(); return ELSE;}
"do"	  {adjust(); return DO;}
"of"	  {adjust(); return OF;}
"nil"	  {adjust(); return NIL;}
[a-zA-Z][a-zA-Z0-9_]*    {adjust();
                          yylval.sval=yytext; 
						  int index = RE_find_nearest(yylval.sval, reserved, RESERVEDSIZE);
						  L_nearest = nearest_list(EM_tokPos, reserved[index].token, yylval.sval, L_nearest);

                          if(parseErrors && EM_tokPos == EM_errorPos){
						      IntList lines = linePos; 
                              int num = lineNum;
						      yylval.sval = reserved[index].name;
							  parseErrors = FALSE;
                              while (lines && lines->i >= EM_tokPos) 
                                  {lines=lines->rest; num--;}
                              if (lines)
							      sprintf(replaceMsg, "Find syntax error at line:%d, pos:%d.\nThe compiler has replaced \"%s\" with \"%s\" to get rid of it.\n", num, EM_tokPos-lines->i, yytext, yylval.sval);
							  return reserved[index].token;
						  }
						  else{
							  return ID;
						  }
						  }
","	  {adjust(); return COMMA;}
":"	  {adjust(); return COLON;}
";"	  {adjust(); return SEMICOLON;}
"("	  {adjust(); return LPAREN;}
")"    {adjust(); return RPAREN;}
"["    {adjust(); return LBRACK;}
"]"   {adjust(); return RBRACK;}
"{"   {adjust(); return LBRACE;}
"}"   {adjust(); return RBRACE;}
"."   {adjust(); return DOT;}
"+"   {adjust(); return PLUS;}
"-"   {adjust(); return MINUS;}
"="   {adjust(); return EQ;}
"<>"  {adjust(); return NEQ;}
"<"   {adjust(); return LT;}
"<="  {adjust(); return LE;}
">"   {adjust(); return GT;}
">="  {adjust(); return GE;}
":="  {adjust(); return ASSIGN;}
"&"	  {adjust(); return AND;}
"|"	  {adjust(); return OR;}
[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}

\" {adjust(); BEGIN(STR); setup();}
<STR>{
	\" 			{adjust(); yylval.sval=teardown(); BEGIN(INITIAL); return STRING;}
	\\n			{adjust(); appendstr("\n");}
	\\t			{adjust(); appendstr("\t");}
	\\[0-9]{3}	{adjust(); appendstr(yytext);}
	\\^[GHIJLM]	{adjust(); appendstr(yytext);}
	\\\\		{adjust(); appendstr(yytext);}
	\\\"		{adjust(); appendstr(yytext);}
	\\[ \n\t\r\f]+\\ {adjust();}
	\\(.|\n)	{adjust(); EM_error(EM_tokPos, "illegal token");}
	\n			{adjust(); EM_error(EM_tokPos, "illegal token");}
	[^\"\\\n]+ 	{adjust(); appendstr(yytext);}
}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}
%%

