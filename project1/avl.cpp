#include <iomanip>
#include "avl.h"

avl::avl() : root(nullptr) {}
avl::avl(int id, const std::string& name) : root(new Node(id, name)) {}
avl::~avl() { delete root; }

void avl::rotate_left()
{
  avl N = root;
  avl R = N->_right;
  avl Z = R->_left;
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
  N->_right = Z;
  /**********
  *   N     *
  *     Z   *
  **********/
  R->_left = N;
  /**********
  *    [R]  *
  *   N     *
  *     Z   *
  **********/
  update_height(N);
  update_height(R);
}

void avl::rotate_right()
{
  avl N = root;
  avl L = N->_left;
  avl Z = L->_right;
  root = L;
  N->_left = Z;
  L->_right = N;
  update_height(N);
  update_height(L);
}

void avl::balance()
{
  avl L = root->_left;
  avl R = root->_right;
  unsigned int lh = L ? L->_h + 1 : 0;
  unsigned int rh = R ? R->_h + 1 : 0;
  int bf = static_cast<int>(lh) - rh;
  if (bf > 0) {
    root->_h = lh;
    if (bf > 1) {
      // left-heavy, rotate right
      int lbf = (L->_left ? L->_left->_h + 1 : 0)
              - (L->_right ? L->_right->_h + 1 : 0);
      if (lbf < 0)
        rotate_left(root->_left);
      rotate_right(root);
    }
  } else {
    root->_h = rh;
    if (bf < -1) {
      // right-heavy, rotate left
      int rbf = (R->_left ? R->_left->_h + 1 : 0)
              - (R->_right ? R->_right->_h + 1 : 0);
      if (rbf > 0)
        rotate_right(root->_right);
      rotate_left(root);
    }
  }
}

avl avl::detach_min()
{
  avl node;
  if (!root->_left) {
    node = root;
    root = node->_right;
    node->_right = nullptr;
    return node;
  }
  unsigned int h = root->_left->_h;
  node = detach_min(root->_left);
  // rebalance tree if child height changes
  if (!root->_left || root->_left->_h != h)
    balance(root);
  return node;
}

avl avl::insert(int id, const std::string& name)
{
  // this slot is available, put the new node here
  if (!root)
    return root = new avl(id, name);

  // duplicate
  if (id == root->id)
    return nullptr;

  avl &child = id < root->id ? root->_left : root->_right;
  unsigned int h = height(child);
  // standard recursive BST insert
  avl newnode = insert(child, id, name);
  // rebalance the tree if the child height changes
  if (height(child) != h)
    balance(root);
  return newnode;
}

void avl::remove()
{
  if (!root)
    return;

  avl replace;
  if (root->_left) {
    if (root->_right) {
      // fuck
      replace = detach_min(root->_right);
      replace->_left = root->_left;
      replace->_right = root->_right;
    } else {
      // left child only
      replace = root->_left;
    }
  } else if (root->_right) {
    // right child only
    replace = root->_right;
  } else {
    // poor childless leaf
    replace = nullptr;
  }
  root->_left = nullptr;
  root->_right = nullptr;
  delete root;
  root = replace;
}

bool avl::remove(int id)
{
  // looking in the wrong place
  if (!root)
    return false;

  // found it!
  if (id == root->id) {
    remove(root);
    return true;
  }

  // standard recursive BST delete
  avl &child = id < root->id ? root->_left : root->_right;
  return remove(child, id);
}

void avl::remove_index_impl(int& i)
{
  if (root) {
    remove_index_impl(root->_left, i);
    if (i == 0) return;
    if (--i == 0) {
      remove(root);
      return;
    }
    remove_index_impl(root->_right, i);
  }
}

bool avl::remove_index(int i)
{
  ++i;
  avl::remove_index_impl(root, i);
  return i < 0;
}

void avl::clear()
{
  delete root;
  root = nullptr;
}

avl avl::get(avl node, int id)
{
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

std::vector<avl> avl::search(avl node, const std::string& name)
{
  std::vector<avl> vec;
  search(node, name, vec);
  return vec;
}

void avl::search(avl node, const std::string& name, std::vector<avl>& vec)
{
  if (node) {
    search(node->_left, name, vec);
    if (node->name == name)
      vec.push_back(node);
    search(node->_right, name, vec);
  }
}

void avl::dump(avl node, std::ostream& out)
{
  std::string prefix;
  dump(node, out, prefix, "    --");
}

void avl::dump(avl node, std::ostream& out, std::string& prefix, const char *ch)
{
  if (node) {
    prefix.append(ch, 2);
    dump(node->_left, out, prefix, "  | ,-");
    prefix.resize(prefix.size() - 2);
    out << prefix << ch + 4
        << std::setw(8) << std::setfill('0') << node->id
        << " " << node->name << " [" << node->_h << "]\n";
    prefix.append(ch + 2, 2);
    dump(node->_right, out, prefix, "|   `-");
    prefix.resize(prefix.size() - 2);
  }
}

std::vector<avl> avl::inorder()
{
  std::vector<avl> vec;
  avl::inorder(root, vec);
  return vec;
}

std::vector<avl> avl::preorder()
{
  std::vector<avl> vec;
  avl::preorder(root, vec);
  return vec;
}

std::vector<avl> avl::postorder()
{
  std::vector<avl> vec;
  avl::postorder(root, vec);
  return vec;
}

void avl::inorder(std::vector<avl>& vec)
{
  // LRN
  if (root) {
    inorder(root->_left, vec);
    vec.push_back(root);
    inorder(root->_right, vec);
  }
}

void avl::preorder(std::vector<avl>& vec)
{
  // NLR
  if (root) {
    vec.push_back(root);
    preorder(root->_left, vec);
    preorder(root->_right, vec);
  }
}

void avl::postorder(std::vector<avl>& vec)
{
  // LRN
  if (root) {
    postorder(root->_left, vec);
    postorder(root->_right, vec);
    vec.push_back(root);
  }
}

bool avl::match(const MatchSpec *spec)
{
  return node
    ? (spec &&
        node->id == spec->id &&
        node->name == spec->name &&
        match(node->_left, spec->left) &&
        match(node->_right, spec->right))
    : !spec;
}
