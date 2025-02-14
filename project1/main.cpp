#include <ctype.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "shell.h"

static const char *const usage = "usage: gavl [-h]\n";

static int try_parse_limit(const char *s)
{
  int n = 0;
  if (!*s)
    return -1;
  do {
    if (!isdigit(*s))
      return -1;
    n = n * 10 + (*s - '0');
    ++s;
  } while (*s);
  return n;
}

int main(int argc, char **argv)
{
  int opt;
  while ((opt = getopt(argc, argv, "h")) != -1) {
    switch (opt) {
      case 'h':
        std::cout << usage;
        return 0;
      default:
        std::cerr << usage;
        return 2;
    }
  }
  if (optind < argc) {
    std::cerr << usage;
    return 2;
  }

#ifdef GAVL_EXT
  std::cout << "COP3530 Project 1 | Gator AVL 2.1\n";
#endif

  Shell shell;
  std::string line;
  int limit = -1;
  while (limit) {

#ifdef GAVL_EXT
    std::cout << "~> ";
#endif

    if (!std::getline(std::cin, line))
      goto end;

    // compat: accept an integer to set the command limit
    int n = try_parse_limit(line.c_str());
    if (n >= 0) {
      limit = n;
      continue;
    }

    switch (shell.execute(line.c_str())) {
      case GAVL_DONE:
        break;
      case GAVL_SUCCESS:
        std::cout << "successful\n"; break;
      case GAVL_QUIT:
        goto end;
#ifdef GAVL_EXT
      case GAVL_INVALID_SYNTAX:
        std::cout << "syntax error\n"; break;
      case GAVL_INVALID_DIRECTIVE:
        std::cout << "invalid directive\n"; break;
#endif
      default:
        std::cout << "unsuccessful\n";
    }

    if (limit > 0)
      --limit;
  }

end:
  return 0;
}
