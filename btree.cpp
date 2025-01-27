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
  }
}

// Inesrtion
void LeafNode::insertNotFull(int k, std::string value) {
  if (count == 2 * order) {
    throw std::runtime_error("tree node is full");
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
  if (child->count < order * 2) {
    throw std::runtime_error("child is not full");
  }

  if (child->isLeaf()) {
    LeafNode *leaf = static_cast<LeafNode *>(child);
    /*std::cout << "is leaf\n";*/
    LeafNode *newChild = new LeafNode(order);
    /*std::cout << "new child\n";*/
    newChild->right = leaf->right;
    newChild->left = leaf;
    leaf->right = newChild;
    /*std::cout << "relink pointer\n";*/
    for (int i = 0; i < order; i++) {
      /*std::cout << "num: " << i << "\n";*/
      newChild->keys[i] = leaf->keys[i + order];
      /*std::cout << "copied key: " << newChild->keys[i] << "\n";*/
      newChild->keys[i] = leaf->keys[i + order];
      /*std::cout << "copied data: " << newChild->data[i] << "\n";*/
      newChild->data[i + order] = leaf->data[i + order];
      leaf->keys[i + order] = 0;
      /*std::cout << "zeroed key\n";*/
      leaf->data[i + order] = "";
      /*std::cout << "zeroed data\n";*/
    }
    leaf->count = order;
    newChild->count = order;
    int newKey = newChild->keys[0];

    //  [0, 1, 2,   2.5  ,3, 4, 5]
    // [0, 1, 2, 3.25, 3.5, 4, 5, 6]
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

  throw std::runtime_error("Haven't implemented lol");
}

void InternalNode::insertNotFull(int key, std::string data) {
  int i;
  for (i = 0; i < count && key > keys[i]; i++)
    ;
  BTreeNode *child = children[i];
  if (child->count < order * 2) {
    child->insertNotFull(key, data);
    return;
  }

  splitChild(i);
  if (keys[i] < key)
    i++;
  children[i]->insertNotFull(key, data);
}

void BTree::insert(int key, std::string data) {
  if (root == nullptr) {
    root = new LeafNode(order);
  }

  if (root->count < order * 2) {
    root->insertNotFull(key, data);
    itemCount++;
    return;
  }

  if (root->isLeaf()) {
    InternalNode *newRoot = new InternalNode(order);
    newRoot->children[0] = root;
    newRoot->splitChild(0);
    int i = 0;
    if (newRoot->keys[0] < key)
      i++;
    newRoot->children[i]->insertNotFull(key, data);
    itemCount++;
    root = newRoot;
    return;
  }

  throw std::runtime_error("btree haven't implemented propogation lol");
}
