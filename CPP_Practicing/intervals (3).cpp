#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

class CRange
{
public:
  // constructor
  long long m_hi;
  long long m_lo;
  CRange(long long, long long);
  bool includes(const long long) const;
  bool includes(const CRange) const;
  bool isInteger() const;
  bool operator==(const CRange) const;
  friend ostream & operator << (ostream & , const CRange);

private:
  // todo
};

CRange::CRange(long long lo, long long hi)
{
  if (lo > hi)
    throw logic_error("Invalid interval");
  m_hi = hi;
  m_lo = lo;
}
bool CRange::includes(const long long x) const
{
  if (x >= m_lo && x <= m_hi)
    return true;
  return false;
}
bool CRange::includes(const CRange x) const
{
  if (x.m_lo >= m_lo && x.m_hi <= m_hi)
    return true;
  return false;
}
bool CRange::isInteger() const
{
  return m_hi == m_lo;
}

bool CRange::operator==(const CRange another) const
{
  return m_hi == another.m_hi && m_lo == another.m_lo;
}

ostream & operator << (ostream & os , const CRange src)

{
  ios_base::fmtflags current_flag = os.flags();

  os << dec;
  if(src.m_hi == src.m_lo)
  { 
    // ostringstream oss;
    // oss << src.m_hi;
    // os << oss.str();
    // os << setbase(10) << src.m_hi;
    os << src.m_hi;
    
  }
  else
  {
    // ostringstream lo,hi;
    // lo << src.m_lo;
    // hi << src.m_hi;
    // os << "<" <<  lo.str() << ".."  << hi.str() << ">";
    // os <<  "<"<< setbase(10) <<  src.m_lo << ".." << setbase(10) << src.m_hi << ">";
    os <<  "<"<< src.m_lo << ".." << src.m_hi  << ">";

    
  }
  os.flags(current_flag);
  return os;
}





class CRangeList
{
public:
  // constructor
  CRangeList() = default;
  CRangeList(const vector<CRange> &);
  CRangeList(const CRangeList & );
  CRangeList(initializer_list<CRange>);
  // includes long long / range
  bool includes(const long long) const;
  bool includes(const CRange) const;
  // += range / range list
  CRangeList operator+(const CRange);
  friend CRangeList operator+(const CRange,const CRange);
  CRangeList& operator+=(const CRange);
  CRangeList operator+(const CRangeList) ;
  CRangeList& operator+=(const CRangeList);
  // -= range / range list
  CRangeList operator-(const CRange);
  friend CRangeList operator-(const CRange,const CRange);
  CRangeList& operator-=(const CRange);
  CRangeList operator-(const CRangeList) ;
  CRangeList& operator-=(const CRangeList);
  // = range / range list
  CRangeList &operator=(const CRangeList &src);
  CRangeList &operator=(const CRange);
  // operator ==
  bool operator==(const CRangeList) const;
  // operator !=
  bool operator!=(const CRangeList) const;
  // operator <<
  friend ostream &operator<<(ostream &, const CRangeList);
  void simplify();
  vector<CRange>::const_iterator begin() const;
  vector<CRange>::const_iterator end() const;

private:
  // todo
  vector<CRange> db;
  static bool compare(const CRange &, const CRange &);
};

CRangeList::CRangeList(const CRangeList & src)
{
  db = src.db;
}


// simplification method
void CRangeList::simplify()
{
  vector<CRange> merged;
  //create a new vector
  //loop over the original vector
  for (CRange a : db)
  { 
    //new vector empty or last range in the new empty is overlapped by the range
    if (merged.empty() || (merged.back().m_hi + 1 < a.m_lo && merged.back().m_hi != LLONG_MAX))
      merged.push_back(a);
    else
    {
      //if the range overlaps merge it
      merged.back().m_hi = max(merged.back().m_hi, a.m_hi);
    }
  }
  db = merged;
}

// compare
bool CRangeList::compare(const CRange &a, const CRange &b)
{
  return a.m_lo < b.m_lo;
}

// constructor
CRangeList::CRangeList(const vector<CRange> &data)
{
  db = data;
}

CRangeList::CRangeList(initializer_list<CRange> ranges):db(ranges)
{
  sort(db.begin(),db.end(),compare);
  this->simplify();
}


// includes

bool CRangeList::includes(const CRange x) const
{
  auto index = lower_bound(db.begin(), db.end(), x, compare);
  if (index != db.end() && index->includes(x))
  {
    return true;
  }
  if( index != db.begin())
  {
    index--;
    if(index->includes(x))
      return true;
  }
  return false;
}

bool CRangeList::includes(const long long x) const
{
  return this->includes(CRange(x,x));
}

// = range / range list
CRangeList &CRangeList::operator=(const CRange src)
{
  db.clear();
  db.push_back(src);
  return *this;
}
CRangeList &CRangeList::operator=(const CRangeList &src)
{
  if (&src == this)
    return *this;
  db = src.db;
  return *this;
}


//+ , +=
CRangeList CRangeList::operator+(const CRange other)
{
  CRangeList l(*this);
  auto index = lower_bound(l.db.begin(),l.db.end(),other,compare);
  l.db.insert(index,other);
  l.simplify();
  return l;
}

CRangeList operator+(const CRange a,const CRange b)
{
  vector<CRange> data;
  data.push_back(a);
  if(b.m_lo > a.m_lo)
    data.push_back(b);
  else
    data.insert(data.begin(),b);
  CRangeList result(data);
  result.simplify();
  return result;
}

CRangeList& CRangeList::operator+=(const CRange other)
{
  return *this  = *this + other;
}

CRangeList CRangeList::operator+(const CRangeList other) 
{ 
  CRangeList l(*this);
  for(CRange range: other.db)
    l += range;
  return l;
}


CRangeList& CRangeList::operator+=(const CRangeList other)
{
  return *this = *this + other;
}


//-,-=

CRangeList CRangeList::operator-(const CRange other)
{ 

  // cout << "LIST - Range" << endl;
  // cout << *this << "------" << other << endl;
  CRangeList l(*this);
  auto lower_index = lower_bound(l.db.begin(),l.db.end(),other,compare);
  if(lower_index !=l.db.begin())
    lower_index--;
  for(auto it = lower_index ; it != l.db.end() && other.m_hi >= it->m_lo ; ++it)
  { 
    if(other.m_lo > it->m_hi || other.m_hi < it->m_lo)
      continue;
    if(other.m_lo <= it->m_lo && other.m_hi >= it->m_hi)
    {
      // cout << "Covering the whole range" << endl;
      l.db.erase(it);
      it--;
    }
    else if(other.m_lo > it->m_lo && other.m_lo <= it->m_hi && other.m_hi >= it->m_hi)
    { 
      // cout << "Case 1" << endl;
      
      it->m_hi = other.m_lo-1;

    }
    else if(other.m_lo <= it->m_lo && other.m_hi >= it->m_lo && other.m_hi < it->m_hi)
    { 
      // cout << "Case 2" << endl;
      it->m_lo = other.m_hi+1;

    }
    else
    { 
      // cout << "Sandwich" << endl;
      CRange new_range(other.m_hi+1,it->m_hi);
      it->m_hi = other.m_lo-1;
      l += new_range;
      return l;
    }
  }
  l.simplify();
  return l;

}





CRangeList operator-(const CRange a,const CRange b)
{
  vector<CRange> data;
  if (b.m_lo <= a.m_lo && b.m_hi >= a.m_hi)
    return CRangeList(data);
  if ( b.m_lo > a.m_hi || b.m_hi < a.m_lo)
  {
    data.push_back(a);
    return CRangeList(data);
  }
  if (b.m_lo > a.m_lo && b.m_lo <= a.m_hi && b.m_hi >= a.m_hi)
  { 
    CRange new_range(a.m_lo,b.m_lo-1);
    data.push_back(new_range);
    return CRangeList(data);
  }
  if(b.m_lo <= a.m_lo && b.m_hi >= a.m_lo &&  b.m_hi < a.m_hi)
  {
    CRange new_range (b.m_hi+1,a.m_hi);
    data.push_back(new_range);
    return CRangeList(data);
  }

  CRange new_range2(b.m_hi+1,a.m_hi);
  CRange new_range1(a.m_lo,b.m_lo-1);
  data.push_back(new_range1);
  data.push_back(new_range2);
  return CRangeList(data);
}


CRangeList& CRangeList::operator-=(const CRange other)
{
  return *this  = *this - other;
}

CRangeList CRangeList::operator-(const CRangeList other) 
{ 
  CRangeList l(*this);
  for(CRange range: other.db)
    l -= range;
  return l;
}

CRangeList& CRangeList::operator-=(const CRangeList other)
{
  return *this = *this - other;
}



bool CRangeList::operator==(const CRangeList other) const
{
  return db == other.db;
}
bool CRangeList::operator!=(const CRangeList other) const
{
  return db != other.db;
}

ostream &operator<<(ostream &os, const CRangeList x)
{
  ios_base::fmtflags current_flag = os.flags();
  os << dec;
  os << "{";
  for (size_t i = 0; i < x.db.size(); ++i)
  {
    if (i != 0)
      os << ",";
    // if (x.db[i].isInteger())
    //   os << x.db[i].m_lo;
    // else
    //   os << "<" << x.db[i].m_lo << ".." << x.db[i].m_hi << ">";
    os << x.db[i];
  }
  os << "}";
  os.flags(current_flag);
  return os;
}

  vector<CRange>::const_iterator CRangeList::begin() const
  {
    return db.begin();
  }
  vector<CRange>::const_iterator CRangeList::end() const
  {
    return db.end();
  }


#ifndef __PROGTEST__
string toString(const CRangeList &x)
{
  ostringstream oss;
  oss << x;
  return oss.str();
}

int main(void)
{
  CRangeList a, b;

  assert(sizeof(CRange) <= 2 * sizeof(long long));
  a = CRange(5, 10);
  assert(a==a);
  a += CRange(25, 100);
  assert(toString(a) == "{<5..10>,<25..100>}");
  a += CRange(-5, 0);
  a += CRange(8, 50);
  assert(toString(a) == "{<-5..0>,<5..100>}");
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );

  a -= CRange ( -400, -400 );
  // CRangeList c;
  // c += CRange (2,4) + CRange (6,8) + CRange (14,16) +  CRange (16,18) + CRange(10,12);
  // cout << c << endl;
  // c -= CRange (3,15);
  // cout << "Subtracting" << endl;
  // cout << c << endl;
  // cout << a << endl;

  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  // cout << a << endl;
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  // cout << b << endl;
  assert ( b . includes ( 2900 ) );
  assert ( b . includes ( CRange ( 15, 15 ) ) );
  assert ( b . includes ( CRange ( -350, -350 ) ) );
  assert ( b . includes ( CRange ( 100, 200 ) ) );
  assert ( !b . includes ( CRange ( 800, 900 ) ) );
  assert ( !b . includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );

  // CRangeList d;
  // d += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ); // - CRange (170,170);
  // d -= CRange (170,170);
  // cout << "D:" << d << endl;
  // d = CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 );
  
  // cout << "D:" << d << endl;
  
  // cout << "HEREEE" << endl;
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  // cout << "B:" << b << endl;

  

  
  // cout << b << endl;
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  // cout << "HERE" << endl;
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  // cout << b << endl;
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
#ifdef EXTENDED_SYNTAX
  CRangeList x{{5, 20}, {150, 200}, {-9, 12}, {48, 93}};
  // cout << x << endl;
  assert(toString(x) == "{<-9..20>,<48..93>,<150..200>}");
  ostringstream oss;
  oss << setfill('=') << hex << left;
  for (const auto &v : x + CRange(-100, -100))
    oss << v << endl;
  oss << setw(10) << 1024;
  // cout << oss.str();
  // int test = -100;
  // cout << test << endl;
  assert(oss.str() == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======");
#endif /* EXTENDED_SYNTAX */

  CRangeList o;
  o += CRange(1,1);
  o + CRange(2,2);
  cout << o << endl;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
