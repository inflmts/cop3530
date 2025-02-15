#ifndef AVL_H
#define AVL_H

#include <ostream>
#include <string>
#include <vector>

namespace avl {

class Node;

class Tree
{
  private:
    friend class Node;
    Node *root;
    Tree& operator=(Node *node) { root = node; return *this; }
    Node *operator->() { return root; }
    operator Node*() { return root; }
    operator bool() { return root; }

  public:
    Tree();
    ~Tree();

    unsigned int height();
    Node *insert(int id, const std::string& name);
    void remove();
    bool remove(int id);
    bool remove_index(int i);
    void clear();
    Node *get(int id);
    std::vector<Node*> search(const std::string& name);
    void search(const std::string& name, std::vector<Node*>& vec);
    void dump(std::ostream& out);
    void dump(std::ostream& out, std::string& prefix, const char *marks);

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

    Node(int id, const std::string& name);
    void update_height();
};

} // namespace avl

#endif
