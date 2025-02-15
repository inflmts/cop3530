#include <iomanip>
#include "avl.h"

namespace avl {

/*******************************************************************************
 * Tree
 ******************************************************************************/

Tree::Tree() : root(nullptr) {}

Tree::~Tree()
{
  delete root;
}

unsigned int Tree::height()
{
  return root ? root->h + 1 : 0;
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

void Tree::remove()
{
  if (!root)
    return;

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
}

bool Tree::remove(int id)
{
  // looking in the wrong place
  if (!root)
    return false;

  // found it!
  if (id == root->id) {
    remove();
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
      remove();
      return;
    }
    root->right.remove_index_impl(i);
  }
}

bool Tree::remove_index(int i)
{
  ++i;
  Tree::remove_index_impl(i);
  return i < 0;
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

void Tree::dump(std::ostream& out)
{
  std::string prefix;
  dump(out, prefix, "    --");
}

void Tree::dump(std::ostream& out, std::string& prefix, const char *ch)
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
    root->h = lh;
    if (bf > 1) {
      // left-heavy, rotate right
      int lbf = L->left.height() - L->right.height();
      if (lbf < 0)
        L.rotate_left();
      rotate_right();
    }
  } else {
    root->h = rh;
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
  h(0),
  id(_id),
  name(_name)
{}

void Node::update_height()
{
  h = left ? (right && right->h > left->h ? right->h + 1 : left->h + 1) : 0;
}

}
