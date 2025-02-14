#include "avl.h"

namespace avl {

bool match(Node *node, const MatchSpec *spec)
{
  return node
    ? (spec &&
        node->id == spec->id &&
        node->name == spec->name &&
        match(node->_left, spec->left) &&
        match(node->_right, spec->right))
    : !spec;
}

void inorder(Node *root, itercb cb)
{
  // LRN
  if (root) {
    inorder(root->_left, cb);
    cb(root);
    inorder(root->_right, cb);
  }
}

void preorder(Node *root, itercb cb)
{
  // NLR
  if (root) {
    cb(root);
    inorder(root->_left, cb);
    inorder(root->_right, cb);
  }
}

void postorder(Node *root, itercb cb)
{
  // LRN
  if (root) {
    inorder(root->_left, cb);
    inorder(root->_right, cb);
    cb(root);
  }
}

/*******************************************************************************
 * Node
 ******************************************************************************/

Node::Node(Node **s, int i, const std::string& n) :
  _slot(s),
  _left(nullptr),
  _right(nullptr),
  _h(0),
  id(i),
  name(n)
{}

Node::~Node()
{
  delete _left;
  delete _right;
}

void Node::rotate_left()
{
  Node *R = _right;
  Node *Z = _right->_left;
  // [N]   //
  //    R  //
  //  Z    //
  *_slot = R;
  R->_slot = _slot;
  //  N    //
  //   [R] //
  //  Z    //
  _right = R->_left;
  if (Z)
    Z->_slot = &_right;
  //  N    //
  //    Z  //
  R->_left = this;
  _slot = &R->_left;
  //   [R] //
  //  N    //
  //    Z  //
}

void Node::rotate_right()
{
  Node *L = _left;
  Node *Z = _left->_right;
  *_slot = L;
  L->_slot = _slot;
  _left = L->_right;
  if (Z)
    Z->_slot = &_left;
  L->_right = this;
  _slot = &L->_right;
}

void Node::balance()
{
  unsigned int lh = _left ? _left->_h + 1 : 0;
  unsigned int rh = _right ? _right->_h + 1 : 0;
  int bf = static_cast<int>(lh) - rh;
  if (bf > 0) {
    _h = lh;
    if (bf > 1) {
      // left-heavy, rotate right
      int lbf = (_left->_left ? _left->_left->_h + 1 : 0)
              - (_left->_right ? _left->_right->_h + 1 : 0);
      if (lbf < 0)
        _left->rotate_left();
      rotate_right();
    }
  } else {
    _h = rh;
    if (bf < 1) {
      // right-heavy, rotate left
      int rbf = (_right->_left ? _right->_left->_h + 1 : 0)
              - (_right->_right ? _right->_right->_h + 1 : 0);
      if (rbf > 0)
        _right->rotate_right();
      rotate_left();
    }
  }
}

Node *Node::insert(int id, const std::string& name)
{
  Node **slot;
  Node *node;
  if (id < this->id)
    slot = &_left;
  else if (id > this->id)
    slot = &_right;
  else
    return nullptr; // duplicate

  if (*slot) {
    unsigned int h = (*slot)->_h;
    node = (*slot)->insert(id, name);
    if ((*slot)->_h != h) {
      // child height changed
      balance();
    }
  } else {
    node = new Node(slot, id, name);
    *slot = node;
    // just update height -- this shouldn't destabilize the tree
    balance();
  }
  return node;
}

/*******************************************************************************
 * Tree
 ******************************************************************************/

Tree::Tree() :
  _root(nullptr)
{}

Tree::~Tree()
{
  delete _root;
}

unsigned int Tree::height()
{
  return _root ? _root->_h + 1 : 0;
}

void Tree::clear()
{
  delete _root;
}

Node *Tree::insert(int id, const std::string& name)
{
  if (_root)
    return _root->insert(id, name);
  else
    return _root = new Node(&_root, id, name);
}

bool Tree::remove(int id)
{
  return false;
}

bool Tree::remove_index(int i)
{
  return false;
}

Node *Tree::get(int id)
{
  Node *node = _root;
  while (node) {
    if (id == node->id)
      return node;
    else if (id < node->id)
      node = node->_left;
    else
      node = node->_right;
  }
  return nullptr;
}

std::vector<Node*> Tree::search(const std::string& name)
{
  std::vector<Node*> results;
  inorder([&] (Node *node) {
    if (node->name == name)
      results.push_back(node);
  });
  return results;
}

} // namespace avl
