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

TAB_table TAB_empty(void)						// ��ʼ�����
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

void TAB_enter(TAB_table t, void *key, void *value)						// ����ֵ
{
	int index;
	assert(t && key);
	index = ((unsigned)key) % TABSIZE;									// ��hashֵ
	t->table[index] = Binder(key, value, t->table[index], t->top);		// ��table[index]������һ������֮ǰ��ֵ���Ƶ����������һλ. ֮ǰ��topҲ������������
	t->top = key;
}

void *TAB_look(TAB_table t, void *key)									// ����ֵ
{
	int index;
	binder b;
	assert(t && key);
	index = ((unsigned)key) % TABSIZE;									// ��hashֵ
	for (b = t->table[index]; b; b = b->next)
		if (b->key == key) return b->value;
	return NULL;
}

void *TAB_pop(TAB_table t)					// ����t������µ�ֵ
{
	void *k; binder b; int index;
	assert(t);
	k = t->top;
	assert(k);
	index = ((unsigned)k) % TABSIZE;
	b = t->table[index];
	assert(b);
	t->table[index] = b->next;				// ɾ��
	t->top = b->prevtop;					// ɾ��
	return b->key;
}

void TAB_dump(TAB_table t, void(*show)(void *key, void *value)) // ɾ���������t
{
	void *k = t->top;
	int index = ((unsigned)k) % TABSIZE;
	binder b = t->table[index];
	if (b == NULL) return;
	t->table[index] = b->next;									// ɾ�����µ�ֵ
	t->top = b->prevtop;										// ɾ�����µ�ֵ
	show(b->key, b->value);										// ��ʾɾ��������
	TAB_dump(t, show);											// �ݹ��ɾ�����t
	assert(t->top == b->prevtop && t->table[index] == b->next);
	t->top = k;
	t->table[index] = b;
}
