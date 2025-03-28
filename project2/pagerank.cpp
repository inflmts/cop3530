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

class Page
{
  private:
    float _old_rank;
    float _rank;
    int _outdegree;
    const std::string _url;
    std::unordered_set<Page*> _links;

  public:
    float rank() const { return _rank; }
    const std::string& url() const { return _url; }
    Page(const std::string& url) : _rank(0.0f), _outdegree(0), _url(url) {};
    void add_link(Page *page);
    friend void pagerank(const std::set<Page>& pages, int p);
};

void Page::add_link(Page *page)
{
  if (_links.emplace(page).second)
    ++page->_outdegree;
}

bool operator<(const Page& lhs, const Page& rhs) { return lhs.url() < rhs.url(); }
bool operator<(const Page& lhs, const std::string& rhs) { return lhs.url() < rhs; }
bool operator<(const std::string& lhs, const Page& rhs) { return lhs < rhs.url(); }

void pagerank(const std::set<Page>& pages, int p)
{
  // initialize ranks
  for (auto it = pages.begin(); it != pages.end(); it++)
    const_cast<Page&>(*it)._rank = 1.0f / pages.size();

  while (--p) {
    // save the old values of each rank
    for (auto it = pages.begin(); it != pages.end(); it++) {
      Page& page = const_cast<Page&>(*it);
      page._old_rank = page._rank;
    }
    for (auto it = pages.begin(); it != pages.end(); it++) {
      Page& page = const_cast<Page&>(*it);
      page._rank = 0.0f;
      for (auto it = page._links.begin(); it != page._links.end(); it++) {
        Page *src = *it;
        page._rank += src->_old_rank / src->_outdegree;
      }
    }
  }
}

void pagerank_display(std::ostream& out, const std::set<Page>& pages)
{
  std::ios::fmtflags original_flags = out.flags(std::ios::fixed);
  int original_precision = out.precision(2);
  for (auto it = pages.begin(); it != pages.end(); it++)
    out << it->url() << ' ' << it->rank() << '\n';
  out.flags(original_flags);
  out.precision(original_precision);
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
    dest.add_link(&src);
  }
  pagerank(pages, p);
  pagerank_display(out, pages);
}

#ifndef PAGERANK_NO_MAIN

#include <iostream>

int main()
{
  pagerank_shell(std::cin, std::cout);
  return 0;
}

#endif
