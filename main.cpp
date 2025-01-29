#include "btree.hpp"
#include <iostream>

int main() {
  BTree *tree = new BTree(2);
  std::cout << "count: " << tree->size() << "\n";
  /*for (int i = 0; i < 37; i++) {*/
  /*for (int i = 0; i < 7; i++) {*/
  for (int i = 8; i >= 0; i--) {
    tree->insert(i, "Test");
    std::cout << "After inserting " << i << "\n";
    tree->printTree();
  }
}
