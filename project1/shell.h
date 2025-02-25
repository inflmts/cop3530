/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  shell.h
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *****************************************************************************/

#ifndef SHELL_H
#define SHELL_H

#include <ostream>
#include "avl.h"

enum
{
  GAVL_DONE,
  GAVL_SUCCESS,
  GAVL_FAILURE,
  GAVL_INVALID_SYNTAX,
  GAVL_INVALID_DIRECTIVE
};

#define GAVL_RESULT 0x7f
#define GAVL_QUIT 0x80

class Shell
{
  private:
    std::ostream& out;
    unsigned int limit = 0;
    int execute_internal(const char *s);
    int execute_search_id(const char *s);
    int execute_search_name(const char *s);
    int list(const std::vector<avl::Node*>& nodes);

  public:
    avl::Tree tree;

    Shell();
    Shell(std::ostream& out);

    int execute(const char *s);
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
