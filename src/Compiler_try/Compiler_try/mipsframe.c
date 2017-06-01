#include "frame.h"

//mark
struct F_access_ {
	enum { inFrame, inReg } kind;
	union {
		int offset; /* inFrame */
		Temp_temp reg; /* inReg */
	} u;
};

//mark
const int F_WORD_SIZE = 4; // Stack grows to lower address (32-bit machine - 4 bytes)
static const int F_PRAM_NUM = 4; // Number of parameters kept in registers
struct F_frame_ {
	Temp_label name;
	F_accessList formals;
	int alloc_offset;
	T_stmList argList;
};

static F_access InFrame(int offset);
static F_access InReg(Temp_temp reg);
static F_accessList F_AccessList(F_access head, F_accessList tail);
static F_accessList makeFormalAccessList(F_frame f, U_boolList formals);
static Temp_tempList F_get_arg_register(void);

//mark
static F_accessList F_AccessList(F_access head, F_accessList tail)
{
	F_accessList list = checked_malloc(sizeof(*list));
	list->head = head;
	list->tail = tail;
	return list;
}

//mark
static F_accessList makeFormalAccessList(F_frame f, U_boolList formals)
{
	U_boolList p;
	F_accessList headList = NULL, tailList = NULL;
	Temp_tempList argRegs = F_get_arg_register();
	f->argList = NULL;
	for (p = formals; p; p = p->tail, argRegs = argRegs->tail) {
		F_access access = F_allocLocal(f, p->head);
		if (headList) {
			tailList->tail = F_AccessList(access, NULL);
			tailList = tailList->tail;
		}
		else {
			headList = F_AccessList(access, NULL);
			tailList = headList;
		}
		//store reg arguments into frame
		if (argRegs != NULL) {
			f->argList = T_StmList(T_Move(F_Exp(access, T_Temp(F_FP())), T_Temp(argRegs->head)), f->argList);
		}
	}
	return headList;
}

//mark
static F_access InFrame(int offset)
{
	F_access access = checked_malloc(sizeof(*access));
	access->kind = inFrame;
	access->u.offset = offset;
	return access;
}

//mark
static F_access InReg(Temp_temp reg)
{
	F_access access = checked_malloc(sizeof(*access));
	access->kind = inReg;
	access->u.reg = reg;
	return access;
}


//mark
F_frame F_newFrame(Temp_label name, U_boolList formals)
{
	F_frame frame = checked_malloc(sizeof(*frame));
	frame->name = name;
	frame->alloc_offset = 0;
	frame->formals = makeFormalAccessList(frame, formals);
	return frame;
}

//mark
Temp_label F_name(F_frame f)
{
	return f->name;
}

//mark
F_accessList F_formals(F_frame f)
{
	return f->formals;
}

//mark
F_access F_allocLocal(F_frame f, bool escape)
{
	f->alloc_offset -= F_WORD_SIZE;
	if (escape) return InFrame(f->alloc_offset);
	return InReg(Temp_newtemp());
}

F_frag F_StringFrag(Temp_label label, string str)
{
	F_frag strfrag = checked_malloc(sizeof(*strfrag));
	strfrag->kind = F_stringFrag;
	strfrag->u.stringg.label = label;
	strfrag->u.stringg.str = str;
	return strfrag;
}

F_frag F_ProcFrag(T_stm body, F_frame frame)
{
	F_frag pfrag = checked_malloc(sizeof(*pfrag));
	pfrag->kind = F_procFrag;
	pfrag->u.proc.body = body;
	pfrag->u.proc.frame = frame;
	return pfrag;
}

F_fragList F_FragList(F_frag head, F_fragList tail)
{
	F_fragList fraglist = checked_malloc(sizeof(*fraglist));
	fraglist->head = head;
	fraglist->tail = tail;
	return fraglist;
}

// the following function are used for "view shift"
T_exp F_Exp(F_access access, T_exp framePtr)
{
	if (access->kind == inFrame) {
		return T_Mem(T_Binop(T_plus, framePtr, T_Const(access->u.offset)));
	}
	else {
		return T_Temp(access->u.reg);
	}
}
static T_exp F_Exp_from_sp(F_frame frame, F_access access, T_exp stackPtr) {
	if (access->kind == inFrame) {
		return T_Mem(T_Binop(T_plus, stackPtr, T_Const(access->u.offset - frame->alloc_offset - F_WORD_SIZE)));
	}
	else {
		return T_Temp(access->u.reg);
	}
}

static Temp_tempList F_get_callee_saves(void);
T_stm F_procEntryExit1(F_frame frame, T_stm stm)
{
	//计算新sp->保存原fp->计算新fp->保存 ra->保存Callee - save寄存器->保存参数->(函数体原指令)->恢复Callee - save寄存器->恢复返回地址->恢复fp->恢复sp
	T_stmList t = frame->argList;
	for (; t != NULL; t = t->tail) {
		stm = T_Seq(t->head, stm);
	}
	//在栈帧中生成存放fp和ra的空间
	F_access fpAcc = F_allocLocal(frame, TRUE);
	F_access raAcc = F_allocLocal(frame, TRUE);

	//保存callee_save寄存器
	Temp_tempList callee_save = F_get_callee_saves();
	for (; callee_save; callee_save = callee_save->tail) {
		F_access callee_acc = F_allocLocal(frame, 1);
		stm = T_Seq(T_Move(F_Exp(callee_acc, T_Temp(F_FP())), T_Temp(callee_save->head)), stm);
	}

	//保存ra
	stm = T_Seq(T_Move(F_Exp(raAcc, T_Temp(F_FP())), T_Temp(F_RA())), stm);
	//计算新的fp fp = sp-栈空间+4
	stm = T_Seq(T_Move(T_Temp(F_FP()), T_Binop(T_plus, T_Temp(F_SP()), T_Const(-frame->alloc_offset - F_WORD_SIZE))), stm);
	//保存fp的值
	stm = T_Seq(T_Move(F_Exp_from_sp(frame, fpAcc, T_Temp(F_SP())), T_Temp(F_FP())), stm);
	//计算sp的值
	stm = T_Seq(T_Move(T_Temp(F_SP()), T_Binop(T_plus, T_Temp(F_SP()), T_Const(frame->alloc_offset))), stm);

	//执行body后恢复callee_saves
	callee_save = F_get_callee_saves();
	for (; callee_save; callee_save = callee_save->tail) {
		F_access callee_acc = F_allocLocal(frame, 1);
		stm = T_Seq(stm, T_Move(T_Temp(callee_save->head), F_Exp(callee_acc, T_Temp(F_FP()))));
	}
	//恢复ra
	stm = T_Seq(stm, T_Move(T_Temp(F_RA()), F_Exp(raAcc, T_Temp(F_FP()))));
	//恢复fp
	stm = T_Seq(stm, T_Move(T_Temp(F_FP()), F_Exp_from_sp(frame, fpAcc, T_Temp(F_SP()))));
	//恢复sp
	stm = T_Seq(stm, T_Move(T_Temp(F_SP()), T_Binop(T_minus, T_Temp(F_SP()), T_Const(frame->alloc_offset))));
	return stm;
}

//handle externalCall
T_exp F_externalCall(string str, T_expList args)
{
	return T_Call(T_Name(Temp_namedlabel(str)), args);
}

/* the following functions are used to 
   map and get different kinds of registers
*/
Temp_map F_tempMap = NULL;
static void F_add_to_map(string str, Temp_temp temp)
{
	if (!F_tempMap) {
		F_tempMap = Temp_name();
	}
	Temp_enter(F_tempMap, temp, str);
}

static Temp_tempList F_get_arg_register(void) {
	static Temp_tempList arg_list = NULL;
	if (!arg_list) {
		Temp_temp a0 = Temp_newtemp(), a1 = Temp_newtemp(),
			a2 = Temp_newtemp(), a3 = Temp_newtemp();
		F_add_to_map("$a0", a0); F_add_to_map("$a1", a1);
		F_add_to_map("$a2", a2); F_add_to_map("$a3", a3);
		arg_list = TL(a0, TL(a1, TL(a2, TL(a3, NULL))));
	}
	return arg_list;
}

static Temp_tempList F_get_callee_saves(void) {
	static Temp_tempList callee_saves = NULL;
	if (!callee_saves) {
		Temp_temp t0 = Temp_newtemp(), t1 = Temp_newtemp(),
			t2 = Temp_newtemp(), t3 = Temp_newtemp(),
			t4 = Temp_newtemp(), t5 = Temp_newtemp(),
			t6 = Temp_newtemp(), t7 = Temp_newtemp(),
			t8 = Temp_newtemp(), t9 = Temp_newtemp();

		F_add_to_map("$t0", t0); F_add_to_map("$t1", t1);
		F_add_to_map("$t2", t2); F_add_to_map("$t3", t3);
		F_add_to_map("$t4", t4); F_add_to_map("$t5", t5);
		F_add_to_map("$t6", t6); F_add_to_map("$t7", t7);

		callee_saves = TL(t0, TL(t1, TL(t2, TL(t3, TL(t4, TL(t5, TL(t6, TL(t7, NULL))))))));
	}
	return callee_saves;
}

static Temp_tempList F_get_caller_saves(void) {
	static Temp_tempList caller_saves = NULL;
	if (!caller_saves) {
		Temp_temp s0 = Temp_newtemp(), s1 = Temp_newtemp(),
			s2 = Temp_newtemp(), s3 = Temp_newtemp(),
			s4 = Temp_newtemp(), s5 = Temp_newtemp(),
			s6 = Temp_newtemp(), s7 = Temp_newtemp();

		F_add_to_map("$s0", s0); F_add_to_map("$s1", s1);
		F_add_to_map("$s2", s2); F_add_to_map("$s3", s3);
		F_add_to_map("$s4", s4); F_add_to_map("$s5", s5);
		F_add_to_map("$s6", s6); F_add_to_map("$s7", s6);

		caller_saves = TL(s0, TL(s1, TL(s2, TL(s3, TL(s4, TL(s5, TL(s6, TL(s7, NULL))))))));
	}
	return caller_saves;
}

static Temp_temp F_ZERO(void) {
	static Temp_temp zero = NULL;
	if (!zero) {
		zero = Temp_newtemp();
		F_add_to_map("$zero", zero);
	}
	return zero;
}

static Temp_temp fp = NULL;
Temp_temp F_FP(void)
{
	if (!fp) {
		fp = Temp_newtemp();
		F_add_to_map("$fp", fp);
	}
	return fp;
}

static Temp_temp sp = NULL;
Temp_temp F_SP(void)
{
	if (!sp) {
		sp = Temp_newtemp();
		F_add_to_map("$sp", sp);
	}
	return sp;
}

static Temp_temp ra = NULL;
Temp_temp F_RA(void)
{
	if (!ra) {
		ra = Temp_newtemp();
		F_add_to_map("$ra", ra);
	}
	return ra;
}

static Temp_temp rv = NULL;
Temp_temp F_RV(void) {
	if (!rv) {
		Temp_temp rv = Temp_newtemp();
		F_add_to_map("$v0", rv);
	}
	return rv;
}

Temp_tempList F_registers(void) {
	return Temp_TempList_join(F_get_callee_saves(), F_get_caller_saves());
}
