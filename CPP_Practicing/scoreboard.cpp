#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */



template <typename M_>
class CContest
{
  public:
    // default constructor
    CContest(){}
    // destructor
    ~CContest(){}
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch (const string & contestant1 , const string & contestant2,const M_  & result)
    { 
      for(size_t i = 0 ; i < m_matches.size() ; ++i)
      {
        if((m_matches[i].first.first == contestant1 && m_matches[i].first.second == contestant2) || (m_matches[i].first.first == contestant2 && m_matches[i].first.second == contestant1 ))
        {
          throw logic_error("Already existed");
        }
      }
      contestants.insert(contestant1);
      contestants.insert(contestant2);
      m_matches.push_back({{contestant1,contestant2},result});
      return *this;
    } 
    bool isOrdered (const function<int(const M_&)> comparator ) const noexcept
    { 
      vector<int> number_of_wins;
      for(auto & contestant : contestants)
      {
        int wins = BFS(contestant,comparator);
        auto index = lower_bound(number_of_wins.begin(),number_of_wins.end(),wins);
        // cout << contestant << "-----" << wins << endl;
        // if(index != number_of_wins.end())
        // {
        //   cout << *index << " vs " << wins << endl;
        // }
        // cout << contestant << " : " << wins << endl;
        if(index != number_of_wins.end() && *index == wins)
        { 
          //  cout << endl;

          return false;
        }
        else
        { 
          number_of_wins.insert(index,wins);
        }
      }
      // cout << endl;
      return true;
    }
    list<string> results (const function<int(const M_&)> comparator ) const
    { 
      map <int,string> order;
      list<string> result;
      vector<int> number_of_wins;
      vector<pair<int,string>> elements;
      for(auto & contestant : contestants)
      {
        int wins = BFS(contestant,comparator);
        auto index = lower_bound(number_of_wins.begin(),number_of_wins.end(),wins);
        // cout << contestant << "-----" << wins << endl;
        // if(index != number_of_wins.end())
        // {
        //   cout << *index << " vs " << wins << endl;
        // }
        if(index != number_of_wins.end() && *index == wins)
        { 
          throw logic_error("Cant be ordered");
        }
        else
        { 
          number_of_wins.insert(index,wins);
          elements.push_back({wins,contestant});
        }
      }
      for(auto & element : elements)
      {
        order[element.first] = element.second;
      }
      for(auto it = order.rbegin() ; it != order.rend() ; it++)
      {
        result.push_back(it->second);
      }
      return result;
    }
  private:
    // todo
    set<string> contestants;
    vector <pair< pair<string,string> , M_>> m_matches;
    int BFS(const string & contestant, function<int(const M_&)> comparator) const
    { 
      int count = 0;
      queue<string> q;
      unordered_map<string, bool> visited;
      
      q.push(contestant);

      while(!q.empty())
      {
        string front = q.front();
        q.pop();
        if(visited[front])
        {
          continue;
        }
        count++;
        // visited.insert(front);
        visited[front] = true;

        for(size_t i = 0 ; i < m_matches.size() ; ++i)
        {
          if(m_matches[i].first.first == front)
          {
            if(comparator(m_matches[i].second) > 0)
            { 
              q.push(m_matches[i].first.second);
            }
          }
          else if(m_matches[i].first.second == front)
          { 
            if(comparator(m_matches[i].second) < 0)
            { 
              q.push(m_matches[i].first.first);
            }
          }
          else
          {
            continue;
          }
        }

      }

        return count;


    }

};






#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

void printlist(list<string> strings)
{
  for(string a : strings)
  {
    cout << a << endl;
  }
}

int                main                                    ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
