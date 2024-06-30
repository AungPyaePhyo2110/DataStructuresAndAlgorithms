#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
public:
  //---------------------------------------------------------------------------------------------
  CDate(int y,
        int m,
        int d)
      : m_Year(y),
        m_Month(m),
        m_Day(d)
  {
  }
  //---------------------------------------------------------------------------------------------
  int compare(const CDate &x) const
  {
    if (m_Year != x.m_Year)
      return m_Year - x.m_Year;
    if (m_Month != x.m_Month)
      return m_Month - x.m_Month;
    return m_Day - x.m_Day;
  }
  //---------------------------------------------------------------------------------------------
  int year(void) const
  {
    return m_Year;
  }
  //---------------------------------------------------------------------------------------------
  int month(void) const
  {
    return m_Month;
  }
  //---------------------------------------------------------------------------------------------
  int day(void) const
  {
    return m_Day;
  }
  //---------------------------------------------------------------------------------------------
  friend ostream &operator<<(ostream &os,
                             const CDate &x)
  {
    char oldFill = os.fill();
    return os << setfill('0') << setw(4) << x.m_Year << "-"
              << setw(2) << static_cast<int>(x.m_Month) << "-"
              << setw(2) << static_cast<int>(x.m_Day)
              << setfill(oldFill);
  }
  //---------------------------------------------------------------------------------------------
private:
  int16_t m_Year;
  int8_t m_Month;
  int8_t m_Day;
};
#endif /* __PROGTEST__ */


//---------------------------------------------------------------------------------------------


class CInvoice
{
public:
  CInvoice(const CDate &date,
           const string &seller,
           const string &buyer,
           unsigned int amount,
           double vat);
  CDate date(void) const;
  string buyer(void) const;
  string seller(void) const;
  unsigned int amount(void) const;
  double vat(void) const;
  size_t order(void) const;


  bool operator==(const CInvoice &) const;
  bool operator<(const CInvoice &) const;
  bool operator!=(const CInvoice &) const;
  friend ostream &operator<<(ostream &, const CInvoice &);
  CInvoice(const CInvoice &);
  CInvoice &operator=(const CInvoice &);
  bool equal(const CInvoice &) const;
  void changeseller(const string &);
  void changebuyer(const string &);
  void changeorder(const size_t );

private:
  // todo
  CDate m_date;
  string m_seller;
  string m_buyer;
  unsigned int m_amount;
  double m_vat;
  size_t m_order;
};

CInvoice::CInvoice(const CDate &date, const string &seller, const string &buyer, unsigned int amount, double vat) : m_date(date), m_seller(seller), m_buyer(buyer), m_amount(amount), m_vat(vat), m_order(0)
{
}

CInvoice::CInvoice(const CInvoice &src) : m_date(CDate(src.m_date.year(), src.m_date.month(), src.m_date.day())),
                                          m_seller(src.m_seller), m_buyer(src.m_buyer), m_amount(src.m_amount), m_vat(src.m_vat), m_order(src.m_order)
{
}

CInvoice &CInvoice::operator=(const CInvoice &src)
{
  if (this == &src)
    return *this;
  m_date = src.m_date;
  m_seller = src.m_seller;
  m_buyer = src.m_buyer;
  m_amount = src.m_amount;
  m_vat = src.m_vat;
  m_order = src.m_order;
  return *this;
}

CDate CInvoice::date(void) const
{
  return m_date;
}
string CInvoice::buyer(void) const
{
  return m_buyer;
}
string CInvoice::seller(void) const
{
  return m_seller;
}
unsigned int CInvoice::amount(void) const
{
  return m_amount;
}
double CInvoice::vat(void) const
{
  return m_vat;
}

size_t CInvoice::order(void) const
{
  return m_order;
}


bool CInvoice::operator==(const CInvoice &other) const
{
  return m_date.year() == other.m_date.year() && m_date.month() == other.m_date.month() && m_date.day() == other.m_date.day() && m_seller == other.m_seller && m_buyer == other.m_buyer && m_amount == other.m_amount &&
         abs(m_vat - other.m_vat) < __DBL_EPSILON__ * m_vat * other.m_vat;
}

bool CInvoice::equal(const CInvoice & other) const
{
  return m_date.year() == other.m_date.year() && m_date.month() == other.m_date.month() && m_date.day() == other.m_date.day() && m_seller == other.m_seller && m_buyer == other.m_buyer && m_amount == other.m_amount &&
         abs(m_vat - other.m_vat) < __DBL_EPSILON__ * m_vat * other.m_vat;
}


bool CInvoice::operator!=(const CInvoice &other) const
{
  return !(*this == other);
}

bool CInvoice::operator<(const CInvoice &other) const
{

  if (m_date.compare(other.m_date) != 0)
  {
    return m_date.compare(other.m_date) < 0;
  }
  else if (m_seller != other.m_seller)
  {
    return m_seller < other.m_seller;
  }
  else if (m_buyer != other.m_buyer)
  {
    return m_buyer < other.m_buyer;
  }
  else if (m_amount != other.m_amount)
  {
    return m_amount < other.m_amount;
  }
  else
  {
    return m_vat < other.m_vat;
  }
}

ostream &operator<<(ostream &os, const CInvoice &src)
{
  return os << src.m_date << " , " << src.m_seller << " , " << src.m_buyer << " , " << src.m_amount << " , " << src.m_vat << "    Order :"  << src.m_order;
}

void CInvoice::changeseller(const string &new_seller)
{
  m_seller = new_seller;
}

void CInvoice::changebuyer(const string &new_buyer)
{
  m_buyer = new_buyer;
}


void CInvoice::changeorder(const size_t size )
{
  m_order = size;
}

//----------------------------------------------------------------------------------------------------------------------------------

class CSortOpt
{
public:
  static const int BY_DATE = 0;
  static const int BY_BUYER = 1;
  static const int BY_SELLER = 2;
  static const int BY_AMOUNT = 3;
  static const int BY_VAT = 4;
  CSortOpt(void);
  CSortOpt &addKey(int key,
                   bool ascending = true);
  vector<pair<int, bool>> keys;
  friend ostream &operator<<(ostream &, const CSortOpt);

private:
  // todo
};

CSortOpt::CSortOpt(void)
{
}

CSortOpt &CSortOpt::addKey(int key, bool ascending)
{
  keys.push_back(make_pair(key, ascending));
  return *this;
}

ostream &operator<<(ostream &os, const CSortOpt src)
{
  for (const auto &key : src.keys)
  {
    os << "Key: " << key.first << "Ascending: " << key.second << endl;
  }
  return os;
}

//----------------------------------------------------------------------------------------------------------------------------------
// helper_function

string changetolower(const string & str)
{ 
  string new_string = str;
  for_each(new_string.begin(), new_string.end(), [](char &c)
           { c = towlower(static_cast<unsigned char>(c)); });
  return new_string;
}

bool compare(const CInvoice &a, const CInvoice &b, const CSortOpt &sortBy)
{
  for (const auto &key : sortBy.keys)
  {
    switch (key.first)
    {
    case CSortOpt::BY_DATE:
      if (a.date().compare(b.date()) != 0)
      {
        // cout << "Sorting by date" << endl;
        return key.second ? (a.date().compare(b.date()) < 0) : (b.date().compare(a.date()) < 0);
      }
      break;
    case CSortOpt::BY_SELLER:
      if (changetolower(a.seller()) != changetolower(b.seller()))
      {
        // cout << "Sorting by seller" << endl;
        return key.second ? changetolower(a.seller()) < changetolower(b.seller()) : changetolower(b.seller()) < changetolower(a.seller());
      }
      break;
    case CSortOpt::BY_BUYER:
      if (changetolower(a.buyer()) != changetolower(b.buyer()))
      {
        // cout << "Sorting by buyer" << endl;
        return key.second ? changetolower(a.buyer()) < changetolower(b.buyer()) : changetolower(b.buyer()) < changetolower(a.buyer());
      }
      break;
    case CSortOpt::BY_AMOUNT:
      if (a.amount() != b.amount())
      {
        // cout << "Sorting by amount" << endl;

        return key.second ? (a.amount() < b.amount()) : (b.amount() < a.amount());
      }
      break;
    case CSortOpt::BY_VAT:
      if (!(abs(a.vat() - b.vat()) < __DBL_EPSILON__ * a.vat() * b.vat()))
      {
        // cout << "Sorting by vat" << endl;
        return key.second ? (a.vat() < b.vat()) : (b.vat() < a.vat());
      }
      break;
    default:
      break;
    }
  }
  return a.order() < b.order();
}

  //---------------------------------------------------------------------------------------------

struct CInvoicehash
{
  size_t operator()(const CInvoice &invoice) const
  {
    size_t result = 17;
    result += hash<int>()(invoice.date().year()) + hash<int>()(invoice.date().month()) + hash<int>()(invoice.date().day());
    result += hash<string>()(invoice.seller());
    result += hash<string>()(invoice.buyer());
    result += hash<int>()(invoice.amount());
    result += hash<double>()(invoice.vat());
    return result;
  }
};

struct CInvoiceEqual
{
  bool operator()(const CInvoice &a, const CInvoice &b) const
  {
    return a == b;
  }
};

  //---------------------------------------------------------------------------------------------

class Ccompany
{
private:
  string m_official_name;


public:
  unordered_map<CInvoice,bool,CInvoicehash,CInvoiceEqual> m_issue;
  unordered_map<CInvoice,bool,CInvoicehash,CInvoiceEqual> m_accept;
  Ccompany(const string & name = " ");
  string getOfficialName()const;
  Ccompany(const Ccompany &);
};


Ccompany::Ccompany(const string &name) : m_official_name(name)
{
}

string Ccompany::getOfficialName()const
{
  return m_official_name;
}

Ccompany::Ccompany(const Ccompany & src):m_official_name(src.m_official_name),m_issue(src.m_issue),m_accept(src.m_accept)
{
}


//----------------------------------------------------------------------------------------------------------------------------------

class CVATRegister
{
public:
  CVATRegister(void);
  bool registerCompany(const string &name);
  bool addIssued(const CInvoice &x);
  bool addAccepted(const CInvoice &x);
  bool delIssued(const CInvoice &x);
  bool delAccepted(const CInvoice &x);
  list<CInvoice> unmatched(const string &company,
                           const CSortOpt &sortBy) const;
  string tocannoncial(string) const;
  

private:
  // todo
  unordered_map<string, Ccompany> companies;
  size_t m_order;
};

CVATRegister::CVATRegister():m_order(0)
{
}

string CVATRegister::tocannoncial(string name) const
{
  for_each(name.begin(), name.end(), [](char &c)
           { c = toupper(static_cast<unsigned char>(c)); });

  vector<string> nameParts;
  string::iterator start = name.begin();
  for (string::iterator it = name.begin(); it != name.end(); it++)
  {
    if (isspace(*it))
    {
      if (start != it)
      {
        nameParts.push_back(string(start, it));
      }
      start = it + 1;
    }
  }
  if (start != name.end())
  {
    nameParts.push_back(string(start, name.end()));
  }

  string result;
  for (const string &part : nameParts)
  {
    result += part + " ";
  }

  return result;
}

bool CVATRegister::registerCompany(const string &name)
{
  Ccompany company(name);
  string cannonical_name = tocannoncial(name);
  if (companies.find(cannonical_name) == companies.end())
  {
    companies.insert({cannonical_name, company});
  }
  else
  {
    return false;
  }
  return true;
}

bool CVATRegister::addIssued(const CInvoice &x)
{ 
  CInvoice y = x;
  string cannonnical_seller = tocannoncial(y.seller());
  string cannonnical_buyer = tocannoncial(y.buyer());
  if(cannonnical_seller == cannonnical_buyer) return false;
  
  if(companies.find(cannonnical_seller) == companies.end())
  {
    return false;
  }
  if(companies.find(cannonnical_buyer) == companies.end())
  {
    return false;
  }

  y.changeseller(companies.find(cannonnical_seller)->second.getOfficialName());
  y.changebuyer(companies.find(cannonnical_buyer)->second.getOfficialName());

  if(companies.find(cannonnical_seller)->second.m_issue.find(y) != companies.find(cannonnical_seller)->second.m_issue.end())
  {
    return false;
  }

  if(companies.find(cannonnical_buyer)->second.m_issue.find(y) !=   companies.find(cannonnical_seller)->second.m_issue.end() )
  {
    return false;
  }

  m_order++;
  y.changeorder(m_order);
  companies[cannonnical_buyer].m_issue[y] = true;
  companies[cannonnical_seller].m_issue[y] = true;
  
  return true;
}


bool CVATRegister::addAccepted(const CInvoice &x)
{ 
  CInvoice y = x;
  string cannonnical_seller = tocannoncial(y.seller());
  string cannonnical_buyer = tocannoncial(y.buyer());
  if(cannonnical_seller == cannonnical_buyer) return false;
  
  if(companies.find(cannonnical_seller) == companies.end())
  {
    return false;
  }
  if(companies.find(cannonnical_buyer) == companies.end())
  {
    return false;
  }

  y.changeseller(companies.find(cannonnical_seller)->second.getOfficialName());
  y.changebuyer(companies.find(cannonnical_buyer)->second.getOfficialName());

  if(companies.find(cannonnical_seller)->second.m_accept.find(y) != companies.find(cannonnical_seller)->second.m_accept.end())
  {
    return false;
  }

  if(companies.find(cannonnical_buyer)->second.m_accept.find(y) !=   companies.find(cannonnical_seller)->second.m_accept.end() )
  {
    return false;
  }


  m_order++;
  y.changeorder(m_order);
  companies[cannonnical_buyer].m_accept[y] = true;
  companies[cannonnical_seller].m_accept[y] = true;
  
  return true;
}

bool CVATRegister::delIssued(const CInvoice &x)
{
  CInvoice y = x;
  string cannonnical_seller = tocannoncial(y.seller());
  string cannonnical_buyer = tocannoncial(y.buyer());
  if(cannonnical_seller == cannonnical_buyer) return false;
  
  if(companies.find(cannonnical_seller) == companies.end())
  {
    return false;
  }
  if(companies.find(cannonnical_buyer) == companies.end())
  {
    return false;
  }

  y.changeseller(companies.find(cannonnical_seller)->second.getOfficialName());
  y.changebuyer(companies.find(cannonnical_buyer)->second.getOfficialName());

  if(companies.find(cannonnical_seller)->second.m_issue.find(y) == companies.find(cannonnical_seller)->second.m_issue.end())
  {
    return false;
  }

  if(companies.find(cannonnical_buyer)->second.m_issue.find(y) ==   companies.find(cannonnical_buyer)->second.m_issue.end() )
  {
    return false;
  }


  companies[cannonnical_buyer].m_issue.erase(y);
  companies[cannonnical_seller].m_issue.erase(y);
  
  return true;
}




bool CVATRegister::delAccepted(const CInvoice &x)
{
  CInvoice y = x;
  string cannonnical_seller = tocannoncial(y.seller());
  string cannonnical_buyer = tocannoncial(y.buyer());
  if(cannonnical_seller == cannonnical_buyer) return false;
  
  if(companies.find(cannonnical_seller) == companies.end())
  {
    return false;
  }
  if(companies.find(cannonnical_buyer) == companies.end())
  {
    return false;
  }

  y.changeseller(companies.find(cannonnical_seller)->second.getOfficialName());
  y.changebuyer(companies.find(cannonnical_buyer)->second.getOfficialName());

  if(companies.find(cannonnical_seller)->second.m_accept.find(y) == companies.find(cannonnical_seller)->second.m_accept.end())
  {
    return false;
  }

  if(companies.find(cannonnical_buyer)->second.m_accept.find(y) ==   companies.find(cannonnical_buyer)->second.m_accept.end() )
  {
    return false;
  }


  companies[cannonnical_buyer].m_accept.erase(y);
  companies[cannonnical_seller].m_accept.erase(y);
  
  return true;
}

void printlist(const list<CInvoice> &src)
{
  cout << "LIST:" << endl;
  for (const auto &i : src)
    cout << i << endl;
  cout << "______________________________" << endl;
}

list<CInvoice> CVATRegister::unmatched(const string &company,const CSortOpt &sortBy) const
{
  list<CInvoice> result;
  auto iterator = companies.find(tocannoncial(company));
  if(iterator != companies.end())
  {
    // printlist(iterator->second.m_issues);
    // printlist(iterator->second.m_accept);
    // cout << "ISSUE" << endl;
    for (auto & invoice : iterator->second.m_issue)
    {
      // cout << invoice.first << endl;
      if(iterator->second.m_accept.find(invoice.first) == iterator->second.m_accept.end())
      {
        // cout << invoice.first << endl;
        result.push_back(invoice.first);
      }
    }
    // cout << "ACCEPT" << endl;
    for (auto & invoice : iterator->second.m_accept)
    {
      // cout << invoice.first<< endl;
      if(iterator->second.m_issue.find(invoice.first) == iterator->second.m_issue.end())
      {
        // cout << invoice.first << endl;
        result.push_back(invoice.first);
      }
    }
  }

  result.sort([sortBy](const CInvoice& a, const CInvoice& b) {
    return compare(a, b, sortBy);
  });
  // printlist(result);
  return result;
}


#ifndef __PROGTEST__
bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b)
{
  // todo
if (a.size() != b.size())
    return false;

  list<CInvoice>::const_iterator a1 = a.begin();
  list<CInvoice>::const_iterator b1 = b.begin();

  while (a1 != a.end() && b1 != b.end())
  { 
    if((*a1).equal(*b1))
    {
      a1++;
      b1++;
    }
    else
    {
      return false;
    }

  }
  return true;
}

int main(void)
{
   CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
