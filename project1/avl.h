/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  avl.h
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *****************************************************************************/

#ifndef AVL_H
#define AVL_H

#include <initializer_list>
#include <ostream>
#include <string>
#include <vector>

namespace avl {

class Node;

class Tree
{
  private:
    friend class Node;
    Node *root = nullptr;
    Tree& operator=(Node *node) { root = node; return *this; }
    Node *operator->() const { return root; }
    operator Node*() const { return root; }
    operator bool() const { return root; }

  public:
    Tree();
    explicit Tree(const Tree& other);
    ~Tree();

    unsigned int height();
    Node *insert(int id, const std::string& name);
    bool pop();
    bool remove(int id);
    bool remove_index(int i);
    void clear();
    Node *get(int id);
    std::vector<Node*> search(const std::string& name);
    void search(const std::string& name, std::vector<Node*>& vec);
    void dump(std::ostream& out) const;
    void dump(std::ostream& out, std::string& prefix, const char *marks) const;
    bool check() const;
    bool check(int min, int max) const;
    bool identical(const Tree& other) const;

    // Iteration

    std::vector<Node*> inorder();
    std::vector<Node*> preorder();
    std::vector<Node*> postorder();
    void inorder(std::vector<Node*>& vec);
    void preorder(std::vector<Node*>& vec);
    void postorder(std::vector<Node*>& vec);

  private:
    void rotate_left();
    void rotate_right();
    void balance();
    Node *detach_min();
    void remove_index_impl(int& i);
};

static inline std::ostream& operator<<(std::ostream& out, const Tree& tree)
{
  tree.dump(out);
  return out;
}

class Node
{
  private:
    friend class Tree;
    Tree left;
    Tree right;
    unsigned int h;

  public:
    const int id;
    const std::string name;

    Node(const Node& other);
    Node(int id, const std::string& name);
    void update_height();
};

inline unsigned int Tree::height()
{
  return root ? root->h : 0;
}

} // namespace avl

#endif
