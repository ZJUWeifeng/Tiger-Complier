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

extern bool fatalErrors;
extern char replaceMsg[1024];
int main(int argc, string *argv)
{
	A_exp absyn_root;
	S_table base_env, base_tenv;
	char outfile[100];
	FILE *out = stdout;

	if (argc == 2) {
		absyn_root = parse(argv[1]);		// 语法分析
		if (!absyn_root) {
			system("pause");
			return 1;
		}
		if (replaceMsg[0] != '\0') {
			fprintf(out, "%s", replaceMsg);
		}

		pr_exp(out, absyn_root, 0);			// 输出抽象语法树
		fprintf(out, "\n");

		SEM_transProg(absyn_root);			// 语义分析
		if (fatalErrors) {					// 如果出现错误即退出
			system("pause");
			return 1;
		}

		system("pause");
		return 0;
	}
	EM_error(0, "参数数量错误，请输入待编译的Tiger源文件路径");
	system("pause");
	return 1;
}
