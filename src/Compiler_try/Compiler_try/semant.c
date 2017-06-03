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
static int is_equal_ty(Ty_ty tType, Ty_ty eType)		// 比较类型是否相同
{
	Ty_ty actualtType = actual_ty(tType);
	int tyKind = actualtType->kind;
	int eKind = eType->kind;
	return ( ((tyKind == Ty_record || tyKind == Ty_array) && actualtType == eType) ||
		(tyKind == Ty_record && eKind == Ty_nil) ||
		(tyKind != Ty_record && tyKind != Ty_array && tyKind == eKind) );
}

void SEM_transProg(A_exp root)					// semant.c的入口, 在这里初始化值空间和类型空间, 并开始解析抽象语法树的根A_exp exp
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
	// A_varExp, A_assignExp需要transVar, A_letExp需要transDec
	switch (a->kind) {		
	case A_varExp:		transExp_var(level, venv, tenv, breakk, a);		break;
	case A_nilExp:		transExp_nil();									break;
	case A_intExp:		transExp_int(a->u.intt);						break;
	case A_stringExp:	transExp_string(a->u.stringg);					break;
	case A_callExp:		transExp_call(level, venv, tenv, breakk, a);	break;
	case A_opExp:		transExp_op(level, venv, tenv, breakk, a);		break;
	case A_recordExp:	transExp_record(level, venv, tenv, breakk, a);	break;						// 应该是给函数参数列表用的
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
	// A_fieldVar, A_subscriptVar需要transVar
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
	case A_varDec:		transDec_var(level, venv, tenv, breakk, d);			break;		// 定义变量
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
struct expty transExp_nil()		// 调用Tr_nilExp
{
	struct expty res;
	res = expTy(Tr_nilExp(), Ty_Nil());
	return res;
}
struct expty transExp_int(int intt) 
{
	struct expty res;
	res = expTy(Tr_intExp(intt), Ty_Int());			// 返回CONST, 类型为Ty_int
	return res;
}
struct expty transExp_string(string stringg)
{
	struct expty res;
	res = expTy(Tr_stringExp(stringg), Ty_String());	// 返回NAME, 类型为Ty_string
	return res;
}
struct expty transExp_call(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	A_expList args = NULL;
	Ty_tyList formals;
	E_enventry x = S_look(venv, a->u.call.func);			// 在值空间中查找函数名
	Tr_exp translation = Tr_noExp();
	Tr_expList argList = Tr_ExpList();
	if (x && x->kind == E_funEntry) {						// 如果是函数入口
															// check type of formals
		formals = x->u.fun.formals;							// 取出值空间中函数的输入变量格式formals. formals是Ty_tyList格式, 每个Ty_ty可能是record, name(普通变量), array, 常量int, 常量string等
		for (args = a->u.call.args; args && formals;		// 取出实际输入变量格式args, 遍历
		args = args->tail, formals = formals->tail) {
			struct expty arg = transExp(level, venv, tenv, breakk, args->head);		// 对每个arg建立树, 获取类型
			if (!is_equal_ty(arg.ty, formals->head))								// 比较类型, 如果不对就报错
				EM_error(args->head->pos, "incorrect type %s; expected %s",
					Ty_ToString(arg.ty), Ty_ToString(formals->head));
			Tr_ExpList_append(argList, arg.exp);									// 如果没错, 就把这个变量的这棵树加到IR树中
		}
		/* Check we have the same number of arguments and formals */
		if (args == NULL && formals != NULL)										// 如果参数数量不一致就报错
			EM_error(a->pos, "not enough arguments");
		else if (args != NULL && formals == NULL)									// 如果参数数量不一致就报错
			EM_error(a->pos, "too many arguments");
		translation = Tr_callExp(level, x->u.fun.level, x->u.fun.label, argList);	// 生成CALL树节点
		return expTy(translation, actual_ty(x->u.fun.result));
	}
	else {
		EM_error(a->pos, "undefined function %s", S_name(a->u.call.func));			// 如果不是函数入口则报错
		return expTy(translation, Ty_Int());
	}
}
struct expty transExp_op(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	A_oper oper = a->u.op.oper;
	struct expty left = transExp(level, venv, tenv, breakk, a->u.op.left);			// 左边构造IR树
	struct expty right = transExp(level, venv, tenv, breakk, a->u.op.right);		// 右边构造IR树
	Tr_exp translation = Tr_noExp();
	switch (oper) {
	case A_plusOp:
	case A_minusOp:
	case A_timesOp:
	case A_divideOp:
		if (left.ty->kind != Ty_int)											// 如果加减乘除, 如果一侧不是整数则报错
			EM_error(a->u.op.left->pos, "%s expression given for LHS; expected int",
				Ty_ToString(left.ty));
		if (right.ty->kind != Ty_int)											// 如果加减乘除, 如果一侧不是整数则报错
			EM_error(a->u.op.right->pos, "%s expression given for RHS; expected int",
				Ty_ToString(right.ty));
		return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());			// 生成BINOP节点
	case A_eqOp:
	case A_neqOp:
		switch (left.ty->kind) {
		case Ty_int:
			if (is_equal_ty(right.ty, left.ty))								// 在相等和不等运算下, 如果类型一致, 为Ty_int
				translation = Tr_eqExp(oper, left.exp, right.exp);			// 生成true和false为NULL的CJUMP
			break;
		case Ty_string:
			if (is_equal_ty(right.ty, left.ty))								// 在相等和不等运算下, 如果类型一致, 为Ty_string
				translation = Tr_eqStringExp(oper, left.exp, right.exp);	// 直接比较, 生成CONST(1)或CONST(0)
			break;
		case Ty_array:
		{
			if (right.ty->kind != left.ty->kind) {							// 在相等和不等运算下, 如果类型为Ty_array且不一致则报错
				EM_error(a->u.op.right->pos,
					"%s expression given for RHS; expected %s",
					Ty_ToString(right.ty), Ty_ToString(left.ty));
			}
			translation = Tr_eqRef(oper, left.exp, right.exp);				// 否则生成CONST(0)
			break;
		}
		case Ty_record:
		{
			if (right.ty->kind != Ty_record && right.ty->kind != Ty_nil) {	// 在相等和不等运算下, 如果类型为Ty_record且不一致则报错
				EM_error(a->u.op.right->pos,
					"%s expression given for RHS; expected record or nil",
					Ty_ToString(right.ty));
			}
			translation = Tr_eqRef(oper, left.exp, right.exp);				// 否则生成CONST(0)
			break;
		}
		default:															// 其他类型报错
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
		if (right.ty->kind != left.ty->kind) {									// 在比较运算下, 如果类型不一致则报错
			EM_error(a->u.op.right->pos,
				"%s expression given for RHS; expected %s",
				Ty_ToString(right.ty), Ty_ToString(left.ty));
		}
		switch (left.ty->kind) {
		case Ty_int:
			translation = Tr_relExp(oper, left.exp, right.exp); break;		// 如果为Ty_int, 生成true和false为NULL的CJUMP
		case Ty_string:
			translation = Tr_noExp(); break;								// 如果为Ty_string, 生成CONST(0)
		default:															// 其他类型无法比较, 报错
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
	Ty_ty typ = S_look_ty(tenv, a->u.record.typ);									// 在类型空间里查找类型
	if (!typ) {																		// 如果不存在则报错
		EM_error(a->pos, "undefined type");
		return expTy(Tr_noExp(), Ty_Record(NULL));
	}
	if (typ->kind != Ty_record)														// 存在但不是Ty_record也报错
		EM_error(a->pos, "%s is not a record type", S_name(a->u.record.typ));
	Ty_fieldList fieldTys = typ->u.record;
	A_efieldList recList;
	Tr_expList list = Tr_ExpList();
	int n = 0;
	for (recList = a->u.record.fields; recList;
	recList = recList->tail, fieldTys = fieldTys->tail, n++) {				// 比较类型空间中的名称和类型是否匹配, 不匹配则报错
		struct expty e = transExp(level, venv, tenv, breakk, recList->head->exp);	// 翻译实际中每一项的IR树
		if (recList->head->name != fieldTys->head->name)
			EM_error(a->pos, "%s not a valid field name", recList->head->name);
		if (!is_equal_ty(fieldTys->head->ty, e.ty))
			EM_error(recList->head->exp->pos, "type error: given %s but expected %s",
				Ty_ToString(e.ty), Ty_ToString(fieldTys->head->ty));
		Tr_ExpList_prepend(list, e.exp);											// 从后向前构造list, 头结点为最后一个
	}
	return expTy(Tr_recordExp(n, list), typ);										// Tr_recordExp也从后向前构造ESEQ, 所以顺序又对了
}
struct expty transExp_seq(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	struct expty expr = expTy(Tr_noExp(), Ty_Void()); /* empty seq case */
	A_expList seq;
	Tr_expList list = Tr_ExpList();
	for (seq = a->u.seq; seq; seq = seq->tail) {
		expr = transExp(level, venv, tenv, breakk, seq->head);						// 对每一个表达式翻译IR树
		Tr_ExpList_prepend(list, expr.exp);											// 从后向前构造list, 头结点为最后一个
	}
	// At least one exp in the list.
	if (Tr_ExpList_empty(list))
		Tr_ExpList_prepend(list, expr.exp);
	return expTy(Tr_seqExp(list), expr.ty);											// Tr_seqExp也从后向前构造ESEQ, 所以顺序又对了
}
struct expty transExp_assign(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)	// 调用了Tr_assignExp
{
	struct expty exp = transExp(level, venv, tenv, breakk, a->u.assign.exp);		// 构造表达式的IR树
	struct expty var = transVar(level, venv, tenv, breakk, a->u.assign.var);		// transVar找到变量
	if (!is_equal_ty(var.ty, exp.ty))												// 如果类型不一致则报错
		EM_error(a->u.assign.exp->pos, "expression not of expected type %s", Ty_ToString(var.ty));
	return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());						// 构造MOVE
}
struct expty transExp_if(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	//Tr_exp test_exp = NULL, then_exp = NULL, elsee_exp = NULL;
	//struct expty test = transExp(level, venv, tenv, breakk, a->u.iff.test);						// 将test部分翻译成IR树
	//test_exp = test.exp;
	//if (test.ty->kind != Ty_int) EM_error(a->u.iff.test->pos, "integer required");				// 如果test的返回值不是int则报错
	//struct expty then = transExp(level, venv, tenv, breakk, a->u.iff.then);						// 将then部分翻译成IR树
	//then_exp = then.exp;
	//if (a->u.iff.elsee) {																		// 如果存在elsee部分
	//	struct expty elsee = transExp(level, venv, tenv, breakk, a->u.iff.elsee);				// 将elsee部分也翻译成IR树
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
struct expty transExp_while(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)			// 调用了Tr_whileExp
{
	Tr_exp test_exp = NULL, body_exp = NULL;
	struct expty test = transExp(level, venv, tenv, breakk, a->u.whilee.test); test_exp = test.exp;		// 将test部分翻译成IR树
	if (test.ty->kind != Ty_int) EM_error(a->u.whilee.test->pos, "integer required");					// 如果test的返回值不是int则报错
	Tr_exp newBreakk = Tr_doneExp();																	// 改变了breakk的值, 是一个NAME下的new label
	struct expty body = transExp(level, venv, tenv, breakk, a->u.whilee.body); body_exp = body.exp;		// 将body部分翻译成IR树
	if (body.ty->kind != Ty_void) EM_error(a->u.whilee.body->pos, "must produce no value");				// body部分返回值一定为void, 否则报错
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
	A_exp letExp = A_LetExp(a->pos,														// 先将for循环改成while循环
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
	struct expty e = transExp(level, venv, tenv, breakk, letExp);						// 再转换新的抽象语法树
	return e;
}
struct expty transExp_break(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)		// 调用了Tr_breakExp
{
	Tr_exp translation = Tr_noExp();
	if (!breakk) EM_error(a->pos, "illegal break expression; must be inside loop construct");		// 如果不在循环内则报错
	else translation = Tr_breakExp(breakk);															// 将break翻译成带有JUMP的IR树
	return expTy(translation, Ty_Void());
}
struct expty transExp_let(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)	// 调用了Tr_seqExp, Tr_ExpList_prepend
{
	Tr_expList list = Tr_ExpList();
	S_beginScope(tenv);																// 开启新的类型空间
	S_beginScope(venv);																// 开启新的值空间
	for (A_decList d = a->u.let.decs; d; d = d->tail)
		Tr_ExpList_prepend(list, transDec(level, venv, tenv, breakk, d->head));		// 翻译每个定义, 并加入列表
	struct expty expr = transExp(level, venv, tenv, breakk, a->u.let.body);			// 翻译body, 并加入列表
	Tr_ExpList_prepend(list, expr.exp);
	S_endScope(tenv);																// 结束类型空间
	S_endScope(venv);																// 结束值空间
	return expTy(Tr_seqExp(list), expr.ty);
}
struct expty transExp_array(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)		// 调用了Tr_arrayExp
{
	Ty_ty arrayType = S_look_ty(tenv, a->u.array.typ);								// 类型空间中查找array的定义
	if (!arrayType) {																// 如果array不存在, 报错并且返回0
		EM_error(a->pos, "undefined type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	struct expty init = transExp(level, venv, tenv, breakk, a->u.array.init);		// 翻译init
	if (!is_equal_ty(arrayType->u.array, init.ty))									// 如果定义的类型和初始化的类型不一致则报错 
		EM_error(a->u.array.init->pos, "type error: %s for initialisation expression; %s required", Ty_ToString(init.ty), Ty_ToString(arrayType->u.array));
	struct expty size = transExp(level, venv, tenv, breakk, a->u.array.size);		// 翻译size
	if (size.ty->kind != Ty_int)													// 如果size不是int, 报错
		EM_error(a->u.array.size->pos, "type error: %s for size expression; int required", Ty_ToString(size.ty));
	return expTy(Tr_arrayExp(size.exp, init.exp), arrayType);
}

struct expty transVar_simple(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)	// 调用了Tr_simpleVar
{
	E_enventry x = S_look(venv, v->u.simple);								// 在值空间中查找
	if (!x || x->kind != E_varEntry) {										// 如果不存在或类型不正确, 则报错
		EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
		return expTy(Tr_noExp(), Ty_Int());
	}					
	return expTy(Tr_simpleVar(x->u.var.access, level), actual_ty(x->u.var.ty));	// 构造TEMP或者MEM
}
struct expty transVar_field(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)		// 调用了Tr_fieldVar
{
	struct expty e = transVar(level, venv, tenv, breakk, v->u.field.var);				// 通过构造树的方式获取变量类型
	if (e.ty->kind != Ty_record) {														// 如果类型不是Ty_record则报错
		EM_error(v->u.field.var->pos, "not a record type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	int fieldOffset = 0;
	Ty_fieldList f;
	int hasFind = 0;
	for (f = e.ty->u.record; f; f = f->tail, fieldOffset++) {							// 遍历每一个field中的每个子变量
		if (f->head->name == v->u.field.sym) {											// 如果类型一致则标注并break
			hasFind = 1;
			break;
		}
	}
	if (!hasFind) {																		// 如果找到了对应的子变量
		EM_error(v->pos, "no such field %s for record type", S_name(v->u.field.sym));
		return expTy(Tr_noExp(), Ty_Int());
	}
	return expTy(Tr_fieldVar(e.exp, fieldOffset), actual_ty(f->head->ty));
}
struct expty transVar_subscript(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)
{
	struct expty e = transVar(level, venv, tenv, breakk, v->u.subscript.var);			// 通过构造树的方式获取变量类型
	if (e.ty->kind != Ty_array) {														// 如果类型不是Ty_array则报错
		EM_error(v->u.subscript.var->pos, "not an array type");
		return expTy(Tr_noExp(), Ty_Int());
	}
	struct expty index = transExp(level, venv, tenv, breakk, v->u.subscript.exp);		// 翻译index
	if (index.ty->kind != Ty_int) {														// 如果index不是int则报错
		EM_error(v->u.subscript.exp->pos, "integer required");
		return expTy(Tr_noExp(), actual_ty(e.ty->u.array));
	}
	return expTy(Tr_subscriptVar(e.exp, index.exp), actual_ty(e.ty->u.array));
}

Tr_exp transDec_function(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	for (A_fundecList funcNode = d->u.function; funcNode != NULL; funcNode = funcNode->tail) {	// 遍历A_letExp中的每一个函数(d->u.function是一个链表)
		A_fundec func = funcNode->head;															// func->head得到当前的函数A_fundec
		Ty_ty resultTy = NULL;
		if (func->result) {
			resultTy = S_look(tenv, func->result);												// 检查返回类型是否已定义
			if (!resultTy) {
				EM_error(func->pos, "undefined type for return type");
			}
		}
		if (!resultTy) resultTy = Ty_Void();

		Ty_tyList formalTys = NULL, tail = NULL;																// 将参数串成参数链表, 得到参数链表头
		int index = 0;
		for (A_fieldList paramNode = func->params; paramNode; paramNode = paramNode->tail, index++) {			// 检查每个参数的类型是否已定义
			A_field paramItem = paramNode->head;
			Ty_ty t = S_look_ty(tenv, paramItem->typ);
			if (!t) {
				EM_error(paramItem->pos, "undefined type %s", S_name(paramItem->typ));							// 如果没有定义则报错
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

		U_boolList formals = NULL, tail2 = NULL;																// 将参数是否逃逸信息串成布尔型链表, 得到链表头
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
		S_enter(venv, func->name, E_FunEntry(funLevel, funLabel, formalTys, resultTy));			// 将函数存到值空间中
	}
	for (A_fundecList funcNode = d->u.function; funcNode != NULL; funcNode = funcNode->tail) {	// 遍历A_letExp中的每一个函数
		A_fundec func = funcNode->head;
		E_enventry funEntry = S_look(venv, func->name);
		S_beginScope(venv);																		// 对函数内部开启新的作用域
		Ty_tyList paramTys = funEntry->u.fun.formals;
		A_fieldList paramFields;
		Tr_accessList accessList = Tr_formals(funEntry->u.fun.level);
		for (paramFields = func->params; paramFields;
		paramFields = paramFields->tail, paramTys = paramTys->tail,accessList = accessList->tail) {	// 遍历形参
			S_enter(venv, paramFields->head->name, E_VarEntry(accessList->head, paramTys->head));	// 将形参加入值空间
		}
		struct expty e = transExp(funEntry->u.fun.level, venv, tenv,
			breakk, func->body);																// 翻译函数体
		if (!is_equal_ty(funEntry->u.fun.result, e.ty))											// 如果函数体的返回类型和函数的返回类型不一致则报错
			EM_error(func->body->pos, "incorrect return type %s; expected %s", Ty_ToString(e.ty), Ty_ToString(funEntry->u.fun.result));

		Tr_procEntryExit(funEntry->u.fun.level, e.exp, e.ty->kind == Ty_void);
		S_endScope(venv);																		// 结束作用域
	}
	return Tr_noExp();
}
Tr_exp transDec_var(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	struct expty e = transExp(level, venv, tenv, breakk, d->u.var.init);		// 对初始化表达式构建IR树
	Tr_access access = Tr_allocLocal(level, TRUE);								// 分配寄存器或栈, TRUE表示逃逸变量, 分配栈
	if (d->u.var.typ) {															// 如果显式定义了类型
		Ty_ty type = S_look_ty(tenv, d->u.var.typ);								// 查找类型
		if (!type) {															// 如果类型不存在则报错
			EM_error(d->pos, "undefined type %s", S_name(d->u.var.typ));
			type = Ty_Void();
		}
		if (!is_equal_ty(type, e.ty))											// 如果定义的类型和初始化的类型不同则报错
			EM_error(d->pos, "type error: %s given, expected %s for expression", Ty_ToString(e.ty), S_name(d->u.var.typ));
		S_enter(venv, d->u.var.var, E_VarEntry(access, type));					// 加入值空间
	}
	else {																		// 如果没有显式定义类型
		if (e.ty->kind == Ty_nil)												// 如果初始化无类型
			EM_error(d->u.var.init->pos, "illegal use nil expression");
		S_enter(venv, d->u.var.var, E_VarEntry(access, e.ty));					// 如果初始化有类型就用初始化的类型, 加入值空间
	}
	return Tr_assignExp(Tr_simpleVar(access, level), e.exp);					// 加入IR树
}
Tr_exp transDec_type(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	bool isCyclic = TRUE;																					// 是否存在循环定义的变量
	for (A_nametyList nameList = d->u.type; nameList; nameList = nameList->tail)							// 遍历每个类内类型
		S_enter(tenv, nameList->head->name, Ty_Name(nameList->head->name, NULL));							// 将每个类内类型加入类型空间
	for (A_nametyList nameList = d->u.type; nameList; nameList = nameList->tail) {							// 遍历每个类内类型
		Ty_ty t = transTy(tenv, nameList->head->ty);
		if (isCyclic) {
			if (t->kind != Ty_name) isCyclic = FALSE;
		}
		Ty_ty nameTy = S_look(tenv, nameList->head->name);
		nameTy->u.name.ty = t;
	}
	if (isCyclic) EM_error(d->pos, "illegal type cycle: cycle must contain record, array or built-in type");// 如果循环定义则报错
	return Tr_noExp();
}

Ty_ty transTy_name(S_table tenv, A_ty t)
{
	Ty_ty ty = S_look(tenv, t->u.name);										// 类型空间中寻找类型
	if (!ty) EM_error(t->pos, "undefined type %s", S_name(t->u.name));		// 类型不存在则报错
	return ty;
}
Ty_ty transTy_record(S_table tenv, A_ty t)
{
	A_fieldList fields = t->u.record;
	Ty_fieldList fieldTys = NULL;
	Ty_fieldList tailList = fieldTys;
	int index = 0;
	for (A_fieldList fieldList = fields; fieldList; fieldList = fieldList->tail, index++) {		// 遍历每一个类型
		Ty_ty t = S_look(tenv, fieldList->head->typ);
		if (!t) {																				// 如果在值空间中找不到就报错
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
	Ty_ty ty = S_look(tenv, t->u.name);										// 类型空间中寻找类型
	if (!ty) EM_error(t->pos, "undefined type %s", S_name(t->u.name));		// 类型不存在则报错
	return Ty_Array(ty);
}

