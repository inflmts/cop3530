#ifndef AVL_H
#define AVL_H

#include <functional>
#include <ostream>
#include <string>
#include <vector>

namespace avl {

class Node;
class Tree;

struct MatchSpec
{
  int id;
  std::string name;
  const MatchSpec *left;
  const MatchSpec *right;
  MatchSpec *p() { return this; }
};

bool match(Node *node, const MatchSpec *spec);

typedef const std::function<void(Node*)>& itercb;
void inorder(Node *root, itercb cb);
void preorder(Node *root, itercb cb);
void postorder(Node *root, itercb cb);

class Node
{
  private:
    friend class Tree;
    Node **_slot;
    Node *_left;
    Node *_right;
    unsigned int _h;

  public:
    const int id;
    const std::string name;

  private:
    Node(Node **slot, int id, const std::string& name);
    ~Node();
    void rotate_left();
    void rotate_right();
    void balance();
    Node *insert(int id, const std::string& name);

  public:
    friend bool match(Node *node, const MatchSpec *spec);
    friend void inorder(Node *root, const itercb& cb);
    friend void preorder(Node *root, const itercb& cb);
    friend void postorder(Node *root, const itercb& cb);
};

class Tree
{
  private:
    Node *_root;

  public:
    Tree();
    ~Tree();
    unsigned int height();
    void clear();
    Node *insert(int id, const std::string& name);
    bool remove(int id);
    bool remove_index(int i);
    Node *get(int id);
    std::vector<Node*> search(const std::string& name);

    Node *root() { return _root; }
    bool match(const MatchSpec *spec) { return avl::match(_root, spec); }
    void inorder(itercb cb) { avl::inorder(_root, cb); }
    void preorder(itercb cb) { avl::preorder(_root, cb); }
    void postorder(itercb cb) { avl::postorder(_root, cb); }
};

} // namespace avl

#endif
