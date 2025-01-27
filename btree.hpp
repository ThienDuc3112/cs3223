#ifndef BTREE_HPP
#define BTREE_HPP
#include <cstdlib>
#include <string>

class BTree;
class InternalNode;
class LeafNode;

class BTreeNode {
public:
  BTreeNode(int order) {
    this->keys = new int[order * 2 + 1];
    this->count = 0;
    this->order = order;
  };
  virtual ~BTreeNode() { delete[] this->keys; };
  virtual bool isLeaf() = 0;
  void print();
  friend class BTree;
  friend class InternalNode;
  friend class LeafNode;

protected:
  virtual void insertNotFull(int k, std::string data) = 0;
  int *keys;
  int count;
  int order;
};

class InternalNode : public BTreeNode {
public:
  InternalNode(int order) : BTreeNode(order) {
    this->children = new BTreeNode *[order * 2 + 2];
  }
  ~InternalNode() {
    for (int i = 0; i < count; ++i) {
      delete this->children[i];
    }
    delete[] this->children;
  };
  bool isLeaf() override { return false; };
  void splitChild(int);
  friend class BTree;

private:
  void insertNotFull(int k, std::string data) override;

  BTreeNode **children;
};

class LeafNode : public BTreeNode {
public:
  LeafNode(int order) : BTreeNode(order) {
    this->data = new std::string[order * 2 + 1];
    this->left = nullptr;
    this->right = nullptr;
  }
  ~LeafNode() { delete[] this->data; };
  bool isLeaf() override { return true; };
  friend class BTree;
  friend class InternalNode;

private:
  void insertNotFull(int k, std::string value) override;

  std::string *data;

  LeafNode *left;
  LeafNode *right;
};

class BTree {
public:
  BTree(int order);
  ~BTree();
  void insert(int key, std::string data);
  void printTree();
  int size() { return itemCount; }

private:
  BTreeNode *root;
  int order;
  int itemCount;
};

#endif // !BTREE_HPP
