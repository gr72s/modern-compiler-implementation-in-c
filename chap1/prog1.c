#include "prog1.h"
#include <stdio.h>

A_stm prog(void) {
  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b",
                      A_EseqExp(A_PrintStm(A_PairExpList(
                                    A_IdExp("a"),
                                    A_LastExpList(A_OpExp(A_IdExp("a"), A_minus,
                                                          A_NumExp(1))))),
                                A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
          A_PrintStm(A_LastExpList(A_IdExp("b")))));
}

A_stm prog2(void) {
  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b",
                      A_EseqExp(A_PrintStm(A_PairExpList(
                                    A_IdExp("a"),
                                    A_LastExpList(A_OpExp(A_IdExp("a"), A_minus,
                                                          A_NumExp(1))))),
                                A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
          A_PrintStm(A_LastExpList(A_EseqExp(
              A_PrintStm(A_LastExpList(A_IdExp("d"))), A_IdExp("c"))))));
}

A_stm prog3(void) {
  return A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3)));
}

#define max(a, b) ((a > b) ? a : b)

static int countargs(A_expList expList) {
  return expList->kind == A_pairExpList ? 1 + countargs(expList->u.pair.tail)
                                        : 1;
}

static int expargs(A_exp exp) {
  switch (exp->kind) {
    case A_eseqExp:
      return max(maxargs(exp->u.eseq.stm), expargs(exp->u.eseq.exp));
    case A_opExp:
      return max(expargs(exp->u.op.left), expargs(exp->u.op.right));
    case A_idExp:
    case A_numExp:
      return 1;
    default:
      return 0;
  }
}

static int explistargs(A_expList explist) {
  switch (explist->kind) {
    case A_pairExpList:
      return max(expargs(explist->u.pair.head),
                 explistargs(explist->u.pair.tail));
    case A_lastExpList:
      return expargs(explist->u.last);
    default:
      return 0;
  }
}

int maxargs(A_stm stm) {
  switch (stm->kind) {
    case A_printStm:
      return max(explistargs(stm->u.print.exps), countargs(stm->u.print.exps));
    case A_compoundStm:
      return max(maxargs(stm->u.compound.stm1), maxargs(stm->u.compound.stm2));
    case A_assignStm: {
      return expargs(stm->u.assign.exp);
    }
    default:
      return 0;
  }
}

//------------------------------------------------------------

static int opresult(A_binop op, int i, int j);

IntAndTable_ A_IntAndTable(int i, Table_ t) {
  IntAndTable_ table = checked_malloc(sizeof(*table));
  table->i = i;
  table->t = t;
  return table;
}

Table_ Table(string id, int value, Table_ tail) {
  Table_ t = checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

void interp(A_stm stm) {
  interpStm(stm, NULL);
}

Table_ interpStm(A_stm s, Table_ t) {
  switch (s->kind) {
    case A_compoundStm:
      t = interpStm(s->u.compound.stm1, t);
      t = interpStm(s->u.compound.stm2, t);
      break;
    case A_assignStm: {
      IntAndTable_ table =
          interpExp(s->u.assign.exp, t);       // calculation expression
      t = update(t, s->u.assign.id, table->i); // update symbol table
      break;
    }
    case A_printStm: {
      IntAndTable_ table = interpPrint(s->u.print.exps, t);
      t = table->t;
      break;
    }
    default:
      break;
  }
  return t;
}

IntAndTable_ interpExp(A_exp e, Table_ t) {
  IntAndTable_ table = A_IntAndTable(0, t);
  switch (e->kind) {
    case A_numExp:
      table->i = e->u.num;
      break;
    case A_idExp:
      table->i = lookup(t, e->u.id);
      break;
    case A_opExp:
      table = interpExp(e->u.op.left, t);
      int i = table->i;
      table = interpExp(e->u.op.right, table->t);
      int j = table->i;
      table->i = opresult(e->u.op.oper, i, j);
      break;
    case A_eseqExp:
      t = interpStm(e->u.eseq.stm, t);
      table = interpExp(e->u.eseq.exp, t);
      break;
    default:
      break;
  }
  return table;
}

IntAndTable_ interpPrint(A_expList explist, Table_ t) {
  IntAndTable_ table = A_IntAndTable(0, t);
  switch (explist->kind) {
    case A_lastExpList:
      table = interpExp(explist->u.last, table->t);
      printf("%d\n", table->i); // break line when print end element
      return table;
    case A_pairExpList:
      table = interpExp(explist->u.pair.head, table->t);
      printf("%d ", table->i);
      table = interpPrint(explist->u.pair.tail, table->t);
      break;
    default:
      break;
  }
  return table;
}

// tail recursion, find value by id
int lookup(Table_ t, string id) {
  return t->id == id ? t->value : t->tail == NULL ? 0 : lookup(t->tail, id);
}

// create new Table
Table_ update(Table_ t, string id, int value) {
  return Table(id, value, t);
}

static int opresult(A_binop op, int i, int j) {
  switch (op) {
    case A_plus:
      return i + j;
    case A_minus:
      return i - j;
    case A_times:
      return i * j;
    case A_div:
      return i / j;
    default:
      break;
  }
}