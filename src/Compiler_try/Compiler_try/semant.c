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
static Ty_tyList makeFormalTys(S_table tenv, A_fieldList params);
static Ty_fieldList makeFieldTys(S_table tenv, A_fieldList fields);
static U_boolList makeFormals(A_fieldList params);
static Ty_ty actual_ty(Ty_ty ty);
static Ty_ty S_look_ty(S_table tenv, S_symbol sym);
static int is_equal_ty(Ty_ty tType, Ty_ty eType);

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

F_fragList SEM_transProg(A_exp exp)					// semant.c�����, �������ʼ��ֵ�ռ�����Ϳռ�, ����ʼ���������﷨���ĸ�A_exp exp
{
	/* Set up the type and value environments */
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
	FILE* out = fopen("IR.txt","w");
	//transExp(Tr_outermost(), venv, tenv, NULL, exp);
	struct expty IRTree = transExp(Tr_outermost(), venv, tenv, NULL, exp);
	if (IRTree.exp->kind == Tr_ex)
		pr_tree_exp(out, IRTree.exp->u.ex, 8);
	else if (IRTree.exp->kind == Tr_nx)
		pr_stm(out, IRTree.exp->u.nx, 8);
	else
		printf("cx, can't print");
	return Tr_getResult();
}

static struct expty transExp(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	printf("%d\n", a->kind);
	switch (a->kind) {		// ֻ��A_varExp��A_assignExp��ҪtransVar, ֻ��A_letExp��ҪtransDec
		case A_varExp:
		{
			return transVar(level, venv, tenv, breakk, a->u.var);
		}
		case A_nilExp:
		{
			return expTy(Tr_nilExp(), Ty_Nil());
		}
		case A_intExp:
		{
			return expTy(Tr_intExp(a->u.intt), Ty_Int());			// ����CONST, ����ΪTy_int
		}
		case A_stringExp:
		{
			return expTy(Tr_stringExp(a->u.stringg), Ty_String());	// ����NAME, ����ΪTy_string
		}
		case A_callExp:
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
			} else {
				EM_error(a->pos, "undefined function %s", S_name(a->u.call.func));			// ������Ǻ�������򱨴�
				return expTy(translation, Ty_Int());
			}
		}
		case A_opExp:
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
					switch(left.ty->kind) {
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
					switch(left.ty->kind) {
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
		case A_recordExp:																	// Ӧ���Ǹ����������б��õ�
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
		case A_seqExp:
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
		case A_assignExp:
		{
			struct expty var = transVar(level, venv, tenv, breakk, a->u.assign.var);		// transVar�ҵ�����
			struct expty exp = transExp(level, venv, tenv, breakk, a->u.assign.exp);		// ������ʽ��IR��
			if (!is_equal_ty(var.ty, exp.ty))												// ������Ͳ�һ���򱨴�
				EM_error(a->u.assign.exp->pos, "expression not of expected type %s",
					Ty_ToString(var.ty));
			return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());						// ����MOVE
		}
		case A_ifExp:
		{
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
			} else {
				if (then.ty->kind != Ty_void)
					EM_error(a->u.iff.then->pos, "must produce no value");
				return expTy(Tr_ifExp(test.exp, then.exp, NULL), Ty_Void());
			}
		}
		case A_whileExp:
		{
			struct expty test = transExp(level, venv, tenv, breakk, a->u.whilee.test);
			if (test.ty->kind != Ty_int)
				EM_error(a->u.whilee.test->pos, "integer required");
			Tr_exp newBreakk = Tr_doneExp();												// �ı���breakk��ֵ, ��һ��NAME�µ�new label
			struct expty body = transExp(level, venv, tenv, breakk, a->u.whilee.body);
			if (body.ty->kind != Ty_void)
				EM_error(a->u.whilee.body->pos, "must produce no value");
			return expTy(Tr_whileExp(test.exp, newBreakk, body.exp), Ty_Void());
		}
		case A_forExp:
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
		case A_breakExp:
		{
			Tr_exp translation = Tr_noExp();
			if (!breakk) {																		// �������ѭ�����򱨴�
				EM_error(a->pos, "illegal break expression; must be inside loop construct");
			} else {
				translation = Tr_breakExp(breakk);
			}
			return expTy(translation, Ty_Void());
		}
		case A_letExp:
		{
			struct expty expr;
			A_decList d;
			Tr_expList list = Tr_ExpList();
			S_beginScope(venv);
			S_beginScope(tenv);
			for (d = a->u.let.decs; d; d = d->tail)
				Tr_ExpList_prepend(list, transDec(level, venv, tenv, breakk, d->head));
			expr = transExp(level, venv, tenv, breakk, a->u.let.body);
			Tr_ExpList_prepend(list, expr.exp); // need result of let at the beginning
			S_endScope(venv);
			S_endScope(tenv);
			return expTy(Tr_seqExp(list), expr.ty);
		}
		case A_arrayExp:
		{
			Ty_ty typ = S_look_ty(tenv, a->u.array.typ);
			if (!typ) {
				EM_error(a->pos, "undefined type");
				return expTy(Tr_noExp(), Ty_Int());
			} else {
				struct expty size = transExp(level, venv, tenv, breakk, a->u.array.size);
				struct expty init = transExp(level, venv, tenv, breakk, a->u.array.init);
				if (size.ty->kind != Ty_int)
					EM_error(a->u.array.size->pos,
						"type error: %s for size expression; int required",
						Ty_ToString(size.ty));
				if (!is_equal_ty(typ->u.array, init.ty))
					EM_error(a->u.array.init->pos, 
						"type error: %s for initialisation expression; %s required",
						Ty_ToString(init.ty), Ty_ToString(typ->u.array));
				return expTy(Tr_arrayExp(size.exp, init.exp), typ);
			}
		}
	}
	assert(0);
}

static struct expty transVar(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)
{
	switch(v->kind) {
		case A_simpleVar:
		{
			E_enventry x = S_look(venv, v->u.simple);								// ��ֵ�ռ��в���
			Tr_exp translation = Tr_noExp();
			if (x && x->kind == E_varEntry) {
				translation = Tr_simpleVar(x->u.var.access, level);					// ���������������ȷ, ����TEMP����MEM
				return expTy(translation, actual_ty(x->u.var.ty));
			} else {																// ���򱨴�
				EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
				return expTy(translation, Ty_Int());
			}
		}
		case A_fieldVar:
		{
			struct expty e = transVar(level, venv, tenv, breakk, v->u.field.var);
			if (e.ty->kind != Ty_record) {													// ������Ͳ���Ty_record�򱨴�
				EM_error(v->u.field.var->pos, "not a record type");
			} else {
				/* Cycle through record field type list looking for field we want */
				Ty_fieldList f;
				int fieldOffset = 0;
				for (f = e.ty->u.record; f; f = f->tail, fieldOffset++) {
					if (f->head->name == v->u.field.sym) {
						return expTy(Tr_fieldVar(e.exp, fieldOffset), actual_ty(f->head->ty));
					}
				}
				EM_error(v->pos, "no such field %s for record type", S_name(v->u.field.sym));
			}
			return expTy(Tr_noExp(), Ty_Int());
		}
		case A_subscriptVar:
		{
			struct expty e = transVar(level, venv, tenv, breakk, v->u.subscript.var);
			Tr_exp translation = Tr_noExp();
			if (e.ty->kind != Ty_array) {
				EM_error(v->u.subscript.var->pos, "not an array type");
				return expTy(translation, Ty_Int());
			} else {
				struct expty index = transExp(level, venv, tenv, breakk, v->u.subscript.exp);
				if (index.ty->kind != Ty_int) {
					EM_error(v->u.subscript.exp->pos, "integer required");
				} else {
					translation =  Tr_subscriptVar(e.exp, index.exp);
				}
				return expTy(translation, actual_ty(e.ty->u.array));
			}
		}
	}
	assert(0);
}


/*
 * To maintain the same order as in the params list passed in,
 * we keep a pointer to the last list in the list of parameters;
 * this is where we insert every parameter type (by constructing a
 * type list with the currenct type).
 */
static Ty_tyList makeFormalTys(S_table tenv, A_fieldList params)			// �������Ĳ����б����ʹ�һ��, A_fieldListתΪTy_tyList
{
	Ty_tyList paramTys = NULL;
	Ty_tyList tailList = paramTys;
	A_fieldList paramList;
	for (paramList = params; paramList; paramList = paramList->tail) {		// ���ÿ�������������Ƿ��Ѷ���
		Ty_ty t = S_look_ty(tenv, paramList->head->typ);
		if (!t) {
			EM_error(paramList->head->pos, "undefined type %s",
				S_name(paramList->head->typ));
		} else {
			if (paramTys) {													// ֮���һ��һ������
				tailList->tail = Ty_TyList(t, NULL);
				tailList = tailList->tail;
			} else {														// ���ǵ�һ�γ�ʼ��tailList
				paramTys = Ty_TyList(t, NULL);
				tailList = paramTys;
			}
		}
	}
	return paramTys;
}

/*
 * Creates a list of booleans signifying whether the parameter escapes
 * For the moment the escape field of each parameter is ignored and the list
 * will contain all true values i.e. all parameters escape into the frame.
 * This will be altered later.
 */
static U_boolList makeFormals(A_fieldList params)
{
	U_boolList formalsList = NULL, tailList = NULL;
	A_fieldList paramList;
	for (paramList = params; paramList; paramList = paramList->tail) {
		if (formalsList) {
			tailList->tail = U_BoolList(TRUE, NULL);
			tailList = tailList->tail;
		} else {
			formalsList = U_BoolList(TRUE, NULL);
			tailList = formalsList;
		}
	}
	return formalsList;
}


static Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	switch (d->kind) {
		case A_functionDec:
		{
			A_fundecList funList;
			Ty_tyList formalTys;
			U_boolList formals;
			Ty_ty resultTy;
			for (funList = d->u.function; funList; funList = funList->tail) {				// ����A_letExp�е�ÿһ������
				resultTy = NULL;
				if (funList->head->result) {
					resultTy = S_look(tenv, funList->head->result);							// ��鷵�������Ƿ��Ѷ���
					if (!resultTy) {
						EM_error(funList->head->pos, "undefined type for return type");
					}
				}
				if (!resultTy) resultTy = Ty_Void();
				
				formalTys = makeFormalTys(tenv, funList->head->params);						// ����������Ty_tyList
				formals = makeFormals(funList->head->params);
				Temp_label funLabel = Temp_newlabel();
				Tr_level funLevel = Tr_newLevel(level, funLabel, formals);
				S_enter(venv, funList->head->name,
					E_FunEntry(funLevel, funLabel, formalTys, resultTy));					// �������б���Ϊһ��record���ʹ�����
			}
			/* Now process the function bodies */
			E_enventry funEntry = NULL;
			for (funList = d->u.function; funList; funList = funList->tail) {				// ����A_letExp�е�ÿһ������
				/* Get formal types list from funEntry */
				funEntry = S_look(venv, funList->head->name);
				S_beginScope(venv);															// �Ժ����ڲ������µ�������
				/* Add formal parameters to value environment
				 * using the type list in the function entry and the params field list in
				 * the function abstract declaration */
				Ty_tyList paramTys = funEntry->u.fun.formals;
				A_fieldList paramFields;
				Tr_accessList accessList = Tr_formals(funEntry->u.fun.level);
				for (paramFields = funList->head->params; paramFields;
						paramFields = paramFields->tail, paramTys = paramTys->tail,
						accessList = accessList->tail) {									// �����β�
					S_enter(venv, paramFields->head->name,
						E_VarEntry(accessList->head, paramTys->head));						// ���βμ���ֵ�ռ�
				}
				struct expty e = transExp(funEntry->u.fun.level, venv, tenv,
					breakk, funList->head->body);											// ���뺯����
				if (!is_equal_ty(funEntry->u.fun.result, e.ty))								// ���������ķ������ͺͺ����ķ������Ͳ�һ���򱨴�
					EM_error(funList->head->body->pos, "incorrect return type %s; expected %s",
						Ty_ToString(e.ty), Ty_ToString(funEntry->u.fun.result));
				
				Tr_procEntryExit(funEntry->u.fun.level, e.exp, e.ty->kind == Ty_void);		
				S_endScope(venv);															// ����������
			}
			return Tr_noExp();
		}
		case A_varDec:	// �������
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
			} else {																	// ���û����ʽ��������
				if (e.ty->kind == Ty_nil)												// �����ʼ��������
					EM_error(d->u.var.init->pos, "illegal use nil expression");
				S_enter(venv, d->u.var.var, E_VarEntry(access, e.ty));					// �����ʼ�������;��ó�ʼ��������, ����ֵ�ռ�
			}
			return Tr_assignExp(Tr_simpleVar(access, level), e.exp);					// ����IR��
		}
		case A_typeDec:
		{
			A_nametyList nameList;
			bool isCyclic = TRUE; /* Illegal cycle in type list */
			/* "headers" first */
			for (nameList = d->u.type; nameList; nameList = nameList->tail)					// ����ÿ����������
				S_enter(tenv, nameList->head->name, Ty_Name(nameList->head->name, NULL));	// ��ÿ���������ͼ������Ϳռ�
			/* now we can process the (possibly mutually) recursive bodies */
			for (nameList = d->u.type; nameList; nameList = nameList->tail) {				// ����ÿ����������
				Ty_ty t = transTy(tenv, nameList->head->ty);
				if (isCyclic) {
					if (t->kind != Ty_name) isCyclic = FALSE;
				}
				/* Update type binding */
				Ty_ty nameTy = S_look(tenv, nameList->head->name);
				nameTy->u.name.ty = t;
			}
			if (isCyclic)																	// ���ѭ�������򱨴�
				EM_error(d->pos, "illegal type cycle: cycle must contain record, array or built-in type");
			return Tr_noExp();
		}
	}
	assert(0);
}

static Ty_fieldList makeFieldTys(S_table tenv, A_fieldList fields)			// ��A_fieldListת��Ty_fieldList
{
	Ty_fieldList fieldTys = NULL;
	Ty_fieldList tailList = fieldTys;
	A_fieldList fieldList;
	for (fieldList = fields; fieldList; fieldList = fieldList->tail) {		// ����ÿһ������
		Ty_ty t = S_look(tenv, fieldList->head->typ);
		if (!t) {															// �����ֵ�ռ����Ҳ����ͱ���
			EM_error(fieldList->head->pos, "undefined type %s",
				S_name(fieldList->head->typ));
		} else {
			Ty_field f = Ty_Field(fieldList->head->name, t);
			if (fieldTys) {													// һ���������
				tailList->tail = Ty_FieldList(f, NULL);
				tailList = tailList->tail;
			} else {														// ��ʼ��
				fieldTys = Ty_FieldList(f, NULL);
				tailList = fieldTys;
			}
		}
	}
	return fieldTys;
}


static Ty_ty transTy(S_table tenv, A_ty t)
{
	Ty_ty ty;
	switch (t->kind) {
		case A_nameTy:
		{
			ty = S_look(tenv, t->u.name);
			if (!ty) {
				EM_error(t->pos, "undefined type %s",
					S_name(t->u.name));
			}
			return ty;
		}
		case A_recordTy:
		{
			Ty_fieldList fieldTys = makeFieldTys(tenv, t->u.record);
			return Ty_Record(fieldTys);
		}
		case A_arrayTy:
		{
			ty = S_look(tenv, t->u.name);
			if (!ty) {
				EM_error(t->pos, "undefined type %s",
					S_name(t->u.name));
			}
			return Ty_Array(ty);
		}
	}
	assert(0);
}
