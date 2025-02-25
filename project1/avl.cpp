/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  avl.cpp
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *****************************************************************************/

#include <iomanip>
#include <limits.h>
#include "avl.h"

namespace avl {

/*******************************************************************************
 * Tree
 ******************************************************************************/

Tree::Tree() {}

Tree::Tree(const Tree& other)
{
  if (other.root)
    root = new Node(*other.root);
}

Tree::~Tree()
{
  delete root;
}

Node *Tree::insert(int id, const std::string& name)
{
  // this slot is available, put the new node here
  if (!root)
    return root = new Node(id, name);

  // duplicate
  if (id == root->id)
    return nullptr;

  Tree& child = id < root->id ? root->left : root->right;
  unsigned int h = child.height();
  // standard recursive BST insert
  Node *node = child.insert(id, name);
  // rebalance the tree if the child height changes
  if (child.height() != h)
    balance();
  return node;
}

bool Tree::pop()
{
  if (!root)
    return false;

  Node *replace;
  if (root->left) {
    if (root->right) {
      // fuck
      replace = root->right.detach_min();
      replace->left = root->left;
      replace->right = root->right;
    } else {
      // left child only
      replace = root->left;
    }
  } else if (root->right) {
    // right child only
    replace = root->right;
  } else {
    // poor childless leaf
    replace = nullptr;
  }
  root->left = nullptr;
  root->right = nullptr;
  delete root;
  root = replace;
  return true;
}

bool Tree::remove(int id)
{
  // looking in the wrong place
  if (!root)
    return false;

  // found it!
  if (id == root->id) {
    pop();
    return true;
  }

  // standard recursive BST delete
  Tree& child = id < root->id ? root->left : root->right;
  return child.remove(id);
}

void Tree::remove_index_impl(int& i)
{
  if (root) {
    root->left.remove_index_impl(i);
    if (i == 0) return;
    if (--i == 0) {
      pop();
      return;
    }
    root->right.remove_index_impl(i);
  }
}

bool Tree::remove_index(int i)
{
  ++i;
  Tree::remove_index_impl(i);
  return i == 0;
}

void Tree::clear()
{
  delete root;
  root = nullptr;
}

Node *Tree::get(int id)
{
  Node *node = root;
  while (node) {
    if (id == node->id)
      return node;
    else if (id < node->id)
      node = node->left;
    else
      node = node->right;
  }
  return nullptr;
}

std::vector<Node*> Tree::search(const std::string& name)
{
  std::vector<Node*> vec;
  search(name, vec);
  return vec;
}

void Tree::search(const std::string& name, std::vector<Node*>& vec)
{
  if (root) {
    root->left.search(name, vec);
    if (name == root->name)
      vec.push_back(root);
    root->right.search(name, vec);
  }
}

void Tree::dump(std::ostream& out) const
{
  std::string prefix;
  dump(out, prefix, "    --");
}

void Tree::dump(std::ostream& out, std::string& prefix, const char *ch) const
{
  if (!root)
    return;

  if (root->left) {
    prefix.append(ch, 2);
    root->left.dump(out, prefix, "  | ,-");
    prefix.resize(prefix.size() - 2);
  }
  out << prefix << ch + 4
      << std::setw(8) << std::setfill('0') << root->id
      << " " << root->name << " [" << root->h << "]\n";
  if (root->right) {
    prefix.append(ch + 2, 2);
    root->right.dump(out, prefix, "|   `-");
    prefix.resize(prefix.size() - 2);
  }
}

bool Tree::check() const
{
  return check(INT_MIN, INT_MAX);
}

bool Tree::check(int min, int max) const
{
  if (!root)
    return true;

  unsigned int lh = root->left.height();
  unsigned int rh = root->right.height();
  return (root->id >= min && root->id <= max
      && (lh > rh ? root->h == lh + 1 && lh - rh < 2
                  : root->h == rh + 1 && rh - lh < 2)
      && root->left.check(min, root->id - 1)
      && root->right.check(root->id + 1, max));
}

bool Tree::identical(const Tree& other) const
{
  return !root ? !other.root :
    root->id == other->id
    && root->left.identical(other->left)
    && root->right.identical(other->right);
}

// Iteration

std::vector<Node*> Tree::inorder()
{
  std::vector<Node*> vec;
  inorder(vec);
  return vec;
}

std::vector<Node*> Tree::preorder()
{
  std::vector<Node*> vec;
  preorder(vec);
  return vec;
}

std::vector<Node*> Tree::postorder()
{
  std::vector<Node*> vec;
  postorder(vec);
  return vec;
}

void Tree::inorder(std::vector<Node*>& vec)
{
  // LRN
  if (root) {
    root->left.inorder(vec);
    vec.push_back(root);
    root->right.inorder(vec);
  }
}

void Tree::preorder(std::vector<Node*>& vec)
{
  // NLR
  if (root) {
    vec.push_back(root);
    root->left.preorder(vec);
    root->right.preorder(vec);
  }
}

void Tree::postorder(std::vector<Node*>& vec)
{
  // LRN
  if (root) {
    root->left.postorder(vec);
    root->right.postorder(vec);
    vec.push_back(root);
  }
}

void Tree::rotate_left()
{
  Node *N = root;
  Node *R = N->right;
  Node *Z = R->left;
  /**********
  *  [N]    *
  *     R   *
  *   Z     *
  **********/
  root = R;
  /**********
  *   N     *
  *    [R]  *
  *   Z     *
  **********/
  N->right = Z;
  /**********
  *   N     *
  *     Z   *
  **********/
  R->left = N;
  /**********
  *    [R]  *
  *   N     *
  *     Z   *
  **********/
  N->update_height();
  R->update_height();
}

void Tree::rotate_right()
{
  Node *N = root;
  Node *L = N->left;
  Node *Z = L->right;
  root = L;
  N->left = Z;
  L->right = N;
  N->update_height();
  L->update_height();
}

void Tree::balance()
{
  Tree& L = root->left;
  Tree& R = root->right;
  unsigned int lh = L.height();
  unsigned int rh = R.height();
  int bf = (int)(lh) - rh;
  if (bf > 0) {
    root->h = lh + 1;
    if (bf > 1) {
      // left-heavy, rotate right
      int lbf = L->left.height() - L->right.height();
      if (lbf < 0)
        L.rotate_left();
      rotate_right();
    }
  } else {
    root->h = rh + 1;
    if (bf < -1) {
      // right-heavy, rotate left
      int rbf = R->left.height() - R->right.height();
      if (rbf > 0)
        R.rotate_right();
      rotate_left();
    }
  }
}

Node *Tree::detach_min()
{
  Node *node;
  if (!root->left) {
    node = root;
    root = node->right;
    node->right = nullptr;
    return node;
  }
  unsigned int h = root->left->h;
  node = root->left.detach_min();
  // rebalance tree if child height changes
  if (root->left.height() != h)
    balance();
  return node;
}

/*******************************************************************************
 * Node
 ******************************************************************************/

Node::Node(int _id, const std::string& _name) :
  h(1),
  id(_id),
  name(_name)
{}

Node::Node(const Node& other) :
  left(other.left),
  right(other.right),
  h(other.h),
  id(other.id),
  name(other.name)
{}

void Node::update_height()
{
  unsigned int lh = left.height();
  unsigned int rh = right.height();
  h = (lh > rh ? lh : rh) + 1;
}

}
