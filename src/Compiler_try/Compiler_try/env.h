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

E_enventry E_VarEntry(Tr_access access, Ty_ty ty);	// 形成E_varEntry的格式
E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty result);		// 形成E_funEntry的格式

S_table E_base_tenv(void);	// 初始化类型空间(只有int和string), 类型空间中是名称+Ty_ty
S_table E_base_venv(void);	// 初始化值空间, 值空间中是名称+E_funEntry或E_varEntry

#endif /* TIGER_ENV_H_ */
