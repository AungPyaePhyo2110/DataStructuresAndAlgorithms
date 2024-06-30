#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class CStringSharedData
{
  private:
    int refCtr;
  public:
    size_t m_size;
    size_t m_capacity;
    char * m_data;
    int getCtr();
    void increaseCtr();
    void decreaseCtr();
    CStringSharedData();
    ~CStringSharedData();
};

int CStringSharedData::getCtr()
{
  return refCtr;
}
void CStringSharedData::increaseCtr()
{
  refCtr++;
}
void CStringSharedData::decreaseCtr()
{
  refCtr--;
}
CStringSharedData::CStringSharedData():refCtr(1),m_size(0),m_capacity(0),m_data(nullptr)
{
}
CStringSharedData::~CStringSharedData()
{
  if(refCtr==0)
  {
    delete[] m_data;
  }
}


class CString
{
private:
  CStringSharedData * m_ptr;
public:
  CString(const char *data = "");
  CString(const CString &);
  CString &operator=(const CString &);
  ~CString();
  bool operator==(const CString &) const;
  bool operator < (const CString &) const;
  friend ostream & operator << (ostream &, const CString);
  void deleteData();
  
};

void CString::deleteData()
{
  m_ptr->decreaseCtr();
  if(m_ptr->getCtr()==0)
    delete m_ptr;
}





CString::CString(const char *data)
{ 
  m_ptr = new CStringSharedData;
  m_ptr-> m_size = strlen(data);
  m_ptr->m_capacity = strlen(data)+ 1;
  m_ptr->m_data = new char[m_ptr->m_capacity];
  memcpy(m_ptr->m_data, data, m_ptr->m_size);
  m_ptr->m_data[m_ptr->m_size] = '\0';
}

CString::CString(const CString &src) : m_ptr(src.m_ptr)
{
  m_ptr->increaseCtr();
}

CString &CString::operator=(const CString &src)
{
  if (this == &src)
    return *this;

  deleteData();

  m_ptr = src.m_ptr;
  m_ptr->increaseCtr();

  return *this;
}

CString::~CString()
{
  deleteData();
}


bool CString::operator==(const CString &other) const
{
  return strcmp(m_ptr->m_data, other.m_ptr->m_data) == 0;
}

ostream & operator << (ostream & os , const CString  string)
{
  return os << string.m_ptr->m_data;
}


bool CString::operator < (const CString & other) const
{
  if(strcmp(m_ptr->m_data,other.m_ptr->m_data)<0)
    return true;
  return false;
}






class CMail
{
public:

  CMail();
  CMail(const char *from,const char *to,const char *body);
  bool operator==(const CMail &x) const;
  friend ostream &operator<<(ostream &os,const CMail &m);
  CMail(const CMail&);
  CMail & operator= (const CMail);
  CString getreceiver() const;
  CString getsender() const;
  

private:
  // todo
  CString sender, receiver, content;
};


CMail::CMail() : sender(""), receiver(""), content("")
{
}


CMail::CMail(const char *from, const char *to, const char *body)
{
  sender = CString(from);
  receiver = CString(to);
  content = CString(body);
}

CMail::CMail(const CMail& src)
{
  sender = src.sender;
  receiver = src.receiver;
  content = src.content;
}

CMail & CMail::operator= (const CMail src)
{
  if(this==&src)
    return *this;

  sender = src.sender;
  receiver = src.receiver;
  content = src.content;
  return *this;
}


bool CMail::operator==(const CMail &other) const
{
  return this->sender == other.sender && this->receiver == other.receiver && this->content == other.content;
}

// "From: john, To: alice, Body: deadline notice"
ostream &operator<<(ostream &os, const CMail &mail)
{
  return os << "From: " << mail.sender << ", To: " << mail.receiver << ", Body: " << mail.content;
}

CString CMail::getsender() const
{
  return sender;
}


CString CMail::getreceiver() const
{
  return receiver;
}


class CvectorSharedData
{
  private:
    int refCtr;
  public:
    size_t m_size;
    size_t m_capacity;
    CMail * m_data;
    int getCtr();
    void increaseCtr();
    void decreaseCtr();
    CvectorSharedData();
    ~CvectorSharedData();
};


int CvectorSharedData::getCtr()
{
  return refCtr;
}
void CvectorSharedData::increaseCtr()
{
  refCtr++;
}
void CvectorSharedData::decreaseCtr()
{
  refCtr--;
}
CvectorSharedData::CvectorSharedData():refCtr(1),m_size(0),m_capacity(0),m_data(nullptr)
{
}
CvectorSharedData::~CvectorSharedData()
{
  if(refCtr==0)
  {
    delete[] m_data;
  }
}


class Cvector
{
  public:
  CvectorSharedData * m_ptr;

  Cvector();
  Cvector(CvectorSharedData *);
  Cvector(const Cvector &);
  Cvector &operator=(const Cvector &);
  ~Cvector();
  void deleteData();
  void detach();
  void push_back(CMail);
  const CMail & operator[](size_t index) const;
  friend ostream & operator << (ostream  & , const Cvector &);
};

ostream & operator << (ostream  & os , const Cvector & vector)
{
  for(size_t i = 0 ; i < vector.m_ptr->m_size ; i++)
  {
    os << vector.m_ptr->m_data[i] << endl;
  }
  return os;
}

Cvector::Cvector():m_ptr(new CvectorSharedData)
{
}

Cvector::Cvector(CvectorSharedData * ptr):m_ptr(ptr)
{
}
void Cvector::deleteData()
{
  m_ptr->decreaseCtr();
  if(m_ptr->getCtr()==0)
  {
    delete m_ptr;
  }
}

Cvector::Cvector(const Cvector & src):m_ptr(src.m_ptr)
{
  m_ptr->increaseCtr();
}

Cvector & Cvector::operator=(const Cvector &src)
{
  if(this==&src)
    return *this;

  deleteData();

  m_ptr = src.m_ptr;
  m_ptr->increaseCtr();

  return *this;
}

Cvector::~Cvector()
{
  deleteData();
}


void Cvector::detach()
{
  if(m_ptr->getCtr()==1)
    return;
  
  deleteData();
  CvectorSharedData * new_ptr = new CvectorSharedData;
  new_ptr->m_capacity = m_ptr->m_capacity;
  new_ptr->m_size = m_ptr->m_size;
  new_ptr->m_data  = new CMail[m_ptr->m_capacity];
  copy(m_ptr->m_data,m_ptr->m_data+m_ptr->m_capacity,new_ptr->m_data);
  m_ptr = new_ptr;
}

// CString(const CString &);

void Cvector::push_back(CMail x)
{ 
  detach();

  size_t old_capacity = m_ptr->m_capacity;
  if(m_ptr->m_size >= m_ptr->m_capacity)
  {
    if(m_ptr->m_capacity == 0)
      m_ptr->m_capacity = 1;
    m_ptr->m_capacity *= 2;
    CMail * new_data = new CMail[m_ptr->m_capacity];
    copy(m_ptr->m_data,m_ptr->m_data+old_capacity,new_data);
    delete[] m_ptr->m_data;
    m_ptr->m_data = new_data;    
  }
  m_ptr->m_data[m_ptr->m_size] = x;
  m_ptr->m_size++;
}

const CMail &  Cvector::operator[](size_t index) const
{
  return m_ptr->m_data[index];
}




class CMailIterator
{
public:
  explicit operator bool(void) const;
  bool operator!(void) const;
  const CMail &operator*(void) const;
  CMailIterator &operator++(void);
  CMailIterator(const Cvector &);
  CMailIterator(const CMailIterator&);
  CMailIterator & operator=(const CMailIterator&);
  ~CMailIterator();

private:
  // todo
  Cvector m_data;
  size_t m_size;
  size_t m_index;
};

CMailIterator::operator bool(void) const
{
  return m_index < m_size;
}

bool CMailIterator::operator!(void) const
{
  return !(bool)*this;
}

const CMail & CMailIterator::operator*(void) const
{
  return m_data[m_index];
}

CMailIterator & CMailIterator::operator++(void)
{
  m_index++;
  return *this;
}

CMailIterator::CMailIterator(const Cvector & data):m_data(data),m_size(data.m_ptr->m_size),m_index(0)
{
  // cout << m_data << endl;
  // cout << "Size: " << m_size << endl;
}


CMailIterator::CMailIterator(const CMailIterator& src)
{
  m_size = src.m_size;
  m_index = src.m_index;
  m_data = src.m_data;
}
CMailIterator & CMailIterator::operator=(const CMailIterator& src)
{
  if(this==&src)
    return *this;
  m_size = src.m_size;
  m_index = src.m_index;
  m_data = src.m_data;
  return *this;
}
CMailIterator::~CMailIterator()
{
  m_size = 0;
  m_index = 0;
}


class CNode
{
  public:
  CNode * left;
  CNode * right;
  CString m_name;
  Cvector inbox;
  Cvector outbox;
  CNode();
  CNode(const CNode & );
  void freeNode();
  // void printNode();
};

CNode::CNode()
{
  left = nullptr;
  right = nullptr;
}

CNode::CNode(const CNode & src ):left(nullptr),right(nullptr),m_name(src.m_name),inbox(src.inbox),outbox(src.outbox) 
{
  if(src.left)
  {
    left = new CNode(*src.left);
  }
  if(src.right)
  {
    right = new CNode(*src.right);
  }
}


void CNode::freeNode()
{
  if(left)
  {
    left->freeNode();
    delete left;
  }

  if(right)
  {
    right->freeNode();
    delete right;
  }
}

// void CNode::printNode()
// { 
//   if(left)
//     left->printNode();
//   if(right)
//     right->printNode();
//   cout << "Name: " << m_name << endl;
//   cout << "Inbox: " << endl;
//   for(int i = 0 ; i < inbox_size ; i++)
//   {
//     cout << inbox[i] << endl;
//   }
//   cout << "Outbox:" << endl;
//   for(int i = 0 ; i < outbox_size ; i++)
//   {
//     cout << outbox[i] << endl;
//   }
//   cout << "___________________________________________" << endl;
// }


class BST
{ 
  public:
  CNode * root;
  BST();
  ~BST();
  void insert(CMail);
  // void print();
  BST & operator = (const BST & src);
  CNode * findinbox(const CString name);
  CNode * findoutbox(const CString name);
};

BST::BST():root(nullptr)
{
}

BST::~BST()
{
  if(root)
  {
    root->freeNode();
    delete root;
    root = nullptr;
  }
}

void BST::insert(CMail mail)
{
  CNode ** node = &root;
  bool found = false;
  while(*node)
  {
    if((*node)->m_name == mail.getsender())
    { 
      (*node)->outbox.push_back(mail);
      found = true;
      break;
    }
    else if ((*node)->m_name < mail.getsender())
      node = &(*node)->right;
    else
      node = &(*node)->left;
  }
  if(!found)
  {
    *node = new CNode;
    (*node)->m_name = mail.getsender();
    (*node)->outbox.push_back(mail);

  }

  node = &root;
  while(*node)
  {
    if((*node)->m_name == mail.getreceiver())
    { 
      (*node)->inbox.push_back(mail);
      return;
    }
    else if ((*node)->m_name < mail.getreceiver())
      node = &(*node)->right;
    else
      node = &(*node)->left;
  }

  *node = new CNode;
  (*node)->m_name = mail.getreceiver();
  (*node)->inbox.push_back(mail);


}

// void BST::print()
// {
//   if(root)
//     root->printNode();
// }

BST & BST::operator = (const BST & src)
{
  if(this==&src)
    return * this;

  if(root)
  {
    root->freeNode();
    delete root;
    root = nullptr;
  }

  if(src.root)
  {
    root = new CNode(*src.root);
  }
  
  return *this;
}

CNode * BST::findinbox(const CString name )
{ 
  CNode * node = root;
  while(node)
  {
    if(node->m_name == name)
    {
      return node;
    }
    else if(node->m_name < name)
      node = node->right;
    else
      node = node->left;
  }
  // return Cvector(nullptr);
  return node;
}
CNode * BST::findoutbox(const CString name)
{
  Cvector result;
  CNode * node = root;
  while(node)
  {
    if(node->m_name == name)
    { 
      return node; 

    }
    else if(node->m_name < name)
      node = node->right;
    else
      node = node->left;
  }
  // return Cvector(nullptr);
  return node;
}




class SharedData
{
private:
  int ref_Ctr;

public:
  BST tree;
  int getCtr() const;
  void increaseCtr();
  void decreaseCtr();
  SharedData();
  // ~SharedData();
};

int SharedData::getCtr() const
{
  return ref_Ctr;
}

void SharedData::increaseCtr()
{
  ref_Ctr++;
}
void SharedData::decreaseCtr()
{
  ref_Ctr--;
}

SharedData::SharedData() : ref_Ctr(1)
{
}

// SharedData::~SharedData()
// {
//   if (ref_Ctr == 0)
//     {
//       delete tree;
//     }
// }






class CMailServer
{
public:
  CMailServer(void);
  CMailServer(const CMailServer &src);
  CMailServer &operator=(const CMailServer &src);
  ~CMailServer(void);
  CMail& operator[](size_t);
  CMail operator[](size_t) const;
  void sendMail(const CMail &m);
  CMailIterator outbox(const char *email) const;
  CMailIterator inbox(const char *email) const;
  // friend ostream & operator << (ostream &, const CMailServer);
  // void print();
  void deleteData();

private:
  SharedData * m_ptr;
  void ensureData(size_t);
  void detach();
};

//printing mailserver
// ostream & operator << (ostream & os , const CMailServer server)
// {
//   return os << server.m_ptr->tree.print();
// }
// void CMailServer::print()
// {
//   m_ptr->tree.print();
// }


// void CMailServer::ensureData(size_t index)
// {
//   if(index < m_ptr->m_capacity)
//     return;
//   size_t old_capacity = m_ptr->m_capacity;
//   m_ptr->m_capacity = max(m_ptr->m_capacity * 2,index)+1;
//   CMail * new_data = new CMail[m_ptr->m_capacity];
//   copy(m_ptr->m_data,m_ptr->m_data+old_capacity,new_data);
//   delete[] m_ptr->m_data;
//   m_ptr->m_data = new_data;
// }

void CMailServer::deleteData()
{
  m_ptr->decreaseCtr();
  if(m_ptr->getCtr()==0)
    delete m_ptr;
}

void CMailServer::detach()
{
  if(m_ptr->getCtr()==1)
    return;
  
  deleteData();

  auto tmp = new SharedData;

  tmp->tree = m_ptr->tree;
  m_ptr = tmp;
}





//constructor
CMailServer::CMailServer() : m_ptr(new SharedData)
{
}
//copy constructor
CMailServer::CMailServer(const CMailServer &src):m_ptr(src.m_ptr)
{
  m_ptr->increaseCtr();
}
//operator=
CMailServer &CMailServer::operator=(const CMailServer &src)
{
  if (this == &src)
    return *this;

  deleteData();
  m_ptr = src.m_ptr;
  m_ptr->increaseCtr();
  
  return *this;
}
//destructor
CMailServer::~CMailServer()
{
  deleteData();
}


//sendMail
void CMailServer::sendMail(const CMail &m)
{ 
  detach();
  m_ptr->tree.insert(m);
}

CMailIterator CMailServer::outbox(const char *email) const
{ 
  CNode * node;
  // int size = 0 ;
  node = m_ptr->tree.findoutbox(CString(email));
  if(node == nullptr)
    return CMailIterator(Cvector());

  return CMailIterator(node->outbox);
}
CMailIterator CMailServer::inbox(const char *email) const
{
  
  CNode * node;
  // int size = 0;
  node = m_ptr->tree.findinbox(CString(email));
  // if(node)
  // {
    // cout << node->inbox << endl;
  // }
  if(node == nullptr)
    return CMailIterator(Cvector());

  return CMailIterator(node->inbox);
}




#ifndef __PROGTEST__
bool matchOutput(const CMail &m,
                 const char *str)
{
  ostringstream oss;
  oss << m;
  return oss.str() == str;
}

int main(void)
{
  char from[100], to[100], body[1024];

  assert(CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
  CMailServer s0;
  s0.sendMail(CMail("john", "peter", "some important mail"));
  strncpy(from, "john", sizeof(from));
  strncpy(to, "thomas", sizeof(to));
  strncpy(body, "another important mail", sizeof(body));
  s0.sendMail(CMail(from, to, body));
  strncpy(from, "john", sizeof(from));
  strncpy(to, "alice", sizeof(to));
  strncpy(body, "deadline notice", sizeof(body));
  s0.sendMail(CMail(from, to, body));
  s0.sendMail(CMail("alice", "john", "deadline confirmation"));
  s0.sendMail(CMail("peter", "alice", "PR bullshit"));
  // s0.print();
  CMailIterator i0 = s0.inbox("alice");
  assert(i0);
  assert(i0 && *i0 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
  assert(++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
  assert(!++i0);

  CMailIterator i1 = s0.inbox("john");
  assert(i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
  assert(matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
  assert(!++i1);

  // // cout << s0 << endl;
  CMailIterator i2 = s0.outbox("john");
  // // cout << *i2 << endl;
  assert(i2 && *i2 == CMail("john", "peter", "some important mail"));
  assert(matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
  assert(++i2 && *i2 == CMail("john", "thomas", "another important mail"));
  assert(matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
  assert(++i2 && *i2 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
  assert(!++i2);

  CMailIterator i3 = s0.outbox("thomas");
  assert(!i3);

  CMailIterator i4 = s0.outbox("steve");
  assert(!i4);

  CMailIterator i5 = s0.outbox("thomas");
  s0.sendMail(CMail("thomas", "boss", "daily report"));
  assert(!i5);

  CMailIterator i6 = s0.outbox("thomas");
  assert(i6 && *i6 == CMail("thomas", "boss", "daily report"));
  assert(matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
  assert(!++i6);

  CMailIterator i7 = s0.inbox("alice");
  s0.sendMail(CMail("thomas", "alice", "meeting details"));
  assert(i7 && *i7 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
  assert(++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
  assert(!++i7);

  CMailIterator i8 = s0.inbox("alice");
  assert(i8 && *i8 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
  assert(++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
  assert(!++i8);

  CMailServer s1(s0);


  s0.sendMail(CMail("joe", "alice", "delivery details"));
  s1.sendMail(CMail("sam", "alice", "order confirmation"));

  // s0.print();
  // cout << "s11111111" << endl;
  // s1.print();

  CMailIterator i9 = s0.inbox("alice");
  assert(i9 && *i9 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
  assert(++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
  assert(++i9 && *i9 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
  assert(!++i9);

  CMailIterator i10 = s1.inbox("alice");
  assert(i10 && *i10 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
  assert(++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
  assert(++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
  assert(matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
  assert(!++i10);

  CMailServer s2;
  s2.sendMail(CMail("alice", "alice", "mailbox test"));
  CMailIterator i11 = s2.inbox("alice");
  assert(i11 && *i11 == CMail("alice", "alice", "mailbox test"));
  assert(matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
  assert(!++i11);

  s2 = s0;
  s0.sendMail(CMail("steve", "alice", "newsletter"));
  s2.sendMail(CMail("paul", "alice", "invalid invoice"));
  CMailIterator i12 = s0.inbox("alice");
  assert(i12 && *i12 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
  assert(++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
  assert(++i12 && *i12 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
  assert(++i12 && *i12 == CMail("steve", "alice", "newsletter"));
  assert(matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
  assert(!++i12);

  CMailIterator i13 = s2.inbox("alice");
  assert(i13 && *i13 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
  assert(++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
  assert(++i13 && *i13 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
  assert(++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
  assert(matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
  assert(!++i13);

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */



  // cout << "S0" << endl;
  // cout << s0;
  // cout << "_________________________________" << endl;

  // cout << "S1" << endl;
  // cout << s1;
  // cout << "_________________________________" << endl;


