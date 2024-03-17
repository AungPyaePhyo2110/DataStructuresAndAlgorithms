#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>


enum Point : size_t {};

struct Path {
  Point from, to;
  unsigned length;

  Path(size_t f, size_t t, unsigned l) : from{f}, to{t}, length{l} {}

  friend bool operator == (const Path& a, const Path& b) {
    return std::tie(a.from, a.to, a.length) == std::tie(b.from, b.to, b.length);
  }
  
  friend bool operator != (const Path& a, const Path& b) { return !(a == b); }
};

#endif

std::vector<Path> longest_track(size_t points, const std::vector<Path>& all_paths) {
  std::vector<Path> result;

  std::vector<std::vector<Path> > forward_edges ( points );
  std::vector<std::vector<Path> > backward_edges ( points );


  for ( const Path & path : all_paths)
  {
    forward_edges[path.from].push_back(path);
    backward_edges[path.to].push_back(path);
  }

  std::queue<size_t> Q;
  std::vector<int> inDegrees ( points, 0);
  std::vector<size_t> topSortedList;

  for ( const Path & path: all_paths)
  {
    inDegrees[path.to]++;
  }

  for ( size_t i = 0 ; i < points;  ++i) 
  {
    if(inDegrees[i] == 0)
    {
      Q.push(i);
    }
  }


  while ( ! Q.empty() )
  {
    size_t v = Q.front();
    Q.pop();
    topSortedList.push_back(v);

    for( Path w : forward_edges[v])
    {
      if(--inDegrees[w.to] == 0)
      {
        Q.push(w.to);
      }
    }
  }


  std::vector<unsigned> longest_path(points,0);
  std::vector<Path> predecessors(points,Path(points,points,0));

  unsigned max_length = 0;
  size_t last_point = points;

  for ( size_t v : topSortedList)
  {
    for ( const Path & neighbour : backward_edges[v])
    {
      if(neighbour.length + longest_path[neighbour.from] > longest_path[v])
      {
        longest_path[v] = neighbour.length + longest_path[neighbour.from];
        predecessors[v] = neighbour;
      }
    }
    

    if(longest_path[v] > max_length)
    {
      max_length = longest_path[v];
      last_point = v;
    }

    // std::cout << v << "'s MAX LENGTH : " << max_length << std::endl; 


  }


  size_t current_point = last_point;
  while(current_point < points)
  {
    size_t next_point = predecessors[current_point].from;
    // for ( const auto & path : all_paths)
    // {
    //   if(path.from == next_point && path.to == current_point)
    //   {
    //     result.insert(result.begin(),path);
    //   }

    // }
    Path dummyPath = Path(points,points,0);
    if(predecessors[current_point] != dummyPath)
    {
    result.insert(result.begin(),predecessors[current_point]);
    }


    current_point = next_point;

  }

  // for ( size_t i = 0 ; i < result.size() ; ++i )
  // {
  //   std::cout << "FROM " << result[i].from << " -- TO " << result[i].to << "  length - " << result[i].length << std::endl;
  // }

  // std::cout << std::endl;


  return result;
}




#ifndef __PROGTEST__


struct Test {
  unsigned longest_track;
  size_t points;
  std::vector<Path> all_paths;
};

inline const Test TESTS[] = {
  {13, 5, { {3,2,10}, {3,0,9}, {0,2,3}, {2,4,1} } },
  {11, 5, { {3,2,10}, {3,1,4}, {1,2,3}, {2,4,1} } },
  {16, 8, { {3,2,10}, {3,1,1}, {1,2,3}, {1,4,15} } },
  {28, 6, {{0, 1, 10},{0, 1, 2},{0, 2, 5},{1, 3, 5},{2, 3, 7},{2, 4, 8},{3, 5, 13},{4, 5, 15}}} 
  
};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Fail: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool run_test(const Test& t) {
  auto sol = longest_track(t.points, t.all_paths);

  unsigned length = 0;
  for (auto [ _, __, l ] : sol) length += l;

  CHECK(t.longest_track == length,
    "Wrong length: got %u but expected %u", length, t.longest_track);

  for (size_t i = 0; i < sol.size(); i++) {
    CHECK(std::count(t.all_paths.begin(), t.all_paths.end(), sol[i]),
      "Solution contains non-existent path: %zu -> %zu (%u)",
      sol[i].from, sol[i].to, sol[i].length);

    if (i > 0) CHECK(sol[i].from == sol[i-1].to,
      "Paths are not consecutive: %zu != %zu", sol[i-1].to, sol[i].from);
  }

  return true;
}
#undef CHECK

int main() {
  int ok = 0, fail = 0;

  for (auto&& t : TESTS) (run_test(t) ? ok : fail)++;
  
  if (!fail) printf("Passed all %i tests!\n", ok);
  else printf("Failed %u of %u tests.\n", fail, fail + ok);
}

#endif


// int main() {
//   int ok = 0, fail = 0;

//   for (auto&& t : TESTS)
//   {
//     (run_test(t) ? ok : fail)++;
//     std::cout<<"----------------------------------------------" << std::endl;
//   } 

  
//   if (!fail) printf("Passed all %i tests!\n", ok);
//   else printf("Failed %u of %u tests.\n", fail, fail + ok);
// }

// #endif


