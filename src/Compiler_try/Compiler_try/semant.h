#ifndef TIGER_SEMANT_H_
#define TIGER_SEMANT_H_

#include "absyn.h"
#include "types.h"
#include "translate.h"

struct expty {Tr_exp exp; Ty_ty ty;};

void SEM_transProg(A_exp root);

#endif /* TIGER_SEMANT_H_ */
