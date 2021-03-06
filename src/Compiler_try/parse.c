/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"

extern int yyparse(void);
extern bool fatalErrors;

/* parse source file fname; 
   return abstract syntax data structure */
A_exp parse(string fname)
{
	while (!fatalErrors) {
		EM_reset(fname);
		if (yyparse() == 0)	/* parsing worked */
			return absyn_root;
	}
	return NULL;
}
