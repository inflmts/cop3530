#ifndef SHELL_H
#define SHELL_H

#include <ostream>
#include "avl.h"

enum
{
  GAVL_DONE,
  GAVL_SUCCESS,
  GAVL_QUIT,
  GAVL_FAILURE,
  GAVL_INVALID_SYNTAX,
  GAVL_INVALID_DIRECTIVE
};

class Shell
{
  private:
    std::ostream& out;
    bool try_parse_limit(const char *s);
    int execute_search_id(const char *s);
    int execute_search_name(const char *s);
    int execute_list(const char *s, std::vector<avl::Node*> (avl::Tree::*fn)());

  public:
    avl::Tree tree;

    Shell();

    int execute(const char *s);
    // help
    int execute_help(const char *s);
    // quit
    int execute_quit(const char *s);
    // dump
    int execute_dump(const char *s);
    // fill COUNT
    int execute_fill(const char *s);
    // insert NAME ID
    int execute_insert(const char *s);
    // remove ID
    int execute_remove(const char *s);
    // search ID|NAME
    int execute_search(const char *s);
    // printInorder
    int execute_inorder(const char *s);
    // printPreorder
    int execute_preorder(const char *s);
    // printPostorder
    int execute_postorder(const char *s);
    // printLevelCount
    int execute_height(const char *s);
    // removeInorder N
    int execute_remove_index(const char *s);
};

#endif
