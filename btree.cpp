#include "btree.hpp"
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>

// Constructors
BTree::BTree(int order) {
  this->order = order;
  this->itemCount = 0;
  this->root = nullptr;
}

// Destructors
BTree::~BTree() { delete root; }

void BTreeNode::print() {
  std::cout << (isLeaf() ? "leaf" : "node") << " [";
  for (int i = 0; i < count - 1; i++) {
    std::cout << keys[i] << ", ";
  }
  std::cout << keys[count - 1] << "]";
}

void BTree::printTree() {
  if (root == nullptr) {
    std::cout << "Tree empty\n";
    return;
  }
  int layer = 1;
  std::queue<BTreeNode *> q;
  q.push(root);
  while (!q.empty()) {
    std::cout << "\tLayer " << layer << ": ";
    int len = q.size();
    for (int i = 0; i < len; i++) {
      BTreeNode *cur = q.front();
      q.pop();
      cur->print();
      std::cout << "  ";
      if (!cur->isLeaf()) {
        InternalNode *node = static_cast<InternalNode *>(cur);
        for (int j = 0; j <= node->count; j++)
          q.push(node->children[j]);
      }
    }
    std::cout << "\n";
    layer++;
  }
}

// Inesrtion
void LeafNode::insertNotFull(int k, std::string value) {
  if (count == 2 * order + 1) {
    throw std::runtime_error(
        "tree leaf is order*2+1 before insert, but it should never reach this");
  }
  int i;
  for (i = count - 1; i >= 0 && keys[i] > k; i--)
    keys[i + 1] = keys[i];
  keys[i + 1] = k;
  data[i + 1] = value;
  count++;
}

void InternalNode::splitChild(int index) {
  BTreeNode *child = children[index];
  if (child->count <= order * 2) {
    throw std::runtime_error("child is not full");
  }

  if (child->isLeaf()) {
    LeafNode *leaf = static_cast<LeafNode *>(child);
    LeafNode *newChild = new LeafNode(order);
    newChild->right = leaf->right;
    newChild->left = leaf;
    leaf->right = newChild;
    for (int i = 0; i <= order; i++) {
      newChild->keys[i] = leaf->keys[i + order];
      newChild->data[i + order] = leaf->data[i + order];
      leaf->keys[i + order] = 0;
      leaf->data[i + order] = "";
    }
    leaf->count = order;
    newChild->count = order + 1;
    int newKey = newChild->keys[0];

    int j;
    for (j = count - 1; j >= index; j--) {
      keys[j + 1] = keys[j];
      children[j + 2] = children[j + 1];
    }
    keys[j + 1] = newKey;
    children[j + 2] = newChild;
    count++;
    return;
  }

  InternalNode *node = static_cast<InternalNode *>(child);
  InternalNode *newNode = new InternalNode(order);
  for (int i = 0; i < order; i++) {
    //  [0, 1, (), 3, 4]
    // [0, 1, 2, 3, 4, 5]
    newNode->keys[i] = node->keys[i + order + 1];
    node->keys[i + order + 1] = 0;
    newNode->children[i] = node->children[i + order + 1];
    node->children[i + order + 1] = nullptr;
  }
  newNode->children[order] = node->children[2 * order + 1];
  node->children[2 * order + 1] = nullptr;
  newNode->count = order;
  node->count = order;

  /*std::cout << "===== Splitting an internal node =====\n";*/
  /*node->print();*/
  /*std::cout << " ";*/
  /*newNode->print();*/
  /*std::cout << "\n";*/
  /*std::cout << "new key: " << node->keys[order] << "\n";*/

  int j;
  for (j = count - 1; j >= index; j--) {
    keys[j + 1] = keys[j];
    children[j + 2] = children[j + 1];
  }
  keys[j + 1] = node->keys[order];
  node->keys[order] = 0;
  children[j + 2] = newNode;
  count++;
  return;

  throw std::runtime_error("Haven't implemented lol");
}

void InternalNode::insertNotFull(int key, std::string data) {
  int i;
  for (i = 0; i < count && key > keys[i]; i++)
    ;
  BTreeNode *child = children[i];
  child->insertNotFull(key, data);
  if (child->count == order * 2 + 1)
    splitChild(i);
}

void BTree::insert(int key, std::string data) {
  if (root == nullptr) {
    root = new LeafNode(order);
  }

  root->insertNotFull(key, data);
  itemCount++;

  if (root->count < order * 2 + 1)
    return;

  InternalNode *newRoot = new InternalNode(order);
  newRoot->children[0] = root;
  newRoot->splitChild(0);
  root = newRoot;
  return;
}
