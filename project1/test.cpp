#include <functional>
#include <random>
#include <catch2/catch_test_macros.hpp>
#include "avl.h"
#include "shell.h"

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
  REQUIRE(avl::insert(tree, 80001000, "William Perez"));
  REQUIRE(avl::insert(tree, 80002000, "Ben Burke"));
  REQUIRE(avl::insert(tree, 0xCDA3031, "Adrien Ozanne"));
  REQUIRE(avl::match(shell.tree, avl::MatchSpec {
    avl::MatchSpec { 80001000, "William Perez" },
    80002000, "Ben Burke",
    avl::MatchSpec { 0xCDA3031, "Adrien Ozanne" }
  }));

  // right rotation
  REQUIRE(avl::insert(tree, 60007000, "Donald Trump"));
  REQUIRE(avl::insert(tree, 50003530, "Cheryl Resch"));
  REQUIRE(avl::match(shell.tree, avl::MatchSpec {
    {
      { 60007000, "Donald Trump" },
      50003530, "Cheryl Resch",
      { 80001000, "William Perez" }
    },
    80002000, "Ben Burke",
    { 0xCDA3101, "Adrien Ozanne" }
  }));
}

TEST_CASE("Handles many inserts and deletes") {
  avl::Tree tree;
  int nextid;
  std::vector<avl*> vec;

  // add 100 nodes
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      avl::insert(tree, 90 * i + 900 * j, "test");
    }
  }
  nextid = 0;
  vec = avl::inorder(tree);
  for (avl*& node : vec) {
    if (node->id != nextid)
      FAIL("You suck");
    nextid += 90;
  }
  REQUIRE(nextid == 9000);

  // delete 10 nodes
  for (int w = 1; w <= 10; w++)
    avl::remove(tree, w * 630);

  nextid = 0;
  vec = avl::inorder(tree);
  for (avl*& node : vec) {
    if (node->id != nextid)
      FAIL("You suck more");
    nextid += 90;
    if (nextid % 630 == 0 && nextid <= 6300)
      nextid += 90;
  }
  REQUIRE(nextid == 9000);
}
