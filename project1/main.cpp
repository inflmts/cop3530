/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  main.cpp
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *****************************************************************************/

#include <iostream>
#include <string>
#include <unistd.h>
#include "shell.h"

static const char *const usage = "usage: gavl [-h]\n";

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

  Shell shell;
  std::string line;
  while (1) {
#ifdef GAVL_EXT
    std::cout << "> ";
#endif
    if (!std::getline(std::cin, line))
      break;
    if (shell.execute(line.c_str()) & GAVL_QUIT)
      break;
  }

  return 0;
}
