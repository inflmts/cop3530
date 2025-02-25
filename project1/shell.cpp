/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  shell.cpp
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *****************************************************************************/

#include <iomanip>
#include <iostream>
#include "avl.h"
#include "shell.h"

#define ISSPACE(c) ((c) == ' ')
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')
#define ISALPHA(c) (ISUPPER(c) || ISLOWER(c))
#define ISNAME(c) ((c) == ' ' || ISALPHA(c))

static bool parse_id(const char **p, int *idp)
{
  const char *s = *p;
  int id = 0;
  for (int len = 0; len < 8; len++) {
    if (!ISDIGIT(*s))
      return false;
    id = 10 * id + (*s - '0');
    ++s;
  }
  *p = s;
  *idp = id;
  return true;
}

static bool parse_name(const char **p, std::string *namep)
{
  const char *s = *p;
  if (*s != '"')
    return false;
  ++s;
  size_t len = 0;
  while (1) {
    if (s[len] == '"')
      break;
    if (!ISNAME(s[len]))
      return false;
    ++len;
  }
  *p = s + len + 1;
  *namep = std::string(s, len);
  return true;
}

static bool parse_int(const char **p, int *np)
{
  const char *s = *p;
  int n = 0;
  if (!ISDIGIT(*s))
    return false;
  do {
    n = 10 * n + (*s - '0');
    ++s;
  } while (ISDIGIT(*s));
  *p = s;
  *np = n;
  return true;
}

/*******************************************************************************
 * Shell
 ******************************************************************************/

Shell::Shell() : out(std::cout) {}
Shell::Shell(std::ostream& _out) :  out(_out) {}

int Shell::execute(const char *s)
{
  int status = execute_internal(s);
  switch (status) {
    case GAVL_DONE:
      break;
    case GAVL_SUCCESS:
      out << "successful\n"; break;
    default:
      out << "unsuccessful\n";
  }
  if (limit && --limit == 0)
    status |= GAVL_QUIT;
  return status;
}

int Shell::execute_internal(const char *s)
{
  // compat: accept an integer to set the command limit
  if (ISDIGIT(*s)) {
    limit = *s - '0';
    ++s;
    while (*s) {
      if (!ISDIGIT(*s))
        return GAVL_INVALID_SYNTAX;
      limit = limit * 10 + (*s - '0');
      ++s;
    }
    ++limit;
    return GAVL_DONE;
  }

  const char *t = s;
  while (1) {
    if (!*s || ISSPACE(*s))
      break;
    ++s;
  }
  std::string cmd(t, s - t);
  while (ISSPACE(*s))
    ++s;

  if (cmd == "insert")
    return execute_insert(s);
  if (cmd == "remove")
    return execute_remove(s);
  if (cmd == "search")
    return execute_search(s);
  if (cmd == "printInorder")
    return execute_inorder(s);
  if (cmd == "printPreorder")
    return execute_preorder(s);
  if (cmd == "printPostorder")
    return execute_postorder(s);
  if (cmd == "printLevelCount")
    return execute_height(s);
  if (cmd == "removeInorder")
    return execute_remove_index(s);

  return GAVL_INVALID_DIRECTIVE;
}

int Shell::execute_insert(const char *s)
{
  std::string name;
  int id;

  if (*s != '"')
    return GAVL_INVALID_SYNTAX;
  if (!parse_name(&s, &name))
    return GAVL_INVALID_SYNTAX;
  if (*s != ' ')
    return GAVL_INVALID_SYNTAX;
  ++s;
  if (!parse_id(&s, &id))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  return tree.insert(id, name) ? GAVL_SUCCESS : GAVL_FAILURE;
}

int Shell::execute_remove(const char *s)
{
  int id;
  if (!parse_id(&s, &id))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  return tree.remove(id) ? GAVL_SUCCESS : GAVL_FAILURE;
}

int Shell::execute_search(const char *s)
{
  if (*s == '"')
    return execute_search_name(s);
  else
    return execute_search_id(s);
}

int Shell::execute_search_id(const char *s)
{
  int id;
  if (!parse_id(&s, &id))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  avl::Node *node = tree.get(id);
  if (!node)
    return GAVL_FAILURE;
  out << node->name << "\n";
  return GAVL_DONE;
}

int Shell::execute_search_name(const char *s)
{
  std::string name;
  if (!parse_name(&s, &name))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  std::vector<avl::Node*> nodes = tree.search(name);
  if (nodes.empty())
    return GAVL_FAILURE;

  for (auto& node : nodes)
    out << std::setw(8) << std::setfill('0') << node->id << "\n";
  return GAVL_DONE;
}

int Shell::execute_inorder(const char *s)
{
  if (*s)
    return GAVL_INVALID_SYNTAX;
  return list(tree.inorder());
}

int Shell::execute_preorder(const char *s)
{
  if (*s)
    return GAVL_INVALID_SYNTAX;
  return list(tree.preorder());
}

int Shell::execute_postorder(const char *s)
{
  if (*s)
    return GAVL_INVALID_SYNTAX;
  return list(tree.postorder());
}

int Shell::list(const std::vector<avl::Node*>& nodes)
{
  bool first = true;
  for (auto& node : nodes) {
    if (first)
      first = false;
    else
      out << ", ";
    out << node->name;
  }
  out << "\n";
  return GAVL_DONE;
}

int Shell::execute_height(const char *s)
{
  if (*s)
    return GAVL_INVALID_SYNTAX;
  out << tree.height() << "\n";
  return GAVL_DONE;
}

int Shell::execute_remove_index(const char *s)
{
  int i;
  if (!parse_int(&s, &i))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  return tree.remove_index(i) ? GAVL_SUCCESS : GAVL_FAILURE;
}
