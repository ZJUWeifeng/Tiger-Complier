#include "semant.h"
#include "util.h"
#include "errormsg.h"
#include "env.h"
#include "printtree.h"
#include <assert.h>
#include <stdlib.h>

/* prototypes for functions local to this module */
static struct expty transExp(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
static struct expty transVar(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v);
static Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d);
static Ty_ty transTy(S_table tenv, A_ty t);
static struct expty expTy(Tr_exp exp, Ty_ty ty);
static Ty_ty actual_ty(Ty_ty ty);
static Ty_ty S_look_ty(S_table tenv, S_symbol sym);
static int is_equal_ty(Ty_ty tType, Ty_ty eType);

struct expty transExp_var(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_nil();
struct expty transExp_int(int intt);
struct expty transExp_string(string stringg);
struct expty transExp_call(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_op(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_record(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_seq(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_assign(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_if(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_while(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_for(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_break(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_let(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transExp_array(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);

struct expty transVar_simple(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v);
struct expty transVar_field(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v);
struct expty transVar_subscript(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v);

Tr_exp transDec_function(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d);
Tr_exp transDec_var(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d);
Tr_exp transDec_type(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d);

Ty_ty transTy_name(S_table tenv, A_ty t);
Ty_ty transTy_record(S_table tenv, A_ty t);
Ty_ty transTy_array(S_table tenv, A_ty t);

/* expty constructor */
static struct expty expTy(Tr_exp exp, Ty_ty ty)
{
	struct expty e;
	e.exp = exp; e.ty = ty;
	return e;
}

static Ty_ty actual_ty(Ty_ty ty)
{
	if (ty->kind == Ty_name)
		return actual_ty(ty->u.name.ty);
	else
		return ty;
}

static Ty_ty S_look_ty(S_table tenv, S_symbol sym)
{
	Ty_ty t = S_look(tenv, sym);
	if (t)
		return actual_ty(t);
	else
		return NULL;
}

/*
 * Compares two type kinds and returns 1 if they are the same
 * or 0 otherwise.
 * The first argument is the type of the variable, etc.
 * The second argument is the type of the initialising expression.
 * This also handles the record and nil expression constraint.
 */
static int is_equal_ty(Ty_ty tType, Ty_ty eType)		// �Ƚ������Ƿ���ͬ
{
	Ty_ty actualtType = actual_ty(tType);
	int tyKind = actualtType->kind;
	int eKind = eType->kind;
	return ( ((tyKind == Ty_record || tyKind == Ty_array) && actualtType == eType) ||
		(tyKind == Ty_record && eKind == Ty_nil) ||
		(tyKind != Ty_record && tyKind != Ty_array && tyKind == eKind) );
}

void SEM_transProg(A_exp root)					// semant.c�����, �������ʼ��ֵ�ռ�����Ϳռ�, ����ʼ���������﷨���ĸ�A_exp exp
{
	/* Set up the type and value environments */
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
	FILE* out = fopen("IR.txt","w");
	FILE* json_out = fopen("display/js/json_IR.js", "w");
	struct expty IRTree = transExp(Tr_outermost(), venv, tenv, NULL, root);
	if (IRTree.exp->kind == Tr_ex) {
		pr_tree_exp(out, IRTree.exp->u.ex, 8);
		json_tree_exp_init(json_out, IRTree.exp->u.ex);
	}
	else if (IRTree.exp->kind == Tr_nx) {
		pr_stm(out, IRTree.exp->u.nx, 8);
		json_stm_init(json_out, IRTree.exp->u.nx);
	}
	else
		printf("cx, can't print");
	fclose(out);
	fclose(json_out);
}


static struct expty transExp(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	// A_varExp, A_assignExp��ҪtransVar, A_letExp��ҪtransDec
	switch (a->kind) {		
	case A_varExp:		transExp_var(level, venv, tenv, breakk, a);		break;
	case A_nilExp:		transExp_nil();									break;
	case A_intExp:		transExp_int(a->u.intt);						break;
	case A_stringExp:	transExp_string(a->u.stringg);					break;
	case A_callExp:		transExp_call(level, venv, tenv, breakk, a);	break;
	case A_opExp:		transExp_op(level, venv, tenv, breakk, a);		break;
	case A_recordExp:	transExp_record(level, venv, tenv, breakk, a);	break;						// Ӧ���Ǹ����������б��õ�
	case A_seqExp:		transExp_seq(level, venv, tenv, breakk, a);		break;
	case A_assignExp:	transExp_assign(level, venv, tenv, breakk, a);	break;
	case A_ifExp:		transExp_if(level, venv, tenv, breakk, a);		break;
	case A_whileExp:	transExp_while(level, venv, tenv, breakk, a);	break;
	case A_forExp:		transExp_for(level, venv, tenv, breakk, a);		break;
	case A_breakExp:	transExp_break(level, venv, tenv, breakk, a);	break;
	case A_letExp:		transExp_let(level, venv, tenv, breakk, a);		break;
	case A_arrayExp:	transExp_array(level, venv, tenv, breakk, a);	break;
	default: assert(0);
	}
}
static struct expty transVar(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)
{
	// A_fieldVar, A_subscriptVar��ҪtransVar
	switch(v->kind) {
	case A_simpleVar:	transVar_simple(level, venv, tenv, breakk, v);	break;
	case A_fieldVar:	transVar_field(level, venv, tenv, breakk, v);	break;
	case A_subscriptVar:transVar_subscript(level, venv, tenv, breakk, v);break;
	default: assert(0);
	}
}
static Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	switch (d->kind) {
	case A_functionDec:	transDec_function(level, venv, tenv, breakk, d);	break;
	case A_varDec:		transDec_var(level, venv, tenv, breakk, d);			break;		// �������
	case A_typeDec:		transDec_type(level, venv, tenv, breakk, d);		break;
	default: assert(0);
	}
}
static Ty_ty transTy(S_table tenv, A_ty t)
{
	switch (t->kind) {
	case A_nameTy:	transTy_name(tenv, t);	break;
	case A_recordTy:transTy_record(tenv, t); break;
	case A_arrayTy:	transTy_array(tenv, t);	break;
	default: assert(0);
	}
}


struct expty transExp_var(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	struct expty res;
	res = transVar(level, venv, tenv, breakk, a->u.var);
	return res;
}
struct expty transExp_nil()		// ����Tr_nilExp
{
	struct expty res;
	res = expTy(Tr_nilExp(), Ty_Nil());
	return res;
}
struct expty transExp_int(int intt) 
{
	struct expty res;
	res = expTy(Tr_intExp(intt), Ty_Int());			// ����CONST, ����ΪTy_int
	return res;
}
struct expty transExp_string(string stringg)
{
	struct expty res;
	res = expTy(Tr_stringExp(stringg), Ty_String());	// ����NAME, ����ΪTy_string
	return res;
}
struct expty transExp_call(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	A_expList args = NULL;
	Ty_tyList formals;
	E_enventry x = S_look(venv, a->u.call.func);			// ��ֵ�ռ��в��Һ�����
	Tr_exp translation = Tr_noExp();
	Tr_expList argList = Tr_ExpList();
	if (x && x->kind == E_funEntry) {						// ����Ǻ������
															// check type of formals
		formals = x->u.fun.formals;							// ȡ��ֵ�ռ��к��������������ʽformals. formals��Ty_tyList��ʽ, ÿ��Ty_ty������record, name(��ͨ����), array, ����int, ����string��
		for (args = a->u.call.args; args && formals;		// ȡ��ʵ�����������ʽargs, ����
		args = args->tail, formals = formals->tail) {
			struct expty arg = transExp(level, venv, tenv, breakk, args->head);		// ��ÿ��arg������, ��ȡ����
			if (!is_equal_ty(arg.ty, formals->head))								// �Ƚ�����, ������Ծͱ���
				EM_error(args->head->pos, "incorrect type %s; expected %s",
					Ty_ToString(arg.ty), Ty_ToString(formals->head));
			Tr_ExpList_append(argList, arg.exp);									// ���û��, �Ͱ����������������ӵ�IR����
		}
		/* Check we have the same number of arguments and formals */
		if (args == NULL && formals != NULL)										// �������������һ�¾ͱ���
			EM_error(a->pos, "not enough arguments");
		else if (args != NULL && formals == NULL)									// �������������һ�¾ͱ���
			EM_error(a->pos, "too many arguments");
		translation = Tr_callExp(level, x->u.fun.level, x->u.fun.label, argList);	// ����CALL���ڵ�
		return expTy(translation, actual_ty(x->u.fun.result));
	}
	else {
		EM_error(a->pos, "undefined function %s", S_name(a->u.call.func));			// ������Ǻ�������򱨴�
		return expTy(translation, Ty_Int());
	}
}
struct expty transExp_op(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	A_oper oper = a->u.op.oper;
	struct expty left = transExp(level, venv, tenv, breakk, a->u.op.left);			// ��߹���IR��
	struct expty right = transExp(level, venv, tenv, breakk, a->u.op.right);		// �ұ߹���IR��
	Tr_exp translation = Tr_noExp();
	switch (oper) {
	case A_plusOp:
	case A_minusOp:
	case A_timesOp:
	case A_divideOp:
		if (left.ty->kind != Ty_int)											// ����Ӽ��˳�, ���һ�಻�������򱨴�
			EM_error(a->u.op.left->pos, "%s expression given for LHS; expected int",
				Ty_ToString(left.ty));
		if (right.ty->kind != Ty_int)											// ����Ӽ��˳�, ���һ�಻�������򱨴�
			EM_error(a->u.op.right->pos, "%s expression given for RHS; expected int",
				Ty_ToString(right.ty));
		return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());			// ����BINOP�ڵ�
	case A_eqOp:
	case A_neqOp:
		switch (left.ty->kind) {
		case Ty_int:
			if (is_equal_ty(right.ty, left.ty))								// ����ȺͲ���������, �������һ��, ΪTy_int
				translation = Tr_eqExp(oper, left.exp, right.exp);			// ����true��falseΪNULL��CJUMP
			break;
		case Ty_string:
			if (is_equal_ty(right.ty, left.ty))								// ����ȺͲ���������, �������һ��, ΪTy_string
				translation = Tr_eqStringExp(oper, left.exp, right.exp);	// ֱ�ӱȽ�, ����CONST(1)��CONST(0)
			break;
		case Ty_array:
		{
			if (right.ty->kind != left.ty->kind) {							// ����ȺͲ���������, �������ΪTy_array�Ҳ�һ���򱨴�
				EM_error(a->u.op.right->pos,
					"%s expression given for RHS; expected %s",
					Ty_ToString(right.ty), Ty_ToString(left.ty));
			}
			translation = Tr_eqRef(oper, left.exp, right.exp);				// ��������CONST(0)
			break;
		}
		case Ty_record:
		{
			if (right.ty->kind != Ty_record && right.ty->kind != Ty_nil) {	// ����ȺͲ���������, �������ΪTy_record�Ҳ�һ���򱨴�
				EM_error(a->u.op.right->pos,
					"%s expression given for RHS; expected record or nil",
					Ty_ToString(right.ty));
			}
			translation = Tr_eqRef(oper, left.exp, right.exp);				// ��������CONST(0)
			break;
		}
		default:															// �������ͱ���
		{
			EM_error(a->u.op.right->pos, "unexpected %s expression in comparsion",
				Ty_ToString(right.ty));
		}
		}
		return expTy(translation, Ty_Int());
	case A_ltOp:
	case A_leOp:
	case A_gtOp:
	case A_geOp:
	{
		if (right.ty->kind != left.ty->kind) {									// �ڱȽ�������, ������Ͳ�һ���򱨴�
			EM_error(a->u.op.right->pos,
				"%s expression given for RHS; expected %s",
				Ty_ToString(right.ty), Ty_ToString(left.ty));
		}
		switch (left.ty->kind) {
		case Ty_int:
			translation = Tr_relExp(oper, left.exp, right.exp); break;		// ���ΪTy_int, ����true��falseΪNULL��CJUMP
		case Ty_string:
			translation = Tr_noExp(); break;								// ���ΪTy_string, ����CONST(0)
		default:															// ���������޷��Ƚ�, ����
		{
			EM_error(a->u.op.right->pos, "unexpected type %s in comparsion",
				Ty_ToString(right.ty));
			translation = Tr_noExp();
		}
		}
		return expTy(translation, Ty_Int());
	}
	}
	assert(0 && "Invalid operator in expression");
	return expTy(Tr_noExp(), Ty_Int());
}
struct expty transExp_record(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	Ty_ty typ = S_look_ty(tenv, a->u.record.typ);									// �����Ϳռ����������
	if (!typ) {																		// ����������򱨴�
		EM_error(a->pos, "undefined type");
		return expTy(Tr_noExp(), Ty_Record(NULL));
	}
	if (typ->kind != Ty_record)														// ���ڵ�����Ty_recordҲ����
		EM_error(a->pos, "%s is not a record type", S_name(a->u.record.typ));
	Ty_fieldList fieldTys = typ->u.record;
	A_efieldList recList;
	Tr_expList list = Tr_ExpList();
	int n = 0;
	for (recList = a->u.record.fields; recList;
	recList = recList->tail, fieldTys = fieldTys->tail, n++) {				// �Ƚ����Ϳռ��е����ƺ������Ƿ�ƥ��, ��ƥ���򱨴�
		struct expty e = transExp(level, venv, tenv, breakk, recList->head->exp);	// ����ʵ����ÿһ���IR��
		if (recList->head->name != fieldTys->head->name)
			EM_error(a->pos, "%s not a valid field name", recList->head->name);
		if (!is_equal_ty(fieldTys->head->ty, e.ty))
			EM_error(recList->head->exp->pos, "type error: given %s but expected %s",
				Ty_ToString(e.ty), Ty_ToString(fieldTys->head->ty));
		Tr_ExpList_prepend(list, e.exp);											// �Ӻ���ǰ����list, ͷ���Ϊ���һ��
	}
	return expTy(Tr_recordExp(n, list), typ);										// Tr_recordExpҲ�Ӻ���ǰ����ESEQ, ����˳���ֶ���
}
struct expty transExp_seq(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	struct expty expr = expTy(Tr_noExp(), Ty_Void()); /* empty seq case */
	A_expList seq;
	Tr_expList list = Tr_ExpList();
	for (seq = a->u.seq; seq; seq = seq->tail) {
		expr = transExp(level, venv, tenv, breakk, seq->head);						// ��ÿһ�����ʽ����IR��
		Tr_ExpList_prepend(list, expr.exp);											// �Ӻ���ǰ����list, ͷ���Ϊ���һ��
	}
	// At least one exp in the list.
	if (Tr_ExpList_empty(list))
		Tr_ExpList_prepend(list, expr.exp);
	return expTy(Tr_seqExp(list), expr.ty);											// Tr_seqExpҲ�Ӻ���ǰ����ESEQ, ����˳���ֶ���
}
struct expty transExp_assign(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)	// ������Tr_assignExp
{
	struct expty exp = transExp(level, venv, tenv, breakk, a->u.assign.exp);		// ������ʽ��IR��
	struct expty var = transVar(level, venv, tenv, breakk, a->u.assign.var);		// transVar�ҵ�����
	if (!is_equal_ty(var.ty, exp.ty))												// ������Ͳ�һ���򱨴�
		EM_error(a->u.assign.exp->pos, "expression not of expected type %s", Ty_ToString(var.ty));
	return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());						// ����MOVE
}
struct expty transExp_if(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	//Tr_exp test_exp = NULL, then_exp = NULL, elsee_exp = NULL;
	//struct expty test = transExp(level, venv, tenv, breakk, a->u.iff.test);						// ��test���ַ����IR��
	//test_exp = test.exp;
	//if (test.ty->kind != Ty_int) EM_error(a->u.iff.test->pos, "integer required");				// ���test�ķ���ֵ����int�򱨴�
	//struct expty then = transExp(level, venv, tenv, breakk, a->u.iff.then);						// ��then���ַ����IR��
	//then_exp = then.exp;
	//if (a->u.iff.elsee) {																		// �������elsee����
	//	struct expty elsee = transExp(level, venv, tenv, breakk, a->u.iff.elsee);				// ��elsee����Ҳ�����IR��
	//	elsee_exp = elsee.exp;
	//}
	//return expTy(Tr_ifExp(test_exp, then_exp, elsee_exp), Ty_Void());

	struct expty test, then, elsee;
	test = transExp(level, venv, tenv, breakk, a->u.iff.test);
	if (test.ty->kind != Ty_int)
		EM_error(a->u.iff.test->pos, "integer required");
	then = transExp(level, venv, tenv, breakk, a->u.iff.then);
	if (a->u.iff.elsee) {
		elsee = transExp(level, venv, tenv, breakk, a->u.iff.elsee);
		if (!is_equal_ty(then.ty, elsee.ty)) {
			EM_error(a->u.iff.elsee->pos, "if-then-else branches must return same type");
		}
		return expTy(Tr_ifExp(test.exp, then.exp, elsee.exp), then.ty);
	}
	else {
		if (then.ty->kind != Ty_void)
			EM_error(a->u.iff.then->pos, "must produce no value");
		return expTy(Tr_ifExp(test.exp, then.exp, NULL), Ty_Void());
	}
}
struct expty transExp_while(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)			// ������Tr_whileExp
{
	Tr_exp test_exp = NULL, body_exp = NULL;
	struct expty test = transExp(level, venv, tenv, breakk, a->u.whilee.test); test_exp = test.exp;		// ��test���ַ����IR��
	if (test.ty->kind != Ty_int) EM_error(a->u.whilee.test->pos, "integer required");					// ���test�ķ���ֵ����int�򱨴�
	Tr_exp newBreakk = Tr_doneExp();																	// �ı���breakk��ֵ, ��һ��NAME�µ�new label
	struct expty body = transExp(level, venv, tenv, breakk, a->u.whilee.body); body_exp = body.exp;		// ��body���ַ����IR��
	if (body.ty->kind != Ty_void) EM_error(a->u.whilee.body->pos, "must produce no value");				// body���ַ���ֵһ��Ϊvoid, ���򱨴�
	return expTy(Tr_whileExp(test_exp, newBreakk, body_exp), Ty_Void());
}
struct expty transExp_for(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	/* Convert a for loop into a let expression with a while loop */
	A_dec i = A_VarDec(a->pos, a->u.forr.var, NULL, a->u.forr.lo);
	A_dec limit = A_VarDec(a->pos, S_Symbol("limit"), NULL, a->u.forr.hi);
	A_dec test = A_VarDec(a->pos, S_Symbol("test"), NULL, A_IntExp(a->pos, 1));
	A_exp testExp = A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("test")));
	A_exp iExp = A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var));
	A_exp limitExp = A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("limit")));
	A_exp increment = A_AssignExp(a->pos,
		A_SimpleVar(a->pos, a->u.forr.var),
		A_OpExp(a->pos, A_plusOp, iExp, A_IntExp(a->pos, 1)));
	A_exp setFalse = A_AssignExp(a->pos,
		A_SimpleVar(a->pos, S_Symbol("test")), A_IntExp(a->pos, 0));
	/* The let expression we pass to this function */
	A_exp letExp = A_LetExp(a->pos,														// �Ƚ�forѭ���ĳ�whileѭ��
		A_DecList(i, A_DecList(limit, A_DecList(test, NULL))),
		A_SeqExp(a->pos,
			A_ExpList(
				A_IfExp(a->pos,
					A_OpExp(a->pos, A_leOp, a->u.forr.lo, a->u.forr.hi),
					A_WhileExp(a->pos, testExp,
						A_SeqExp(a->pos,
							A_ExpList(a->u.forr.body,
								A_ExpList(
									A_IfExp(a->pos,
										A_OpExp(a->pos, A_ltOp, iExp,
											limitExp),
										increment, setFalse),
									NULL)))),
					NULL),
				NULL)));
	struct expty e = transExp(level, venv, tenv, breakk, letExp);						// ��ת���µĳ����﷨��
	return e;
}
struct expty transExp_break(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)		// ������Tr_breakExp
{
	Tr_exp translation = Tr_noExp();
	if (!breakk) EM_error(a->pos, "illegal break expression; must be inside loop construct");		// �������ѭ�����򱨴�
	else translation = Tr_breakExp(breakk);															// ��break����ɴ���JUMP��IR��
	return expTy(translation, Ty_Void());
}
struct expty transExp_let(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)	// ������Tr_seqExp, Tr_ExpList_prepend
{
	Tr_expList list = Tr_ExpList();
	S_beginScope(tenv);																// �����µ����Ϳռ�
	S_beginScope(venv);																// �����µ�ֵ�ռ�
	for (A_decList d = a->u.let.decs; d; d = d->tail)
		Tr_ExpList_prepend(list, transDec(level, venv, tenv, breakk, d->head));		// ����ÿ������, �������б�
	struct expty expr = transExp(level, venv, tenv, breakk, a->u.let.body);			// ����body, �������б�
	Tr_ExpList_prepend(list, expr.exp);
	S_endScope(tenv);																// �������Ϳռ�
	S_endScope(venv);																// ����ֵ�ռ�
	return expTy(Tr_seqExp(list), expr.ty);
}
struct expty transExp_array(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)		// ������Tr_arrayExp
{
	Ty_ty arrayType = S_look_ty(tenv, a->u.array.typ);								// ���Ϳռ��в���array�Ķ���
	if (!arrayType) {																// ���array������, �����ҷ���0
		EM_error(a->pos, "undefined type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	struct expty init = transExp(level, venv, tenv, breakk, a->u.array.init);		// ����init
	if (!is_equal_ty(arrayType->u.array, init.ty))									// �����������ͺͳ�ʼ�������Ͳ�һ���򱨴� 
		EM_error(a->u.array.init->pos, "type error: %s for initialisation expression; %s required", Ty_ToString(init.ty), Ty_ToString(arrayType->u.array));
	struct expty size = transExp(level, venv, tenv, breakk, a->u.array.size);		// ����size
	if (size.ty->kind != Ty_int)													// ���size����int, ����
		EM_error(a->u.array.size->pos, "type error: %s for size expression; int required", Ty_ToString(size.ty));
	return expTy(Tr_arrayExp(size.exp, init.exp), arrayType);
}

struct expty transVar_simple(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)	// ������Tr_simpleVar
{
	E_enventry x = S_look(venv, v->u.simple);								// ��ֵ�ռ��в���
	if (!x || x->kind != E_varEntry) {										// ��������ڻ����Ͳ���ȷ, �򱨴�
		EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
		return expTy(Tr_noExp(), Ty_Int());
	}					
	return expTy(Tr_simpleVar(x->u.var.access, level), actual_ty(x->u.var.ty));	// ����TEMP����MEM
}
struct expty transVar_field(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)		// ������Tr_fieldVar
{
	struct expty e = transVar(level, venv, tenv, breakk, v->u.field.var);				// ͨ���������ķ�ʽ��ȡ��������
	if (e.ty->kind != Ty_record) {														// ������Ͳ���Ty_record�򱨴�
		EM_error(v->u.field.var->pos, "not a record type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	int fieldOffset = 0;
	Ty_fieldList f;
	int hasFind = 0;
	for (f = e.ty->u.record; f; f = f->tail, fieldOffset++) {							// ����ÿһ��field�е�ÿ���ӱ���
		if (f->head->name == v->u.field.sym) {											// �������һ�����ע��break
			hasFind = 1;
			break;
		}
	}
	if (!hasFind) {																		// ����ҵ��˶�Ӧ���ӱ���
		EM_error(v->pos, "no such field %s for record type", S_name(v->u.field.sym));
		return expTy(Tr_noExp(), Ty_Int());
	}
	return expTy(Tr_fieldVar(e.exp, fieldOffset), actual_ty(f->head->ty));
}
struct expty transVar_subscript(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)
{
	struct expty e = transVar(level, venv, tenv, breakk, v->u.subscript.var);			// ͨ���������ķ�ʽ��ȡ��������
	if (e.ty->kind != Ty_array) {														// ������Ͳ���Ty_array�򱨴�
		EM_error(v->u.subscript.var->pos, "not an array type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	struct expty index = transExp(level, venv, tenv, breakk, v->u.subscript.exp);		// ����index
	if (index.ty->kind != Ty_int) {														// ���index����int�򱨴�
		EM_error(v->u.subscript.exp->pos, "integer required");
		return expTy(Tr_noExp(), actual_ty(e.ty->u.array));
	}
	return expTy(Tr_subscriptVar(e.exp, index.exp), actual_ty(e.ty->u.array));
}

Tr_exp transDec_function(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	for (A_fundecList funcNode = d->u.function; funcNode != NULL; funcNode = funcNode->tail) {	// ����A_letExp�е�ÿһ������(d->u.function��һ������)
		A_fundec func = funcNode->head;															// func->head�õ���ǰ�ĺ���A_fundec
		Ty_ty resultTy = NULL;
		if (func->result) {
			resultTy = S_look(tenv, func->result);												// ��鷵�������Ƿ��Ѷ���
			if (!resultTy) {
				EM_error(func->pos, "undefined type for return type");
			}
		}
		if (!resultTy) resultTy = Ty_Void();

		Ty_tyList formalTys = NULL, tail = NULL;																// ���������ɲ�������, �õ���������ͷ
		int index = 0;
		for (A_fieldList paramNode = func->params; paramNode; paramNode = paramNode->tail, index++) {			// ���ÿ�������������Ƿ��Ѷ���
			A_field paramItem = paramNode->head;
			Ty_ty t = S_look_ty(tenv, paramItem->typ);
			if (!t) {
				EM_error(paramItem->pos, "undefined type %s", S_name(paramItem->typ));							// ���û�ж����򱨴�
				continue;
			}
			if (index == 0) {
				formalTys = Ty_TyList(t, NULL);
				tail = formalTys;
				continue;
			}
			tail->tail = Ty_TyList(t, NULL);
			tail = tail->tail;
		}																

		U_boolList formals = NULL, tail2 = NULL;																// �������Ƿ�������Ϣ���ɲ���������, �õ�����ͷ
		index = 0;
		for (A_fieldList paramNode = func->params; paramNode; paramNode = paramNode->tail, index++) {
			if (index == 0) {
				formals = U_BoolList(TRUE, NULL);
				tail2 = formals;
				continue;
			}
			tail2->tail = U_BoolList(TRUE, NULL);
			tail2 = tail2->tail;
		}

		Temp_label funLabel = Temp_newlabel();
		Tr_level funLevel = Tr_newLevel(level, funLabel, formals);
		S_enter(venv, func->name, E_FunEntry(funLevel, funLabel, formalTys, resultTy));			// �������浽ֵ�ռ���
	}
	for (A_fundecList funcNode = d->u.function; funcNode != NULL; funcNode = funcNode->tail) {	// ����A_letExp�е�ÿһ������
		A_fundec func = funcNode->head;
		E_enventry funEntry = S_look(venv, func->name);
		S_beginScope(venv);																		// �Ժ����ڲ������µ�������
		Ty_tyList paramTys = funEntry->u.fun.formals;
		A_fieldList paramFields;
		Tr_accessList accessList = Tr_formals(funEntry->u.fun.level);
		for (paramFields = func->params; paramFields;
		paramFields = paramFields->tail, paramTys = paramTys->tail,accessList = accessList->tail) {	// �����β�
			S_enter(venv, paramFields->head->name, E_VarEntry(accessList->head, paramTys->head));	// ���βμ���ֵ�ռ�
		}
		struct expty e = transExp(funEntry->u.fun.level, venv, tenv,
			breakk, func->body);																// ���뺯����
		if (!is_equal_ty(funEntry->u.fun.result, e.ty))											// ���������ķ������ͺͺ����ķ������Ͳ�һ���򱨴�
			EM_error(func->body->pos, "incorrect return type %s; expected %s", Ty_ToString(e.ty), Ty_ToString(funEntry->u.fun.result));

		Tr_procEntryExit(funEntry->u.fun.level, e.exp, e.ty->kind == Ty_void);
		S_endScope(venv);																		// ����������
	}
	return Tr_noExp();
}
Tr_exp transDec_var(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	struct expty e = transExp(level, venv, tenv, breakk, d->u.var.init);		// �Գ�ʼ�����ʽ����IR��
	Tr_access access = Tr_allocLocal(level, TRUE);								// ����Ĵ�����ջ, TRUE��ʾ���ݱ���, ����ջ
	if (d->u.var.typ) {															// �����ʽ����������
		Ty_ty type = S_look_ty(tenv, d->u.var.typ);								// ��������
		if (!type) {															// ������Ͳ������򱨴�
			EM_error(d->pos, "undefined type %s", S_name(d->u.var.typ));
			type = Ty_Void();
		}
		if (!is_equal_ty(type, e.ty))											// �����������ͺͳ�ʼ�������Ͳ�ͬ�򱨴�
			EM_error(d->pos, "type error: %s given, expected %s for expression", Ty_ToString(e.ty), S_name(d->u.var.typ));
		S_enter(venv, d->u.var.var, E_VarEntry(access, type));					// ����ֵ�ռ�
	}
	else {																		// ���û����ʽ��������
		if (e.ty->kind == Ty_nil)												// �����ʼ��������
			EM_error(d->u.var.init->pos, "illegal use nil expression");
		S_enter(venv, d->u.var.var, E_VarEntry(access, e.ty));					// �����ʼ�������;��ó�ʼ��������, ����ֵ�ռ�
	}
	return Tr_assignExp(Tr_simpleVar(access, level), e.exp);					// ����IR��
}
Tr_exp transDec_type(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	bool isCyclic = TRUE;																					// �Ƿ����ѭ������ı���
	for (A_nametyList nameList = d->u.type; nameList; nameList = nameList->tail)							// ����ÿ����������
		S_enter(tenv, nameList->head->name, Ty_Name(nameList->head->name, NULL));							// ��ÿ���������ͼ������Ϳռ�
	for (A_nametyList nameList = d->u.type; nameList; nameList = nameList->tail) {							// ����ÿ����������
		Ty_ty t = transTy(tenv, nameList->head->ty);
		if (isCyclic) {
			if (t->kind != Ty_name) isCyclic = FALSE;
		}
		Ty_ty nameTy = S_look(tenv, nameList->head->name);
		nameTy->u.name.ty = t;
	}
	if (isCyclic) EM_error(d->pos, "illegal type cycle: cycle must contain record, array or built-in type");// ���ѭ�������򱨴�
	return Tr_noExp();
}

Ty_ty transTy_name(S_table tenv, A_ty t)
{
	Ty_ty ty = S_look(tenv, t->u.name);										// ���Ϳռ���Ѱ������
	if (!ty) EM_error(t->pos, "undefined type %s", S_name(t->u.name));		// ���Ͳ������򱨴�
	return ty;
}
Ty_ty transTy_record(S_table tenv, A_ty t)
{
	A_fieldList fields = t->u.record;
	Ty_fieldList fieldTys = NULL;
	Ty_fieldList tailList = fieldTys;
	int index = 0;
	for (A_fieldList fieldList = fields; fieldList; fieldList = fieldList->tail, index++) {		// ����ÿһ������
		Ty_ty t = S_look(tenv, fieldList->head->typ);
		if (!t) {																				// �����ֵ�ռ����Ҳ����ͱ���
			EM_error(fieldList->head->pos, "undefined type %s", S_name(fieldList->head->typ));
			continue;
		}
		Ty_field f = Ty_Field(fieldList->head->name, t);
		if (index == 0) {
			fieldTys = Ty_FieldList(f, NULL);
			tailList = fieldTys;
			continue;
		}
		tailList->tail = Ty_FieldList(f, NULL);
		tailList = tailList->tail;
	}
	return Ty_Record(fieldTys);
}
Ty_ty transTy_array(S_table tenv, A_ty t)
{
	Ty_ty ty = S_look(tenv, t->u.name);										// ���Ϳռ���Ѱ������
	if (!ty) EM_error(t->pos, "undefined type %s", S_name(t->u.name));		// ���Ͳ������򱨴�
	return Ty_Array(ty);
}

