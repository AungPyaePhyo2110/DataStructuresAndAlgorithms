#ifndef __PROGTEST__
#include <cassert>
#include <cctype>
#include <cmath>
#include <cinttypes>
#include <limits>

#include <memory>
#include <string>
#include <random>
#include <algorithm>

#include <iostream>
#include <iomanip>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

// rooms are numbered consecutively starting with zero
using Room = unsigned;
using Corridor = std::pair<Room, Room>;

inline constexpr Room NO_ROOM = std::numeric_limits<Room>::max();

struct Map {
  Room s;
  std::vector<Room> beds;
  std::vector<unsigned> food;
  std::vector<Corridor> corridors;
};

struct Result {
  Room final_bed;
  unsigned food_eaten;
};

bool operator == (Result a, Result b) {
  return a.final_bed == b.final_bed && (
    a.food_eaten == b.food_eaten || a.final_bed == NO_ROOM);
}

bool operator != (Result a , Result b)
{
  return !(a == b);
}

std::ostream& operator << (std::ostream& out, Result r) {
  out << "(room: ";
  if (r.final_bed == NO_ROOM) out << "NO_ROOM";
  else out << r.final_bed;
  return out << ", food: " << r.food_eaten << ")";
}

#endif


void dfs3(size_t & time , std::vector<size_t> & finisingtime , size_t node , std::vector<bool> & closed , std::vector<bool> & visited, std::stack<Room> & myStack, std::vector<std::vector<Room>> & neighbours)
{
  std::stack<size_t> s;
  s.push(node);
  size_t start = time;
  while(!s.empty())
  {
    size_t current = s.top();
    // std::cout << "Current   " << current << std::endl;

    s.pop();
    if(!visited[current])
    {
      visited[current] = true;
      s.push(current);
    }
    else
    {
      if(!closed[current])
      {
        finisingtime[time] = current;
        time++;
        closed[current] = true;
        // std::cout << "Closing   " << current << std::endl;
      }
    }
    for(const Room & neighbour: neighbours[current])
    {
      // std::cout << neighbour << " , ";
      if(visited[neighbour])continue;
      s.push(neighbour);
    }
    // std::cout << std::endl;
  }

  

  for(size_t i = start ; i < time ; i++)
  {
    // std::cout << finisingtime[i] << std::endl;
    myStack.push(finisingtime[i]);
  }
  // std::cout << "--------------------" << std::endl;

}



void dfs2(size_t start, bool & startFound , size_t node , int group, std::vector<int> & SCC ,
   std::vector<unsigned> & SCCFood ,const std::vector<unsigned>& mapFood,
   std::vector<std::vector<int>> & dagNeighbours ,const std::vector<std::vector<Room>> & Gneighbours)
{
  std::stack<size_t> s; 
  SCC[node] = group;
  // std::vector<bool> alreadyAddedEdge (mapFood.size(),false);
  
  s.push(node);
  while(!s.empty())
  {
    size_t current = s.top();
    if(current == start)
      startFound = true;
    s.pop();
    SCCFood[group] += mapFood[current];
    for(const Room & neighbour : Gneighbours[current])
    {
      // std::cout << neighour << " , ";
      int* neighbourSCC = &SCC[neighbour];
      if(*neighbourSCC == -1 )
      {
        *neighbourSCC = group;
        s.push(neighbour);
      }
      else
      {
        // if(!alreadyAddedEdge[SCC[neighour]])
        if(group !=  SCC[neighbour] )
        {
          dagNeighbours[group].push_back(SCC[neighbour]);
          // std::cout << group << " ,  " << SCC[neighour] << std::endl;
          // alreadyAddedEdge[SCC[neighour]] = true;
        }
      }
    }

  }

}

void printStack(std::stack<Room>  originalStack) {
    std::stack<Room> tempStack;
    // Move elements from original to temp, printing them
    while (!originalStack.empty()) {
        Room top = originalStack.top();
        std::cout << top << std::endl; // Print the top element
        tempStack.push(top);
        originalStack.pop();
    }

    // Move elements back from temp to original
    while (!tempStack.empty()) {
        originalStack.push(tempStack.top());
        tempStack.pop();
    }
    // std::cout <<"//////////////////////////////////////" << std::endl;
}

Result find_lost_cat(const Map& map) {
  // TODO
  //Build condensation graph of the map
  std::vector<std::vector<Room>> neighbours(map.food.size()+1);
  std::vector<std::vector<Room>> reversedGraphneighbours(map.food.size()+1);

  for(size_t i = 0 ; i < map.corridors.size() ; i++)
  {
    neighbours[map.corridors[i].first] .push_back(map.corridors[i].second);
    reversedGraphneighbours[map.corridors[i].second].push_back(map.corridors[i].first);
  }

  // std::stack<Room> stack;
  // std::vector<bool> visited(map.food.size()+1 , false);
  // for(size_t i = 0 ; i < map.food.size(); i++)
  // {
  //   if(!visited[i])
  //     dfs1(i,visited,stack,reversedGraphneighbours);
  // }
  std::stack<Room> stack;
  std::vector<bool> iterativevisited(map.food.size()+1 ,false);
  std::vector<bool> closed(map.food.size()+1,false);
  size_t time = 0;
  std::vector<size_t> finisingtime(neighbours.size()+5);


  for(size_t i = 0 ; i < map.food.size(); i++)
  {
    if(!iterativevisited[i])
    {
      // std::cout << "CALLING DFS ON "  << i << std::endl; 
      dfs3(time,finisingtime,i,closed,iterativevisited,stack,reversedGraphneighbours);
    }
  }

  // std::cout << "Printing the recursive stack" << std::endl;
  // printStack(stack);

  // std::cout << "Printing the iterative stack" << std::endl;
  // printStack(stack);




  


  int group = 0;
  std::vector<int> SCC(map.food.size()+1 , -1 );


  std::vector<unsigned> dagFood(map.food.size()+1 , 0);
  std::vector<std::vector<int>> dagNeighbours(map.food.size()+1);
  bool startFound = false;

  while(!stack.empty())
  {
    size_t current = stack.top();
    stack.pop();
    if(SCC[current] == -1) 
    {
      dfs2(map.s , startFound ,  current, group++ , SCC , dagFood , map.food , dagNeighbours , neighbours);
    }
    if(startFound)
      break;
  }
  int dagStart = SCC[map.s];








  //TOPSORT
  std::queue<int> Q;
  std::vector<size_t> inDegrees(group,0);
  for(int i = 0 ; i < group ; i++)
  {
    for(const Room neighbour : dagNeighbours[i])
    {
      inDegrees[neighbour]++;
    }
  }

  for(size_t i = 0 ; i < inDegrees.size() ; i++)
  {
    if(inDegrees[i]==0)
      Q.push(i);
  }

  std::vector<int> topSortedVertices;
  size_t startVertexIndex = 0;
  while(!Q.empty())
  {
    int current = Q.front(); 
    Q.pop();
    if(current == dagStart)
    {
      startVertexIndex = topSortedVertices.size();
    }
    topSortedVertices.push_back(current);
    for(const int neighbour: dagNeighbours[current])
    {
      inDegrees[neighbour]--;
      if(inDegrees[neighbour]==0)
        Q.push(neighbour);
    }
  }


  

  //find the availabe in each vertices
  std::vector<unsigned> totalFood(group,0);

  totalFood[dagStart] = dagFood[dagStart];

  std::vector<bool> reachableSCC(topSortedVertices.size(),false);
  reachableSCC[dagStart] = true;

  for(size_t i = startVertexIndex ; i < topSortedVertices.size() ; i++)
  {
    for(const int neighbour: dagNeighbours[topSortedVertices[i]])
    { 
      if(reachableSCC[topSortedVertices[i]])
      {
        reachableSCC[neighbour] = true;
        if(totalFood[neighbour] < totalFood[topSortedVertices[i]]+dagFood[neighbour])
        {
          totalFood[neighbour] = totalFood[topSortedVertices[i]]+dagFood[neighbour];
        }
      }

    }
  }




  //final result
  Result result;
  result.food_eaten = 0;
  result.final_bed = NO_ROOM;
  


  for(int i = map.beds.size()-1 ; i >=0 ; i--)
  {
    if(SCC[map.beds[i]] != -1 && reachableSCC[SCC[map.beds[i]]] )
    {
      if(totalFood[SCC[map.beds[i]]] >= result.food_eaten)
      {
        result.food_eaten = totalFood[SCC[map.beds[i]]];
        result.final_bed = map.beds[i];
      }
    }
  }

  return result;
  
}


#ifndef __PROGTEST__

const std::vector<std::pair<Result, Map>> test_data = {
  // Simple
  { { 1, 4 }, Map{ 0, { 1 },
    { 0, 1, 1, 2, 1 },
    {
      { 0, 2 }, { 0, 3 },
      { 2, 4 }, { 3, 4 },
      { 0, 1 }, { 4, 1 }
    }
  }},
  { { 7, 8 }, Map{ 3, { 7 },
    {
       1,  1,  0,  0,  1,  2,  0,  0,  0,  1,  2,  2,
       2,
    }, {
      { 0,  2}, { 1,  8}, { 2,  4}, { 2,  7}, { 2, 11}, { 3,  6},
      { 3,  9}, { 3, 10}, { 4,  7}, { 5,  8}, { 6,  1}, { 6,  5},
      { 6,  8}, { 8,  0}, { 8,  2}, { 8, 12}, { 9,  6}, {10,  6},
      {11,  7}, {12,  2},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 4, { 5 },
    {
       2,  1,  0,  0,  2,  0,  2,  2,  1,  0,  1,  0,
       1,
    }, {
      { 0,  2}, { 1,  5}, { 3,  0}, { 3,  2}, { 3, 10}, { 4,  3},
      { 5,  6}, { 5,  8}, { 5, 11}, { 6, 11}, { 7,  5}, { 8, 11},
      { 9,  1}, { 9,  5}, { 9,  7}, {10,  2}, {11,  3}, {11,  4},
      {11, 12}, {12,  3},
    }
  }},

  // Acyclic
  { { NO_ROOM, 0 }, Map{ 2, { 4 },
    {
       6,  1,  3,  0,  3,  6,  4,  0,
    }, {
      { 0,  1}, { 0,  6}, { 1,  6}, { 3,  0}, { 3,  2}, { 3,  6},
      { 4,  1}, { 4,  5}, { 5,  0}, { 5,  1}, { 5,  6}, { 6,  2},
      { 7,  2}, { 7,  3}, { 7,  4},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 3, { 7 },
    {
       0,  0,  0,  2,  0,  6,  6,  0,
    }, {
      { 1,  0}, { 1,  3}, { 1,  5}, { 2,  1}, { 3,  5}, { 4,  2},
      { 5,  6}, { 7,  3}, { 7,  4}, { 7,  5}, { 7,  6},
    }
  }},
  { { 6, 6 }, Map{ 0, { 6 },
    {
       2,  2,  2,  1,  0,  0,  4,  6,
    }, {
      { 0,  5}, { 0,  6}, { 2,  1}, { 3,  2}, { 4,  1}, { 4,  2},
      { 5,  7}, { 6,  1}, { 6,  2}, { 6,  4}, { 6,  5}, { 6,  7},
      { 7,  1},
    }
  }},

  // General
  { { 8, 23 }, Map{ 1, { 8 },
    {
       3,  6,  4,  0,  6,  4,  6,  3,  0,  3,
    }, {
      { 0,  8}, { 1,  5}, { 2,  6}, { 2,  9}, { 3,  0}, { 3,  2},
      { 3,  8}, { 4,  2}, { 4,  8}, { 5,  3}, { 5,  8}, { 6,  0},
      { 7,  2}, { 7,  3}, { 8,  1}, { 8,  9},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 6, { 7 },
    {
       5,  0,  0,  1,  5,  0,  0,  0,  2,  5,
    }, {
      { 0,  4}, { 0,  9}, { 1,  4}, { 2,  3}, { 3,  5}, { 3,  6},
      { 4,  0}, { 5,  8}, { 6,  1}, { 6,  4}, { 7,  0}, { 7,  1},
      { 7,  8}, { 9,  4},
    }
  }},
  { { 6, 14 }, Map{ 5, { 6 },
    {
       5,  0,  1,  3,  2,  3,  6,  5,  0,  0,
    }, {
      { 0,  4}, { 0,  6}, { 1,  0}, { 1,  5}, { 2,  4}, { 3,  7},
      { 4,  2}, { 5,  0}, { 5,  1}, { 5,  4}, { 6,  4}, { 7,  9},
      { 8,  4}, { 8,  6}, { 9,  8},
    }
  }},
  { { 2, 15 }, Map{ 1, { 2 },
    {
       1,  0,  6,  0,  0,  0,  5,  0,  5,  4,
    }, {
      { 1,  3}, { 1,  7}, { 3,  2}, { 3,  7}, { 3,  9}, { 4,  5},
      { 4,  7}, { 5,  3}, { 5,  8}, { 7,  3}, { 7,  8}, { 8,  1},
      { 8,  6}, { 9,  5}, { 9,  8},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 9, { 3 },
    {
       1,  4,  2,  3,  0,  1,  0,  4,  1,  0,
    }, {
      { 0,  1}, { 1,  0}, { 1,  5}, { 1,  9}, { 2,  0}, { 2,  5},
      { 2,  7}, { 3,  9}, { 4,  9}, { 6,  2}, { 6,  3}, { 6,  5},
      { 7,  0}, { 7,  2}, { 7,  3}, { 7,  8}, { 8,  2}, { 9,  4},
    }
  }},
  { { 2, 7 }, Map {
  4, {2, 4, 7},  // Mimi starts in room 4, prefers bed 2 then bed 4 then bed 7
  {0, 0, 1, 4, 2, 0, 0, 0},  // Food in rooms (index corresponds to room number, added zeros for missing rooms)
  {{0, 1}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {3, 2}, {4, 5}, {4, 6}, {5, 1}, {5, 7}, {6, 3}, {6, 5}}  // Corridors
}},
{ { 0, 0 }, Map {
  0, {0},  // Mimi starts in room 0, only bed 0
  {0},     // Food array with only one element (room 0)
  {{}}    // No corridors (isolated room)
}},
{
  {0,5},Map{
    0,{0},
    {0,1,1,1,1,1},
    {
      {0,1},{1,2},{2,3},{3,4},{4,5},{5,0}
    }
  }
}


};




int main() {

  int i = 0;
  for (const auto& [ exp_res,  M ] : test_data) {
    std::cout << "No --- " << i++ << std::endl;
    Result stud_res = find_lost_cat(M);
    if (stud_res != exp_res) {
      std::cout << "Fail: " << exp_res << " != " << stud_res << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
  }

  return 0;
}

#endif


