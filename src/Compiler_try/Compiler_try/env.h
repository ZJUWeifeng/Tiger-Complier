#ifndef TIGER_ENV_H_
#define TIGER_ENV_H_

#include "types.h"
#include "translate.h"
#include "temp.h"
#include "symbol.h"

typedef struct E_enventry_ *E_enventry;

struct E_enventry_ {
	enum { E_varEntry, E_funEntry } kind;
	union {
		struct {
			Tr_access access;
			Ty_ty ty;
		} var;
		struct {
			Tr_level level;
			Temp_label label;
			Ty_tyList formals;
			Ty_ty result;
		} fun;
	} u;
};

E_enventry E_VarEntry(Tr_access access, Ty_ty ty);	// �γ�E_varEntry�ĸ�ʽ
E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty result);		// �γ�E_funEntry�ĸ�ʽ

S_table E_base_tenv(void);	// ��ʼ�����Ϳռ�(ֻ��int��string), ���Ϳռ���������+Ty_ty
S_table E_base_venv(void);	// ��ʼ��ֵ�ռ�, ֵ�ռ���������+E_funEntry��E_varEntry

#endif /* TIGER_ENV_H_ */
