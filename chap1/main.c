#include "prog1.h"
#include "tree.h"
#include <stdio.h>

#define TEST_OK(a) assert((a) == TRUE)
#define TEST_FAIL(a) assert((a) == FALSE)

int main(int argc, char const *argv[]) {
  printf("program design\n");
  int args = maxargs(prog());
  printf("%d\n", args);
  interp(prog());
  printf("exercise\n");
  T_tree t = Tree(NULL, "d", NULL);
  t = insert("b", t);
  t = insert("c", t);
  t = insert("c", t);
  t = insert("e", t);
  TEST_OK(member(t, "c"));
  TEST_FAIL(member(t, "f"));
  return 0;
}
