#include <ctype.h>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include "avl.h"
#include "shell.h"

struct Command
{
  std::initializer_list<const char*> names;
  int (Shell::*fn)(const char *s);
  const char *usage;
};

std::initializer_list<Command> commands = {
  { { "help", "h" }, Shell::execute_help },
  { { "quit", "q" }, Shell::execute_quit },
  { { "dump", "d" }, Shell::execute_dump },
  { { "fill", "f" }, Shell::execute_fill },
  { { "insert", "i" }, Shell::execute_insert, "NAME ID" },
  { { "remove", "r" }, Shell::execute_remove, "ID" },
  { { "search", "s" }, Shell::execute_search, "ID|NAME" },
  { { "list", "l", "printInorder" }, Shell::execute_inorder },
  { { "pre", "printPreorder" }, Shell::execute_preorder },
  { { "post", "printPostorder" }, Shell::execute_postorder },
  { { "height", "printLevelCount" }, Shell::execute_height },
  { { "removeindex", "ri", "removeInorder" }, Shell::execute_remove_index, "N" }
};

static bool isnamechar(int c)
{
  return c == ' '
      || (c >= 'A' && c <= 'Z')
      || (c >= 'a' && c <= 'z');
}

static bool parse_id(const char **p, int *idp)
{
  const char *s = *p;
  int id = 0;
  for (int len = 0; len < 8; len++) {
    if (!isdigit(*s))
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
    if (!isnamechar(s[len]))
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
  if (!isdigit(*s))
    return false;
  do {
    n = 10 * n + (*s - '0');
    ++s;
  } while (isdigit(*s));
  *p = s;
  *np = n;
  return true;
}

/*******************************************************************************
 * Shell
 ******************************************************************************/

Shell::Shell() :
  out(std::cout)
{}

int Shell::execute(const char *s)
{
  const char *t = s;
  while (1) {
    if (!*s || isspace(*s))
      break;
    ++s;
  }
  std::string cmd(t, s - t);
  while (isspace(*s))
    ++s;

  for (auto& command : commands)
    for (const char *name : command.names)
    if (cmd == name)
        return (this->*command.fn)(s);

  return GAVL_INVALID_DIRECTIVE;
}

int Shell::execute_help(const char *s)
{
  out << "Available commands:\n";
  for (auto& command : commands) {
    out << " " << *command.names.begin();
    if (command.usage)
      out << " " << command.usage;
    out << '\n';
  }
  return GAVL_DONE;
}

int Shell::execute_quit(const char *s)
{
  return GAVL_QUIT;
}

int Shell::execute_dump(const char *s)
{
  avl::dump(tree, out);
  return GAVL_DONE;
}

int Shell::execute_fill(const char *s)
{
  int count;
  if (!parse_int(&s, &count))
    return GAVL_INVALID_SYNTAX;
  if (count > 1000)
    return GAVL_FAILURE;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  for (int i = 0; i < count; i++) {
    avl::insert(tree, rand() % 100000000, "test");
    avl::dump(tree, out);
    out << "\n";
  }
  return GAVL_SUCCESS;
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

  return avl::insert(tree, id, name) ? GAVL_SUCCESS : GAVL_FAILURE;
}

int Shell::execute_remove(const char *s)
{
  int id;
  if (!parse_id(&s, &id))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  return avl::remove(tree, id) ? GAVL_SUCCESS : GAVL_FAILURE;
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

  avl *node = avl::get(tree, id);
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

  std::vector<avl*> nodes = avl::search(tree, name);
  if (nodes.empty())
    return GAVL_FAILURE;

  for (auto& node : nodes)
    out << std::setw(8) << std::setfill('0') << node->id << "\n";
  return GAVL_DONE;
}

int Shell::execute_inorder(const char *s)
{
  return execute_list(s, avl::inorder);
}

int Shell::execute_preorder(const char *s)
{
  return execute_list(s, avl::preorder);
}

int Shell::execute_postorder(const char *s)
{
  return execute_list(s, avl::postorder);
}

int Shell::execute_list(const char *s, std::vector<avl*> (*fn)(avl*))
{
  if (*s)
    return GAVL_INVALID_SYNTAX;

  std::vector<avl*> nodes = fn(tree);
  bool first = true;
  for (avl*& node : nodes) {
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
  out << avl::height(tree) << "\n";
  return GAVL_DONE;
}

int Shell::execute_remove_index(const char *s)
{
  int i;
  if (!parse_int(&s, &i))
    return GAVL_INVALID_SYNTAX;
  if (*s)
    return GAVL_INVALID_SYNTAX;

  return avl::remove_index(tree, i) ? GAVL_SUCCESS : GAVL_FAILURE;
}
