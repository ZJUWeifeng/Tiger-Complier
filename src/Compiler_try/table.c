/*
 * table.c - Functions to manipulate generic tables.
 * Copyright (c) 1997 Andrew W. Appel.
 */

#include <stdio.h>
#include "util.h"
#include "table.h"

#define TABSIZE 127

typedef struct binder_ *binder;
struct binder_ { void *key; void *value; binder next; void *prevtop; };
struct TAB_table_ 
{
	binder table[TABSIZE];
	void *top;
};


static binder Binder(void *key, void *value, binder next, void *prevtop)
{
	binder b = checked_malloc(sizeof(*b));
	b->key = key; b->value = value; b->next = next; b->prevtop = prevtop;
	return b;
}

TAB_table TAB_empty(void)						// 初始化表格
{
	TAB_table t = checked_malloc(sizeof(*t));
	int i;
	t->top = NULL;
	for (i = 0; i < TABSIZE; i++)
		t->table[i] = NULL;
	return t;
}

/* The cast from pointer to integer in the expression
 *   ((unsigned)key) % TABSIZE
 * may lead to a warning message.  However, the code is safe,
 * and will still operate correctly.  This line is just hashing
 * a pointer value into an integer value, and no matter how the
 * conversion is done, as long as it is done consistently, a
 * reasonable and repeatable index into the table will result.
 */

void TAB_enter(TAB_table t, void *key, void *value)						// 加入值
{
	int index;
	assert(t && key);
	index = ((unsigned)key) % TABSIZE;									// 求hash值
	t->table[index] = Binder(key, value, t->table[index], t->top);		// 在table[index]处产生一个链表，之前的值被移到了链表的下一位. 之前的top也被保存了下来
	t->top = key;
}

void *TAB_look(TAB_table t, void *key)									// 查找值
{
	int index;
	binder b;
	assert(t && key);
	index = ((unsigned)key) % TABSIZE;									// 求hash值
	for (b = t->table[index]; b; b = b->next)
		if (b->key == key) return b->value;
	return NULL;
}

void *TAB_pop(TAB_table t)					// 弹出t表格最新的值
{
	void *k; binder b; int index;
	assert(t);
	k = t->top;
	assert(k);
	index = ((unsigned)k) % TABSIZE;
	b = t->table[index];
	assert(b);
	t->table[index] = b->next;				// 删除
	t->top = b->prevtop;					// 删除
	return b->key;
}

void TAB_dump(TAB_table t, void(*show)(void *key, void *value)) // 删除整个表格t
{
	void *k = t->top;
	int index = ((unsigned)k) % TABSIZE;
	binder b = t->table[index];
	if (b == NULL) return;
	t->table[index] = b->next;									// 删除最新的值
	t->top = b->prevtop;										// 删除最新的值
	show(b->key, b->value);										// 显示删除的内容
	TAB_dump(t, show);											// 递归地删除表格t
	assert(t->top == b->prevtop && t->table[index] == b->next);
	t->top = k;
	t->table[index] = b;
}
