/******************************************************************************
 *
 *  Author: Daniel Li
 *
 *****************************************************************************/

#include <vector>
#include <catch2/catch_test_macros.hpp>
#include "avl.h"
#include "shell.h"

static std::vector<int> nodes_to_ids(std::vector<avl::Node*> nodes)
{
  std::vector<int> ids;
  for (auto& node : nodes)
    ids.push_back(node->id);
  return ids;
}

TEST_CASE("Rejects invalid commands") {
  Shell shell;
  REQUIRE(shell.execute("insert \"Arleen_\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"B@gen\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Francesca\" 3003456") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Genesis\" .4569999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Heston 04569999") == GAVL_INVALID_SYNTAX);
}

TEST_CASE("Rotates correctly on insert") {
  avl::Tree tree;

  // left rotation
  REQUIRE(tree.insert(3, "William Perez"));
  REQUIRE(tree.insert(4, "Ben Burke"));
  REQUIRE(tree.insert(5, "Adrien Ozanne"));
  REQUIRE(nodes_to_ids(tree.preorder()) == std::vector<int>{ 4, 3, 5 });

  // right rotation
  REQUIRE(tree.insert(2, "Donald Trump"));
  REQUIRE(tree.insert(1, "Cheryl Resch"));
  REQUIRE(nodes_to_ids(tree.preorder()) == std::vector<int>{ 4, 2, 1, 3, 5 });

  // right-left rotation
  REQUIRE(tree.insert(19, "Hillary Clinton"));
  REQUIRE(tree.insert(6, "Kent Fuchs"));
  REQUIRE(nodes_to_ids(tree.preorder()) == std::vector<int>{ 4, 2, 1, 3, 6, 5, 19 });

  // left-right rotation
  REQUIRE(tree.insert(16, "Thomas Ippolito"));
  REQUIRE(tree.insert(18, "Your Mother"));
  REQUIRE(nodes_to_ids(tree.preorder()) == std::vector<int>{ 4, 2, 1, 3, 6, 5, 18, 16, 19 });
}

TEST_CASE("Handles many inserts and deletes") {
  avl::Tree tree;
  int nextid;
  std::vector<avl::Node*> vec;

  // add 100 nodes
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      tree.insert(90 * i + 900 * j, "test");
    }
  }
  nextid = 0;
  vec = tree.inorder();
  for (auto& node : vec) {
    if (node->id != nextid)
      FAIL("You suck");
    nextid += 90;
  }
  REQUIRE(nextid == 9000);

  // delete 10 nodes
  for (int w = 1; w <= 10; w++)
    tree.remove(w * 630);

  nextid = 0;
  vec = tree.inorder();
  for (auto& node : vec) {
    if (node->id != nextid)
      FAIL("You suck more");
    nextid += 90;
    if (nextid % 630 == 0 && nextid <= 6300)
      nextid += 90;
  }
  REQUIRE(nextid == 9000);
}
