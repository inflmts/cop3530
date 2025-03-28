/*
 * ----------------------------------------
 * Project 2: Simplified PageRank Algorithm
 * ----------------------------------------
 *
 * test.cpp
 * Author: Daniel Li
 * Date: 03/28/2025
 */

#define PAGERANK_NO_MAIN
#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include "pagerank.cpp"

std::string pagerank_exec(const std::string& input)
{
  std::stringstream in(input);
  std::stringstream out;
  pagerank_shell(in, out);
  return out.str();
}

#define PAGERANK_TEST(input, output) REQUIRE(pagerank_exec(input) == (output))

TEST_CASE("No input") {
  PAGERANK_TEST(
    "0 10\n"
    ,
    ""
  );
}

TEST_CASE("Set 1, p=1") {
  PAGERANK_TEST(
    "12 1\n"
    "google.com doi.org\n"
    "google.com arm.com\n"
    "google.com fedex.com\n"
    "google.com blender.org\n"
    "google.com java.com\n"
    "expressjs.com google.com\n"
    "expressjs.com java.com\n"
    "heroku.com creativecommons.org\n"
    "heroku.com fedex.com\n"
    "imacs.org google.com\n"
    "java.com blender.org\n"
    "arm.com blender.org\n"
    ,
    "arm.com 0.10\n"
    "blender.org 0.10\n"
    "creativecommons.org 0.10\n"
    "doi.org 0.10\n"
    "expressjs.com 0.10\n"
    "fedex.com 0.10\n"
    "google.com 0.10\n"
    "heroku.com 0.10\n"
    "imacs.org 0.10\n"
    "java.com 0.10\n"
  );
}

TEST_CASE("Set 1, p=2") {
  PAGERANK_TEST(
    "12 2\n"
    "google.com doi.org\n"
    "google.com arm.com\n"
    "google.com fedex.com\n"
    "google.com blender.org\n"
    "google.com java.com\n"
    "expressjs.com google.com\n"
    "expressjs.com java.com\n"
    "heroku.com creativecommons.org\n"
    "heroku.com fedex.com\n"
    "imacs.org google.com\n"
    "java.com blender.org\n"
    "arm.com blender.org\n"
    ,
    "arm.com 0.02\n"
    "blender.org 0.22\n"
    "creativecommons.org 0.05\n"
    "doi.org 0.02\n"
    "expressjs.com 0.00\n"
    "fedex.com 0.07\n"
    "google.com 0.15\n"
    "heroku.com 0.00\n"
    "imacs.org 0.00\n"
    "java.com 0.07\n"
  );
}

TEST_CASE("Set 1, p=3") {
  PAGERANK_TEST(
    "12 3\n"
    "google.com doi.org\n"
    "google.com arm.com\n"
    "google.com fedex.com\n"
    "google.com blender.org\n"
    "google.com java.com\n"
    "expressjs.com google.com\n"
    "expressjs.com java.com\n"
    "heroku.com creativecommons.org\n"
    "heroku.com fedex.com\n"
    "imacs.org google.com\n"
    "java.com blender.org\n"
    "arm.com blender.org\n"
    ,
    "arm.com 0.03\n"
    "blender.org 0.12\n"
    "creativecommons.org 0.00\n"
    "doi.org 0.03\n"
    "expressjs.com 0.00\n"
    "fedex.com 0.03\n"
    "google.com 0.00\n"
    "heroku.com 0.00\n"
    "imacs.org 0.00\n"
    "java.com 0.03\n"
  );
}

TEST_CASE("Set 1, p=3, different order") {
  PAGERANK_TEST(
    "12 3\n"
    "google.com java.com\n"
    "google.com blender.org\n"
    "java.com blender.org\n"
    "heroku.com creativecommons.org\n"
    "arm.com blender.org\n"
    "google.com doi.org\n"
    "heroku.com fedex.com\n"
    "expressjs.com google.com\n"
    "expressjs.com java.com\n"
    "google.com arm.com\n"
    "google.com fedex.com\n"
    "imacs.org google.com\n"
    ,
    "arm.com 0.03\n"
    "blender.org 0.12\n"
    "creativecommons.org 0.00\n"
    "doi.org 0.03\n"
    "expressjs.com 0.00\n"
    "fedex.com 0.03\n"
    "google.com 0.00\n"
    "heroku.com 0.00\n"
    "imacs.org 0.00\n"
    "java.com 0.03\n"
  );
}

TEST_CASE("Set 1, p=3, with duplicates") {
  PAGERANK_TEST(
    "30 3\n"
    "expressjs.com google.com\n"
    "arm.com blender.org\n"
    "imacs.org google.com\n"
    "expressjs.com google.com\n"
    "heroku.com fedex.com\n"
    "java.com blender.org\n"
    "google.com fedex.com\n"
    "google.com java.com\n"
    "google.com fedex.com\n"
    "java.com blender.org\n"
    "google.com blender.org\n"
    "google.com fedex.com\n"
    "arm.com blender.org\n"
    "google.com doi.org\n"
    "imacs.org google.com\n"
    "google.com doi.org\n"
    "expressjs.com java.com\n"
    "heroku.com creativecommons.org\n"
    "expressjs.com java.com\n"
    "expressjs.com java.com\n"
    "google.com arm.com\n"
    "google.com blender.org\n"
    "java.com blender.org\n"
    "google.com arm.com\n"
    "heroku.com creativecommons.org\n"
    "heroku.com fedex.com\n"
    "google.com doi.org\n"
    "imacs.org google.com\n"
    "google.com java.com\n"
    "google.com java.com\n"
    ,
    "arm.com 0.03\n"
    "blender.org 0.12\n"
    "creativecommons.org 0.00\n"
    "doi.org 0.03\n"
    "expressjs.com 0.00\n"
    "fedex.com 0.03\n"
    "google.com 0.00\n"
    "heroku.com 0.00\n"
    "imacs.org 0.00\n"
    "java.com 0.03\n"
  );
}

TEST_CASE("Gradescope 1, p=2", "[gradescope]") {
  PAGERANK_TEST(
    "7 2\n"
    "google.com gmail.com\n"
    "google.com maps.com\n"
    "facebook.com ufl.edu\n"
    "ufl.edu google.com\n"
    "ufl.edu gmail.com\n"
    "maps.com facebook.com\n"
    "gmail.com maps.com\n"
    ,
    "facebook.com 0.20\n"
    "gmail.com 0.20\n"
    "google.com 0.10\n"
    "maps.com 0.30\n"
    "ufl.edu 0.20\n"
  );
}

TEST_CASE("Gradescope 1, p=3", "[gradescope]") {
  PAGERANK_TEST(
    "7 3\n"
    "google.com gmail.com\n"
    "google.com maps.com\n"
    "facebook.com ufl.edu\n"
    "ufl.edu google.com\n"
    "ufl.edu gmail.com\n"
    "maps.com facebook.com\n"
    "gmail.com maps.com\n"
    ,
    "facebook.com 0.30\n"
    "gmail.com 0.15\n"
    "google.com 0.10\n"
    "maps.com 0.25\n"
    "ufl.edu 0.20\n"
  );
}
