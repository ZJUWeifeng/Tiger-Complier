#ifndef TIGER_ERRORMSG_H_
#define TIGER_ERRORMSG_H_

extern bool EM_fatalErrors;

typedef struct intList { int i; struct intList *rest; } *IntList;
void EM_newline(void);

extern int EM_tokPos;
extern int charPos;
void EM_parse_error(int, string);
void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);

#endif /* TIGER_ERRORMSG_H */
