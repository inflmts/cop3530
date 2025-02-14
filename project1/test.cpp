#include <functional>
#include <random>
#include <catch2/catch_test_macros.hpp>
#include "avl.h"
#include "shell.h"

const std::uniform_int_distribution<unsigned int> dist(10000000, 99999999);
std::mt19937 random;
auto random_id = std::bind(dist, random);

TEST_CASE("Rejects invalid commands") {
  Shell shell;
  REQUIRE(shell.execute("insert \"Arleen_\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"B@gen\" 45679999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Francesca\" 3003456") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Genesis\" .4569999") == GAVL_INVALID_SYNTAX);
  REQUIRE(shell.execute("insert \"Heston 04569999") == GAVL_INVALID_SYNTAX);
}

TEST_CASE("Rotates correctly on insert") {
  Shell shell;
  // left rotation
  REQUIRE(shell.execute("insert \"William Perez\" 10003031") == GAVL_SUCCESS);
  REQUIRE(shell.execute("insert \"Ben Burke\" 20003031") == GAVL_SUCCESS);
  REQUIRE(shell.execute("insert \"Adrien Ozanne\" 30003031") == GAVL_SUCCESS);
  REQUIRE(shell.tree.match(avl::MatchSpec
    { 20003031, "Ben Burke",
      avl::MatchSpec { 10003031, "William Perez" }.p(),
      avl::MatchSpec { 30003031, "Adrien Ozanne" }.p()
    }.p()
  ));
}

TEST_CASE("Handles many inserts and deletes") {
  avl::Tree tree;
  for (int i = 0; i < 101; i++) {
    tree.insert(random_id(), "test");
  }
  int previd = 0;
  int count = 0;
  tree.inorder([&] (avl::Node *node) {
    REQUIRE(node->id > previd);
    previd = node->id;
    ++count;
  });
  REQUIRE(count == 101);
}
