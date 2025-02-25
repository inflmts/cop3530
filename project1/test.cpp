/******************************************************************************
 *  Project 1: Gator AVL
 ******************************************************************************
 *
 *  test.cpp
 *  Author: Daniel Li
 *  Date: Feb 24 2025
 *
 *  Unit tests [5 points]
 *  Submit a file containing the following five tests using the Catch Framework:
 *
 *****************************************************************************/

#include <sstream>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include "avl.h"
#include "shell.h"

/*******************************************************************************
 * Test at least five incorrect commands.
 * Ex: insert "A11y" 45679999 [0.5 points]
 */
TEST_CASE("Invalid commands", "[required]") {
  std::stringstream ss;
  Shell shell(ss);
  REQUIRE(shell.execute("insert \"Arleen_\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"B@gen\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Francesca\" 3003456") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Genesis\" .4569999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Heston 04569999") == GAVL_INVALID_SYNTAX);
  REQUIRE(ss.str() ==
    "unsuccessful\n"
    "unsuccessful\n"
    "unsuccessful\n"
    "unsuccessful\n"
    "unsuccessful\n"
  );
}

/*******************************************************************************
 * Test at least three edge cases for various functions.
 * Ex: removing a node that doesn’t exist [0.5 points]
 */
TEST_CASE("Edge cases", "[required]") {
  avl::Tree tree;

  // populate tree with data
  for (int i = 100; i <= 10000; i += 100)
    tree.insert(i, "test");

  SECTION("Inserting existing node") {
    avl::Tree copy(tree);
    REQUIRE(tree.insert(2500, "test") == nullptr);
    REQUIRE(copy.identical(tree));
  }

  SECTION("Retrieving node that doesn't exist") {
    REQUIRE(tree.get(6070) == nullptr);
  }

  SECTION("Removing node that doesn't exist") {
    avl::Tree copy(tree);
    REQUIRE_FALSE(tree.remove(6070));
    REQUIRE(copy.identical(tree));
  }
}

/*******************************************************************************
 * Test all four rotation cases [1 point]
 */
TEST_CASE("Rotations", "[required]") {
  avl::Tree tree;

  SECTION("Left rotation") {
    REQUIRE(tree.insert(100, "William Perez"));
    REQUIRE(tree.insert(200, "Ben Burke"));
    REQUIRE(tree.insert(300, "Adrien Ozanne"));
  }

  SECTION("Right rotation") {
    REQUIRE(tree.insert(300, "Amanpreet Kapoor"));
    REQUIRE(tree.insert(200, "Mohammad Al-Saad"));
    REQUIRE(tree.insert(100, "Cheryl Resch"));
  }

  SECTION("Right-left rotation") {
    REQUIRE(tree.insert(100, "Cheryl Resch"));
    REQUIRE(tree.insert(300, "Amanpreet Kapoor"));
    REQUIRE(tree.insert(200, "Mohammad Al-Saad"));
  }

  SECTION("Left-right rotation") {
    REQUIRE(tree.insert(300, "Cheryl Resch"));
    REQUIRE(tree.insert(100, "Amanpreet Kapoor"));
    REQUIRE(tree.insert(200, "Mohammad Al-Saad"));
  }

  INFO(tree);
  REQUIRE(tree.check());
}

/*******************************************************************************
 * Test all three deletion cases (no children, one child, two children) [1 point]
 */
TEST_CASE("Deletions", "[required]") {
  avl::Tree tree;

  // populate tree with data
  for (int i = 100; i <= 1300; i += 100)
    tree.insert(i, "test");

  INFO(tree);

  // just test every node
  int id = GENERATE(range(100, 1400, 100));
  REQUIRE(tree.remove(id));
  INFO(tree);
  tree.check();
}

/*******************************************************************************
 * Insert at least 100 nodes, remove ten random nodes using any delete
 * operation, and check in order.
 * Hint: Use a loop to verify the 100 insertions and the 90 nodes after removal.
 */
TEST_CASE("Inserting and removing many nodes", "[required]") {
  avl::Tree tree;
  int lastid;
  std::vector<avl::Node *> result;

  // insert 100 nodes out of order
  for (int i = 0; i <= 900; i += 100) {
    for (int j = 0; j <= 9000; j += 1000) {
      tree.insert(100 + i + j, "test");
    }
  }

  INFO("After insertion:");
  INFO(tree);
  tree.check();

  lastid = 0;
  result = tree.inorder();
  REQUIRE(result.size() == 100);
  for (auto& node : result) {
    lastid += 100;
    REQUIRE(node->id == lastid);
  }
  REQUIRE(lastid == 10000);

  // remove 10 nodes
  for (int w = 10; w >= 1; --w)
    tree.remove(w * 600);

  INFO("After deletion:");
  INFO(tree);
  tree.check();

  lastid = 0;
  result = tree.inorder();
  REQUIRE(result.size() == 90);
  for (auto& node : result) {
    lastid += 100;
    // skip deleted nodes
    if (lastid % 600 == 0 && lastid <= 6000)
      lastid += 100;
    REQUIRE(node->id == lastid);
  }
  REQUIRE(lastid == 10000);
}

/******************************************************************************/

TEST_CASE("Shell syntax") {
  std::stringstream ss;
  Shell shell(ss);

  SECTION("Empty input") {
    REQUIRE(shell.execute("") == GAVL_INVALID_DIRECTIVE);
    REQUIRE(ss.str() ==
      "unsuccessful\n"
    );
  }

  SECTION("Invalid directive") {
    REQUIRE(shell.execute("bad") == GAVL_INVALID_DIRECTIVE);
    REQUIRE(ss.str() ==
      "unsuccessful\n"
    );
  }

  SECTION("Command limit") {
    REQUIRE(shell.execute("5") == GAVL_DONE);
    REQUIRE(shell.execute("insert \"Daniel Li\" 12345678") == GAVL_SUCCESS);
    REQUIRE(shell.execute("insert \"Daniel L@\" 12345678") == GAVL_INVALID_SYNTAX);
    REQUIRE(shell.execute("insert \"Am Kapoor\" 12345678") == GAVL_FAILURE);
    REQUIRE(shell.execute("insert \"Robert Pi\" 12345679") == GAVL_SUCCESS);
    REQUIRE(shell.execute("insert \"Alfred Ko\" 12345680") == (GAVL_SUCCESS | GAVL_QUIT));
    REQUIRE(ss.str() ==
      "successful\n"
      "unsuccessful\n"
      "unsuccessful\n"
      "successful\n"
      "successful\n"
    );
  }
}
