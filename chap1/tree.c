#include "tree.h"
#include <stddef.h>
#include <string.h>

T_tree Tree(T_tree l, string k, T_tree r) {
  T_tree t = checked_malloc(sizeof(*t));
  t->left = l;
  t->key = k;
  t->right = r;
  return t;
}

T_tree insert(string k, T_tree t) {
  if (t == NULL) {
    return Tree(NULL, k, NULL);
  } else if (strcmp(k, t->key) < 0) {
    return Tree(insert(k, t->left), t->key, t->right);
  } else if (strcmp(k, t->key) > 0) {
    return Tree(t->left, t->key, insert(k, t->right));
  } else {
    return Tree(t->left, k, t->right);
  }
}

//------------------------------------------------------------

// 1.1 a.

bool member(T_tree t, string k) {
  if (t == NULL)
    return FALSE;

  if (strcmp(t->key, k) < 0) {
    return member(t->right, k);
  } else if (strcmp(t->key, k) > 0) {
    return member(t->left, k);
  } else if (strcmp(t->key, k) == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

// 1.1 b. 
// 未实现，在T_tree中新增一个binding类型的成员变量
// 该成员含value成员变量，用于存储k
// insert与lookup只需要在判断时使用binding取出k即可