/*
 * ----------------------------------------
 * Project 2: Simplified PageRank Algorithm
 * ----------------------------------------
 *
 * pagerank.cpp
 * Author: Daniel Li
 * Date: 03/28/2025
 */

#include <istream>
#include <ostream>
#include <set>
#include <unordered_set>

struct Page
{
  float old_rank;
  float rank;
  const std::string url;
  std::unordered_set<Page*> links;
  Page(const std::string& _url) : url(_url) {}
};

bool operator<(const Page& lhs, const Page& rhs) { return lhs.url < rhs.url; }
bool operator<(const Page& lhs, const std::string& rhs) { return lhs.url < rhs; }
bool operator<(const std::string& lhs, const Page& rhs) { return lhs < rhs.url; }

void pagerank(const std::set<Page>& pages, int p)
{
  // initialize ranks
  for (auto it = pages.begin(); it != pages.end(); it++)
    const_cast<Page&>(*it).rank = 1.0f / pages.size();

  while (--p) {
    // save the current ranks and initialize to zero
    for (auto it = pages.begin(); it != pages.end(); it++) {
      Page& page = const_cast<Page&>(*it);
      page.old_rank = page.rank;
      page.rank = 0.0f;
    }
    for (auto it = pages.begin(); it != pages.end(); it++) {
      Page& page = const_cast<Page&>(*it);
      for (auto di = page.links.begin(); di != page.links.end(); di++)
        (*di)->rank += page.old_rank / page.links.size();
    }
  }
}

void pagerank_shell(std::istream& in, std::ostream& out)
{
  int n, p;
  std::string src_url, dest_url;
  std::set<Page> pages;
  in >> n;
  in >> p;
  for (int i = 0; i < n; i++) {
    in >> src_url;
    in >> dest_url;
    Page& src = const_cast<Page&>(*pages.emplace(src_url).first);
    Page& dest = const_cast<Page&>(*pages.emplace(dest_url).first);
    src.links.emplace(&dest);
  }

  pagerank(pages, p);

  std::ios::fmtflags original_flags = out.flags(std::ios::fixed);
  int original_precision = out.precision(2);
  for (auto it = pages.begin(); it != pages.end(); it++)
    out << it->url << ' ' << it->rank << '\n';
  out.flags(original_flags);
  out.precision(original_precision);
}

#ifndef PAGERANK_NO_MAIN

#include <iostream>

int main()
{
  pagerank_shell(std::cin, std::cout);
  return 0;
}

#endif
