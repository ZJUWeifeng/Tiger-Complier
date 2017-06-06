#ifndef TIGER_PRINTTREE_H_
#define TIGER_PRINTTREE_H_

/* function prototype from printtree.c */
void printStmList(FILE *out, T_stmList stmList);
void pr_tree_exp(FILE *out, T_exp exp, int d);
void pr_stm(FILE *out, T_stm stm, int d);
void json_stm_list(FILE *out, T_stmList stmList);
void json_stm_init(FILE *out, T_stm stm);
void json_stm(FILE *out, T_stm stm, int d);
void json_tree_exp_init(FILE *out, T_stm stm);
void json_tree_exp(FILE *out, T_exp exp, int d);
#endif /* TIGER_PRINTTREE_H_ */


