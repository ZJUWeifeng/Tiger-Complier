/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"
#include "reserved.h"

bool fatalErrors = FALSE;
bool parseErrors = FALSE;

static string fileName = "";

int lineNum = 1;

int EM_tokPos = 0;
int EM_errorPos = 0;

extern FILE *yyin;
extern Nearest_List L_nearest;
extern char replaceMsg[128];

static IntList intList(int i, IntList rest)
{
	IntList l = checked_malloc(sizeof *l);
	l->i = i; l->rest = rest;
	return l;
}

IntList linePos = NULL;

void EM_newline(void)
{
	lineNum++;
	linePos = intList(EM_tokPos, linePos);
}

void EM_parse_error(int pos, char* message) {
	static int error_count = 0;
	static Nearest_List head = NULL;
	static char* error;
	int i = 0;
	Nearest_List tempHead = NULL;

	parseErrors = TRUE;
	//��һ�ν��뺯�������ú�����ͷ����λ�����ܳ��ִ����idλ��
	if (error_count == 0) {
		head = L_nearest;
		while (head->tokpos > pos) {
			head = head->next;
		}
		EM_errorPos = head->tokpos;
	}
	//���ǵ�һ�ν��뺯����Ӧ��˳�����������ǰѰ�ҿ��ܳ��ִ����idλ��
	tempHead = head;
	while (tempHead->next && i < error_count) {
		tempHead = tempHead->next;
		i++;
		EM_errorPos = tempHead->tokpos;
	}
	error_count++;
	//����׷�ݵ�����λ��ǰ�ĵ�3��idλ��
	if (!tempHead || error_count == 3) {
		EM_error(pos, message);
	}
}

void EM_error(int pos, char *message, ...)
{
	va_list ap;
	IntList lines = linePos;
	int num = lineNum;


	fatalErrors = TRUE;
	while (lines && lines->i >= pos)
	{
		lines = lines->rest; num--;
	}

	if (fileName) fprintf(stderr, "%s:", fileName);
	if (lines) fprintf(stderr, "%d.%d: ", num, pos - lines->i);
	va_start(ap, message);
	vfprintf(stderr, message, ap);
	va_end(ap);
	fprintf(stderr, "\n");

}

void EM_reset(string fname)
{
	fatalErrors = FALSE; fileName = fname; lineNum = 1;
	EM_tokPos = 0;
	charPos = 1;
	L_nearest = NULL;
	linePos = intList(0, NULL);
	sprintf(replaceMsg, "");
	FILE* fp = fopen(fname, "r");
	if (!fp) {
		EM_error(0, "can't open input file.\n");
	}
	yyrestart(fp);
}

