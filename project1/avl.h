#ifndef AVL_H
#define AVL_H

#include <ostream>
#include <string>
#include <vector>

class avl
{
  private:
    class Node;
    Node *root;

    void update_height();
    void rotate_left();
    void rotate_right();
    void balance();
    avl *detach_min();
    void remove_index_impl(int& i);

  public:
    avl();
    avl(int id, const std::string& name);
    ~avl();
    operator bool() { return root; }
    Node *operator->() { return root; }

    unsigned int height();
    avl *insert(int id, const std::string& name);
    void remove();
    bool remove(int id);
    bool remove_index(int i);
    void clear();
    avl *get(int id);
    std::vector<avl> search(const std::string& name);
    void search(const std::string& name, std::vector<avl>& vec);
    void dump(std::ostream& out);
    void dump(std::ostream& out, std::string& prefix, const char *marks);

    std::vector<avl> inorder();
    std::vector<avl> preorder();
    std::vector<avl> postorder();
    void inorder(std::vector<avl>& vec);
    void preorder(std::vector<avl>& vec);
    void postorder(std::vector<avl>& vec);

    struct MatchSpec
    {
      const MatchSpec *left;
      int id;
      std::string name;
      const MatchSpec *right;
      operator MatchSpec *() { return this; }
    };

    bool match(const MatchSpec *spec);
};

class avl::Node
{
  friend class avl;
  avl _left;
  avl _right;
  unsigned int _h;
  const int id;
  const std::string name;
  Node(int i, const std::string& nm) : id(i), name(nm) {}
};

inline void avl::update_height()
{
  root->_h = root->_left
    ? (root->_right && root->_right->_h > root->_left->_h
        ? root->_right->_h + 1
        : root->_left->_h + 1)
    : 0;
}

inline unsigned int avl::height()
{
  return root ? root->_h + 1 : 0;
}

#endif
