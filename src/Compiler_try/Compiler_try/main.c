/*
 * main.c
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "temp.h" /* needed by translate.h */
#include "tree.h" /* needed by frame.h */
#include "frame.h" /* needed by translate.h and printfrags prototype */
#include "semant.h" /* function prototype for transProg */
#include "prabsyn.h"
#include "printtree.h"
#include "parse.h"

extern bool anyErrors;

int main(int argc, string *argv)
{
	A_exp absyn_root;
	S_table base_env, base_tenv;
	F_fragList frags;
	char outfile[100];
	FILE *out = stdout;

	if (argc == 2) {
		absyn_root = parse(argv[1]);		// 语法分析
		if (!absyn_root)
			return 1;

		pr_exp(out, absyn_root, 0);			// 输出抽象语法树
		fprintf(out, "\n");

		frags = SEM_transProg(absyn_root);	// 语义分析
		if (anyErrors) return 1;			// 如果出现错误即退出

		system("pause");
		return 0;
	}
	else if (argc == 1) {
		// let var a = 1;
		// in a = 2;
		absyn_root = A_SeqExp(0,
			A_ExpList(
				A_LetExp(1,
					A_DecList(
						A_VarDec(2,
							S_Symbol("a"),
							S_Symbol("int"),
							A_IntExp(3, 1)),
						NULL),
					A_AssignExp(4,
						A_SimpleVar(5,
							S_Symbol("a")),
						A_IntExp(6, 2))
					),
				NULL));
		frags = SEM_transProg(absyn_root);	// 语义分析
		if (anyErrors) return 1;			// 如果出现错误即退出
		system("pause");
		return 0;
	}
	EM_error(0, "usage: tiger file.tig");
	system("pause");
	return 1;
}
