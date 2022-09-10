#ifndef CHAP1_PROG1_H
#define CHAP1_PROG1_H

#include "slp.h"

A_stm prog(void);
A_stm prog2(void);
A_stm prog3(void);

int maxargs(A_stm);

//------------------------------------------------------------

typedef struct table *Table_;
struct table {
  string id;
  int value;
  Table_ tail;
};

typedef struct IntAndTable *IntAndTable_;
struct IntAndTable {
  int i;
  Table_ t;
};

IntAndTable_ A_IntAndTable(int, Table_);
Table_ Table(string, int, Table_);
void interp(A_stm);
Table_ interpStm(A_stm s, Table_ t);
IntAndTable_ interpExp(A_exp e, Table_ t);
IntAndTable_ interpPrint(A_expList, Table_);
int lookup(Table_, string);
Table_ update(Table_, string, int);

#endif