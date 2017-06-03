
/*  A Bison parser, made from tiger.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ID	258
#define	STRING	259
#define	INT	260
#define	COMMA	261
#define	COLON	262
#define	SEMICOLON	263
#define	LPAREN	264
#define	RPAREN	265
#define	LBRACK	266
#define	RBRACK	267
#define	LBRACE	268
#define	RBRACE	269
#define	DOT	270
#define	PLUS	271
#define	MINUS	272
#define	TIMES	273
#define	DIVIDE	274
#define	EQ	275
#define	NEQ	276
#define	LT	277
#define	LE	278
#define	GT	279
#define	GE	280
#define	AND	281
#define	OR	282
#define	ASSIGN	283
#define	ARRAY	284
#define	IF	285
#define	THEN	286
#define	ELSE	287
#define	WHILE	288
#define	FOR	289
#define	TO	290
#define	DO	291
#define	LET	292
#define	IN	293
#define	END	294
#define	OF	295
#define	BREAK	296
#define	NIL	297
#define	FUNCTION	298
#define	VAR	299
#define	TYPE	300
#define	SWITCH	301
#define	CASE	302
#define	LOW	303
#define	UMINUS	304

#line 1 "tiger.y"

#include <stdio.h>
#include "util.h"
#include "absyn.h"
#include "errormsg.h"

int yylex(void); /* function prototype */

void yyerror(char *s)
{
  EM_parse_error(EM_tokPos, "%s", s);
}

#line 16 "tiger.y"
typedef union {
	int pos;
	int ival;
	string sval;
	S_symbol sym;
	A_var var;
	A_exp exp;
	A_dec dec;
	A_ty ty;
	A_decList decList;
	A_expList expList;
	A_field field;
	A_fieldList fieldList;
	A_fundec fundec;
	A_fundecList fundecList;
	A_namety namety;
	A_nametyList nametyList;
	A_efield efield;
	A_efieldList efieldList;
	A_caselist caselist;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		152
#define	YYFLAG		-32768
#define	YYNTBASE	50

#define YYTRANSLATE(x) ((unsigned)(x) <= 304 ? yytranslate[x] : 74)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     5,     7,     9,    11,    16,    18,    22,
    24,    26,    30,    34,    38,    42,    46,    50,    54,    58,
    62,    66,    70,    74,    81,    86,    90,    93,    95,    96,
    98,   102,   108,   114,   115,   117,   119,   123,   125,   130,
   135,   139,   140,   142,   146,   153,   158,   163,   172,   176,
   177,   182,   183,   186,   188,   190,   192,   194,   197,   202,
   204,   208,   212,   213,   215,   217,   221,   225,   230,   237,
   239,   241,   244,   252
};

static const short yyrhs[] = {    51,
     0,     0,    52,     0,     5,     0,     4,     0,    42,     0,
    71,     9,    56,    10,     0,    58,     0,     9,    59,    10,
     0,    60,     0,    55,     0,    52,    27,    52,     0,    52,
    26,    52,     0,    52,    22,    52,     0,    52,    24,    52,
     0,    52,    23,    52,     0,    52,    25,    52,     0,    52,
    16,    52,     0,    52,    17,    52,     0,    52,    18,    52,
     0,    52,    19,    52,     0,    52,    20,    52,     0,    52,
    21,    52,     0,    71,    11,    52,    12,    40,    52,     0,
    71,    13,    53,    14,     0,    58,    28,    52,     0,    17,
    52,     0,    41,     0,     0,    54,     0,    71,    20,    52,
     0,    71,    20,    52,     6,    54,     0,    37,    62,    38,
    59,    39,     0,     0,    57,     0,    52,     0,    52,     6,
    57,     0,    71,     0,    71,    11,    52,    12,     0,    58,
    11,    52,    12,     0,    58,    15,    71,     0,     0,    52,
     0,    52,     8,    59,     0,    30,    52,    31,    52,    32,
    52,     0,    30,    52,    31,    52,     0,    33,    52,    36,
    52,     0,    34,    71,    28,    52,    35,    52,    36,    52,
     0,    46,    52,    61,     0,     0,    47,    52,    52,    61,
     0,     0,    63,    62,     0,    64,     0,    70,     0,    72,
     0,    65,     0,    65,    64,     0,    45,    71,    20,    66,
     0,    71,     0,    13,    67,    14,     0,    29,    40,    71,
     0,     0,    68,     0,    69,     0,    69,     6,    68,     0,
    71,     7,    71,     0,    44,    71,    28,    52,     0,    44,
    71,     7,    71,    28,    52,     0,     3,     0,    73,     0,
    73,    72,     0,    43,    71,     9,    67,    10,    20,    52,
     0,    43,    71,     9,    67,    10,     7,    71,    20,    52,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    77,    80,    81,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,   102,   103,   104,   105,   106,   107,   108,   111,   112,
   115,   116,   118,   122,   123,   126,   127,   130,   131,   133,
   134,   137,   138,   139,   143,   144,   145,   146,   147,   150,
   151,   154,   155,   158,   159,   160,   163,   165,   169,   172,
   173,   174,   177,   178,   181,   182,   185,   188,   189,   192,
   195,   196,   199,   201
};

static const char * const yytname[] = {   "$","error","$undefined.","ID","STRING",
"INT","COMMA","COLON","SEMICOLON","LPAREN","RPAREN","LBRACK","RBRACK","LBRACE",
"RBRACE","DOT","PLUS","MINUS","TIMES","DIVIDE","EQ","NEQ","LT","LE","GT","GE",
"AND","OR","ASSIGN","ARRAY","IF","THEN","ELSE","WHILE","FOR","TO","DO","LET",
"IN","END","OF","BREAK","NIL","FUNCTION","VAR","TYPE","SWITCH","CASE","LOW",
"UMINUS","program","primyexp","exp","reclist","nonreclist","letexp","arglist",
"nonarglist","lvalue","expseq","condexp","caselist","decs","dec","tydecs","tydec",
"ty","tyfields","tyfields1","tyfield","vardec","id","fundecs","fundec",""
};
#endif

static const short yyr1[] = {     0,
    50,    51,    51,    52,    52,    52,    52,    52,    52,    52,
    52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
    52,    52,    52,    52,    52,    52,    52,    52,    53,    53,
    54,    54,    55,    56,    56,    57,    57,    58,    58,    58,
    58,    59,    59,    59,    60,    60,    60,    60,    60,    61,
    61,    62,    62,    63,    63,    63,    64,    64,    65,    66,
    66,    66,    67,    67,    68,    68,    69,    70,    70,    71,
    72,    72,    73,    73
};

static const short yyr2[] = {     0,
     1,     0,     1,     1,     1,     1,     4,     1,     3,     1,
     1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     6,     4,     3,     2,     1,     0,     1,
     3,     5,     5,     0,     1,     1,     3,     1,     4,     4,
     3,     0,     1,     3,     6,     4,     4,     8,     3,     0,
     4,     0,     2,     1,     1,     1,     1,     2,     4,     1,
     3,     3,     0,     1,     1,     3,     3,     4,     6,     1,
     1,     2,     7,     9
};

static const short yydefact[] = {     2,
    70,     5,     4,    42,     0,     0,     0,     0,    52,    28,
     6,     0,     1,     3,    11,     8,    10,    38,    43,     0,
    27,     0,     0,     0,     0,     0,     0,     0,    52,    54,
    57,    55,    56,    71,    50,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    34,     0,    29,    42,     9,     0,     0,     0,     0,     0,
     0,    42,    53,    58,    72,     0,    49,    18,    19,    20,
    21,    22,    23,    14,    16,    15,    17,    13,    12,     0,
    41,    26,    36,     0,    35,     0,     0,    30,     0,    44,
    46,    47,     0,    63,     0,     0,     0,     0,     0,    40,
     0,     7,    39,    25,     0,     0,     0,     0,    64,    65,
     0,     0,    68,    63,     0,    59,    60,    33,     0,    50,
    37,     0,    31,    45,     0,     0,     0,     0,     0,     0,
     0,    19,    51,    24,     0,     0,     0,     0,    66,    67,
    69,    61,    62,    32,    48,     0,    73,     0,    74,     0,
     0,     0
};

static const short yydefgoto[] = {   150,
    13,    19,    87,    88,    15,    84,    85,    16,    20,    17,
    67,    28,    29,    30,    31,   116,   108,   109,   110,    32,
    18,    33,    34
};

static const short yypact[] = {   224,
-32768,-32768,-32768,   224,   224,   224,   224,     5,   -11,-32768,
-32768,   224,-32768,   390,-32768,    74,-32768,    51,   297,    -7,
-32768,   346,   255,   -17,     5,     5,     5,   -25,   -11,-32768,
   -10,-32768,-32768,    11,     2,   224,   224,   224,   224,   224,
   224,   224,   224,   224,   224,   224,   224,   224,     5,   224,
   224,   224,     5,   224,-32768,   224,   224,   224,    46,     3,
    39,   224,-32768,-32768,-32768,   224,-32768,    -2,    -2,-32768,
-32768,   402,   402,   402,   402,   402,   402,   412,    92,   362,
-32768,   390,    57,    55,-32768,   378,    47,-32768,    48,-32768,
   329,   390,   309,     5,     5,   224,     1,    28,   189,-32768,
   224,-32768,    29,-32768,   224,   224,   224,    76,-32768,    81,
    83,    60,   390,     5,    52,-32768,-32768,-32768,   224,     2,
-32768,   224,   226,   390,   276,    -5,     5,     5,   224,    77,
     5,   152,-32768,   390,     5,   224,     5,   224,-32768,-32768,
   390,-32768,-32768,-32768,   390,    73,   390,   224,   390,    95,
    97,-32768
};

static const short yypgoto[] = {-32768,
-32768,     0,-32768,   -35,-32768,-32768,    19,-32768,   -53,-32768,
   -16,    94,-32768,    72,-32768,-32768,     7,    -3,-32768,-32768,
    45,    91,-32768
};


#define	YYLAST		437


static const short yytable[] = {    14,
    90,   137,    55,     1,    21,    22,    23,     1,    98,    95,
    58,    35,    62,   114,   138,    38,    39,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,   115,
    96,    25,    26,    27,    27,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    66,    82,
    83,    86,    24,    25,    94,    91,    92,    93,    97,    51,
   104,    52,   101,    53,   102,    99,   118,   105,   122,    59,
    60,    61,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,   126,   127,   129,    49,   128,
   142,   131,   148,    81,   151,   113,   152,    89,   120,   144,
    83,    50,    64,   133,   123,   124,   125,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,   132,   121,
   130,   134,    63,   139,    65,     0,     0,     0,   141,     0,
     0,     0,     0,     0,     0,   145,     0,   147,   111,   112,
     0,   117,     0,     0,     0,     0,     0,   149,     0,     0,
     0,   -27,     0,     0,     0,     0,     0,   -27,   111,   -27,
     0,   -27,     0,   -27,     0,   -27,     0,     0,     0,   -27,
   -27,   111,   140,     0,     0,   143,     0,     0,     0,    89,
     0,   146,   -27,   -27,     0,     0,   -27,   -27,     0,   -27,
   -27,     1,     2,     3,   -27,   -27,   -27,     4,   -27,     0,
     0,     0,     0,     0,    36,   119,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,     0,     0,     6,     0,
     0,     7,     8,     0,     0,     9,     1,     2,     3,    10,
    11,   135,     4,     0,    12,     0,     0,     0,     0,     0,
     5,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,     6,     0,     0,     7,     8,     0,     0,
     9,     0,     0,     0,    10,    11,     0,     0,     0,    12,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
    57,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,     0,    54,     0,     0,     0,     0,     0,
     0,   136,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,     0,     0,     0,     0,
     0,     0,     0,   107,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,     0,     0,     0,     0,
   106,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,   100,     0,     0,    56,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,   103,
     0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    36,    37,    38,
    39,-32768,-32768,-32768,-32768,-32768,-32768,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45
};

static const short yycheck[] = {     0,
    54,     7,    10,     3,     5,     6,     7,     3,    62,     7,
    28,    12,    38,    13,    20,    18,    19,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    29,
    28,    43,    44,    45,    45,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    48,    47,    50,
    51,    52,     8,    43,     9,    56,    57,    58,    20,     9,
    14,    11,     6,    13,    10,    66,    39,    20,    40,    25,
    26,    27,    16,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    11,    10,     6,    28,    15,     7,
    14,    40,    20,    49,     0,    96,     0,    53,    99,   135,
   101,    28,    31,   120,   105,   106,   107,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,   119,   101,
   114,   122,    29,   127,    34,    -1,    -1,    -1,   129,    -1,
    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    94,    95,
    -1,    97,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
    -1,     0,    -1,    -1,    -1,    -1,    -1,     6,   114,     8,
    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,    18,
    19,   127,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,
    -1,   137,    31,    32,    -1,    -1,    35,    36,    -1,    38,
    39,     3,     4,     5,    43,    44,    45,     9,    47,    -1,
    -1,    -1,    -1,    -1,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    -1,    -1,    30,    -1,
    -1,    33,    34,    -1,    -1,    37,     3,     4,     5,    41,
    42,     6,     9,    -1,    46,    -1,    -1,    -1,    -1,    -1,
    17,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    30,    -1,    -1,    33,    34,    -1,    -1,
    37,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    46,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    36,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    -1,     8,    -1,    -1,    -1,    -1,    -1,
    -1,    36,    16,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    35,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,
    32,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    12,    -1,    -1,    31,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    12,
    -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */
#include <malloc.h>      /*  add by lyh */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) malloc (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) malloc (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) malloc (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 77 "tiger.y"
{absyn_root=yyvsp[0].exp;;
    break;}
case 2:
#line 80 "tiger.y"
{yyval.exp=NULL;;
    break;}
case 3:
#line 81 "tiger.y"
{yyval.exp=yyvsp[0].exp;;
    break;}
case 4:
#line 84 "tiger.y"
{yyval.exp=A_IntExp(EM_tokPos, yyvsp[0].ival);;
    break;}
case 5:
#line 85 "tiger.y"
{yyval.exp=A_StringExp(EM_tokPos, yyvsp[0].sval);;
    break;}
case 6:
#line 86 "tiger.y"
{yyval.exp=A_NilExp(EM_tokPos);;
    break;}
case 7:
#line 87 "tiger.y"
{yyval.exp=A_CallExp(EM_tokPos, yyvsp[-3].sym, yyvsp[-1].expList);;
    break;}
case 8:
#line 88 "tiger.y"
{yyval.exp=A_VarExp(EM_tokPos, yyvsp[0].var);;
    break;}
case 9:
#line 89 "tiger.y"
{yyval.exp=A_SeqExp(EM_tokPos, yyvsp[-1].expList);;
    break;}
case 10:
#line 90 "tiger.y"
{yyval.exp=yyvsp[0].exp;;
    break;}
case 11:
#line 91 "tiger.y"
{yyval.exp=yyvsp[0].exp;;
    break;}
case 12:
#line 92 "tiger.y"
{yyval.exp=A_IfExp(EM_tokPos, yyvsp[-2].exp, A_IntExp(EM_tokPos,1), yyvsp[0].exp);;
    break;}
case 13:
#line 93 "tiger.y"
{yyval.exp=A_IfExp(EM_tokPos, yyvsp[-2].exp, yyvsp[0].exp, A_IntExp(EM_tokPos,0));;
    break;}
case 14:
#line 94 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_ltOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 15:
#line 95 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_gtOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 16:
#line 96 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_leOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 17:
#line 97 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_geOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 18:
#line 98 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_plusOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 19:
#line 99 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_minusOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 20:
#line 100 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_timesOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 21:
#line 101 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_divideOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 22:
#line 102 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_eqOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 23:
#line 103 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_neqOp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 24:
#line 104 "tiger.y"
{yyval.exp=A_ArrayExp(EM_tokPos, yyvsp[-5].sym, yyvsp[-3].exp, yyvsp[0].exp);;
    break;}
case 25:
#line 105 "tiger.y"
{yyval.exp=A_RecordExp(EM_tokPos, yyvsp[-3].sym, yyvsp[-1].efieldList);;
    break;}
case 26:
#line 106 "tiger.y"
{yyval.exp=A_AssignExp(EM_tokPos, yyvsp[-2].var, yyvsp[0].exp);;
    break;}
case 27:
#line 107 "tiger.y"
{yyval.exp=A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), yyvsp[0].exp);;
    break;}
case 28:
#line 108 "tiger.y"
{yyval.exp=A_BreakExp(EM_tokPos);;
    break;}
case 29:
#line 111 "tiger.y"
{yyval.efieldList=NULL;;
    break;}
case 30:
#line 112 "tiger.y"
{yyval.efieldList=yyvsp[0].efieldList;;
    break;}
case 31:
#line 115 "tiger.y"
{yyval.efieldList=A_EfieldList(A_Efield(yyvsp[-2].sym, yyvsp[0].exp), NULL);;
    break;}
case 32:
#line 116 "tiger.y"
{yyval.efieldList=A_EfieldList(A_Efield(yyvsp[-4].sym, yyvsp[-2].exp), yyvsp[0].efieldList);;
    break;}
case 33:
#line 118 "tiger.y"
{yyval.exp=A_LetExp(EM_tokPos, yyvsp[-3].decList, 
														A_SeqExp(EM_tokPos, yyvsp[-1].expList));;
    break;}
case 34:
#line 122 "tiger.y"
{yyval.expList=NULL;;
    break;}
case 35:
#line 123 "tiger.y"
{yyval.expList=yyvsp[0].expList;;
    break;}
case 36:
#line 126 "tiger.y"
{yyval.expList=A_ExpList(yyvsp[0].exp, NULL);;
    break;}
case 37:
#line 127 "tiger.y"
{yyval.expList=A_ExpList(yyvsp[-2].exp, yyvsp[0].expList);;
    break;}
case 38:
#line 130 "tiger.y"
{yyval.var=A_SimpleVar(EM_tokPos, yyvsp[0].sym);;
    break;}
case 39:
#line 131 "tiger.y"
{yyval.var=A_SubscriptVar(EM_tokPos, 
														A_SimpleVar(EM_tokPos, yyvsp[-3].sym), yyvsp[-1].exp);;
    break;}
case 40:
#line 133 "tiger.y"
{yyval.var=A_SubscriptVar(EM_tokPos, yyvsp[-3].var, yyvsp[-1].exp);;
    break;}
case 41:
#line 134 "tiger.y"
{yyval.var=A_FieldVar(EM_tokPos, yyvsp[-2].var, yyvsp[0].sym);;
    break;}
case 42:
#line 137 "tiger.y"
{yyval.expList=NULL;;
    break;}
case 43:
#line 138 "tiger.y"
{yyval.expList=A_ExpList(yyvsp[0].exp, NULL);;
    break;}
case 44:
#line 139 "tiger.y"
{yyval.expList=A_ExpList(yyvsp[-2].exp, yyvsp[0].expList);;
    break;}
case 45:
#line 143 "tiger.y"
{yyval.exp=A_IfExp(EM_tokPos, yyvsp[-4].exp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 46:
#line 144 "tiger.y"
{yyval.exp=A_IfExp(EM_tokPos, yyvsp[-2].exp, yyvsp[0].exp, NULL);;
    break;}
case 47:
#line 145 "tiger.y"
{yyval.exp=A_WhileExp(EM_tokPos, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 48:
#line 146 "tiger.y"
{yyval.exp=A_ForExp(EM_tokPos, yyvsp[-6].sym, yyvsp[-4].exp, yyvsp[-2].exp, yyvsp[0].exp);;
    break;}
case 49:
#line 147 "tiger.y"
{yyval.exp=A_SwitchExp(EM_tokPos, yyvsp[-1].exp, yyvsp[0].caselist);;
    break;}
case 50:
#line 150 "tiger.y"
{yyval.caselist=NULL;;
    break;}
case 51:
#line 151 "tiger.y"
{yyval.caselist=A_CaseList(EM_tokPos, yyvsp[-2].exp, yyvsp[-1].exp, yyvsp[0].caselist);;
    break;}
case 52:
#line 154 "tiger.y"
{yyval.decList=NULL;;
    break;}
case 53:
#line 155 "tiger.y"
{yyval.decList=A_DecList(yyvsp[-1].dec, yyvsp[0].decList);;
    break;}
case 54:
#line 158 "tiger.y"
{yyval.dec=yyvsp[0].dec;;
    break;}
case 55:
#line 159 "tiger.y"
{yyval.dec=yyvsp[0].dec;;
    break;}
case 56:
#line 160 "tiger.y"
{yyval.dec=yyvsp[0].dec;;
    break;}
case 57:
#line 163 "tiger.y"
{yyval.dec=A_TypeDec(EM_tokPos, 
													A_NametyList(yyvsp[0].namety, NULL));;
    break;}
case 58:
#line 165 "tiger.y"
{yyval.dec=A_TypeDec(EM_tokPos, 
													A_NametyList(yyvsp[-1].namety, yyvsp[0].dec->u.type));;
    break;}
case 59:
#line 169 "tiger.y"
{yyval.namety=A_Namety(yyvsp[-2].sym, yyvsp[0].ty);;
    break;}
case 60:
#line 172 "tiger.y"
{yyval.ty=A_NameTy(EM_tokPos, yyvsp[0].sym);;
    break;}
case 61:
#line 173 "tiger.y"
{yyval.ty=A_RecordTy(EM_tokPos, yyvsp[-1].fieldList);;
    break;}
case 62:
#line 174 "tiger.y"
{yyval.ty=A_ArrayTy(EM_tokPos, yyvsp[0].sym);;
    break;}
case 63:
#line 177 "tiger.y"
{yyval.fieldList=NULL;;
    break;}
case 64:
#line 178 "tiger.y"
{yyval.fieldList=yyvsp[0].fieldList;;
    break;}
case 65:
#line 181 "tiger.y"
{yyval.fieldList=A_FieldList(yyvsp[0].field, NULL);;
    break;}
case 66:
#line 182 "tiger.y"
{yyval.fieldList=A_FieldList(yyvsp[-2].field, yyvsp[0].fieldList);;
    break;}
case 67:
#line 185 "tiger.y"
{yyval.field=A_Field(EM_tokPos, yyvsp[-2].sym, yyvsp[0].sym);;
    break;}
case 68:
#line 188 "tiger.y"
{yyval.dec=A_VarDec(EM_tokPos, yyvsp[-2].sym, NULL, yyvsp[0].exp);;
    break;}
case 69:
#line 189 "tiger.y"
{yyval.dec=A_VarDec(EM_tokPos, yyvsp[-4].sym, yyvsp[-2].sym, yyvsp[0].exp);;
    break;}
case 70:
#line 192 "tiger.y"
{yyval.sym=S_Symbol(yyvsp[0].sval);;
    break;}
case 71:
#line 195 "tiger.y"
{yyval.dec=A_FunctionDec(EM_tokPos, A_FundecList(yyvsp[0].fundec, NULL));;
    break;}
case 72:
#line 196 "tiger.y"
{yyval.dec=A_FunctionDec(EM_tokPos, A_FundecList(yyvsp[-1].fundec, yyvsp[0].dec->u.function));;
    break;}
case 73:
#line 199 "tiger.y"
{yyval.fundec=A_Fundec(EM_tokPos, 
																			yyvsp[-5].sym, yyvsp[-3].fieldList, NULL, yyvsp[0].exp);;
    break;}
case 74:
#line 201 "tiger.y"
{yyval.fundec=A_Fundec(EM_tokPos, 
																			yyvsp[-7].sym, yyvsp[-5].fieldList, yyvsp[-2].sym, yyvsp[0].exp);;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 204 "tiger.y"
