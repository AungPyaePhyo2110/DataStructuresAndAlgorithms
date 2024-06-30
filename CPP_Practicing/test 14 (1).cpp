#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

//--------------------------------------------------------------------------------------------------------------------------------------------
class Interface
{
public:
  virtual ~Interface() = default;
  virtual void print(ostream &os ,string ) const = 0;
  virtual Interface *clone() const = 0;
  virtual void changePos(CRect & ) = 0;
  virtual bool checkid(int ) = 0;
  virtual Interface * search(int) = 0;
};

ostream & operator << (ostream & os , const Interface & src)
{
  src.print(os,"");
  return os;
}


//--------------------------------------------------------------------------------------------------------------------------------------------

class CWindow
{
protected:
  int m_id;
  string m_titile;
  CRect m_rect;
  vector<Interface *> controls;
public:
  CWindow(int id,const string &title,const CRect &absPos):m_id(id),m_titile(title),m_rect(absPos){}

  CWindow(const CWindow & src):m_id(src.m_id),m_titile(src.m_titile),m_rect(src.m_rect)
  {
    for(size_t i = 0 ; i < src.controls.size() ; ++i)
    {
      controls.push_back(src.controls[i]->clone());
    }
  }

  CWindow & operator = (const CWindow & src)
  {
    if(this == &src)
      return *this;
    m_id = src.m_id;
    m_titile = src.m_titile;
    m_rect = src.m_rect;
    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      delete controls[i];
    }
    controls.clear();

    for(size_t i = 0 ; i < src.controls.size() ; ++i)
    {
      controls.push_back(src.controls[i]->clone());
    }
    return *this;
  }

  ~CWindow()
  {
    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      delete controls[i];
    }
  }

  // add
  CWindow & add (const Interface & control)
  {
    Interface * new_control = control.clone();
    new_control->changePos(m_rect);
    controls.push_back(new_control);
    return *this;
  }
  // search
  Interface * search(const int given_id)
  { 
    for(size_t i = 0 ; i < controls.size() ; ++i)
    { 
      if(controls[i]->checkid(given_id))
      { 
        return controls[i]->search(given_id);
      }
    }
    return nullptr;
  }
  // setPosition
  void setPosition(const CRect & new_rect)
  {
    m_rect = new_rect;
    for(size_t i = 0 ; i < controls.size() ; i++)
    {
      controls[i]->changePos(m_rect);
    }
  }
  friend ostream & operator << (ostream & os , const CWindow & src)
  {
    os << "[" << src.m_id << "]" << " Window "<<"\"" << src.m_titile << "\" " << src.m_rect << "\n";
    for(size_t i = 0 ; i < src.controls.size() ; ++i)
    { 
      os << "+- ";
      if(i == src.controls.size()-1)
      {
        src.controls[i]->print(os,"   ");
      }
      else
      {
        src.controls[i]->print(os,"|  ");
      }
    }
    return os;
  }
};

//--------------------------------------------------------------------------------------------------------------------------------------------


class CPanel  : public Interface
{   
  int m_id;
  CRect m_relPos;
  CRect m_absPos;
  vector< Interface *> controls;
  int m_indentation;
  public:
  CPanel ( int id,const CRect & relPos ):m_id(id),m_relPos(relPos),m_absPos(CRect(relPos.m_X,relPos.m_Y,relPos.m_W,relPos.m_H)),m_indentation(0){}

  CPanel(const CPanel & src):m_id(src.m_id),m_relPos(src.m_relPos),m_absPos(src.m_absPos),m_indentation(src.m_indentation)
  {
    for(size_t i = 0 ; i < src.controls.size(); i++)
    {
      controls.push_back(src.controls[i]->clone());
    }
  }

  ~CPanel()
  {
    for(size_t i = 0 ; i < controls.size() ; i++)
    {
      delete controls[i];
    }
  }

  CPanel & operator = (const CPanel & src)
  {
    if(this==&src)
      return *this;
    
    m_id = src.m_id;
    m_relPos = src.m_relPos;
    m_absPos = src.m_absPos;
    m_indentation = src.m_indentation;


    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      delete controls[i];
    }
    controls.clear();

    for(size_t i = 0 ; i < src.controls.size() ; ++i)
    {
      controls.push_back(src.controls[i]->clone());
    }
    
    return *this;
  }


  virtual void print(ostream & os , string prefix) const override
  { 
    os << "[" << m_id << "]" << " Panel " << m_absPos << "\n";
    for(size_t i = 0 ; i < controls.size() ; ++i)
    { 
      os << prefix;
      os << "+- ";
      if(i == controls.size()-1)
      {
        string new_prefix = prefix + "   ";
        controls[i]->print(os,new_prefix);
      }
      else
      {
        string new_prefix = prefix + "|  ";
        controls[i]->print(os,new_prefix);
      }
    }

  }

  virtual Interface * clone() const override
  {
    return new CPanel(*this);
  }

  virtual void changePos(CRect & parent ) override
  {
    m_absPos.m_X = parent.m_X + ( parent.m_W * m_relPos.m_X);
    m_absPos.m_Y = parent.m_Y + (parent.m_H * m_relPos.m_Y);
    m_absPos.m_W = parent.m_W * m_relPos.m_W;
    m_absPos.m_H = parent.m_H * m_relPos.m_H;

    //change the absolute postion of the controls inside the panel as well
    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      controls[i]->changePos(m_absPos);
    }
  }

  virtual bool checkid(int given_id) override
  { 
    if(m_id == given_id)
    {
      return true;
    }
    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      if(controls[i]->checkid(given_id))
      {
        return true;
      }
    }
    return false;
  }

  virtual Interface * search(int given_id) override
  {
    if(m_id == given_id)
    {
      return this;
    }

    for(size_t i = 0 ; i < controls.size() ; ++i)
    {
      if(controls[i]->checkid(given_id))
      {
        return controls[i];
      }
    }
    return nullptr;
  }

    // add
  CPanel & add(const Interface & control)
  {
    Interface * new_control = control.clone();
    new_control->changePos(m_absPos);
    controls.push_back(new_control);
    return *this;
  }
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class CButton : public Interface
{
protected:
  int m_id;
  CRect m_relPos;
  CRect m_absPos;
  string m_name;
  int m_indentation;
public:
  CButton(int id,const CRect &relPos,const string &name):m_id(id),m_relPos(relPos),m_absPos(relPos),m_name(name),m_indentation(0){}

  CButton(const CButton & src):m_id(src.m_id),m_relPos(src.m_relPos),m_absPos(src.m_absPos),m_name(src.m_name),m_indentation(src.m_indentation){}

  CButton & operator = (const CButton & src)
  {
    if(this == & src)
      return *this;
    m_id = src.m_id;
    m_relPos = src.m_relPos;
    m_absPos = src.m_absPos;
    m_name = src.m_name;
    m_indentation = src.m_indentation;
    return *this;
  }

  virtual void print(ostream & os ,string prefix) const override
  { 
    string new_string = prefix;
    new_string = "";
    os << new_string;
    os << "[" << m_id << "]" << " Button " << "\"" << m_name << "\" " << m_absPos << "\n";
  }

  virtual Interface * clone() const override
  {
    return new CButton(*this);
  }

  virtual void changePos(CRect & parent ) override
  {
    m_absPos.m_X = parent.m_X + ( parent.m_W * m_relPos.m_X);
    m_absPos.m_Y = parent.m_Y + (parent.m_H * m_relPos.m_Y);
    m_absPos.m_W = parent.m_W * m_relPos.m_W;
    m_absPos.m_H = parent.m_H * m_relPos.m_H;
  }

  virtual bool checkid(int given_id) override
  {
    return m_id == given_id;
  }

  virtual Interface * search(int given_id) override
  {
    if(m_id == given_id)
    {
      return this;
    }
    return nullptr;
  }

};

//--------------------------------------------------------------------------------------------------------------------------------------------



class CInput : public Interface
{
protected:
  int m_id;
  CRect m_relPos;
  CRect m_absPos;
  string m_value;
  int m_indentation;
public:
  CInput(int id,const CRect &relPos,const string &value):m_id(id),m_relPos(relPos),m_absPos(relPos),m_value(value),m_indentation(0){}

  CInput(const CInput & src):m_id(src.m_id),m_relPos(src.m_relPos),m_absPos(src.m_absPos),m_value(src.m_value),m_indentation(src.m_indentation){}

  CInput & operator = ( const CInput & src)
  {
    if(this == & src)
      return *this;

    m_id = src.m_id;
    m_relPos = src.m_relPos;
    m_absPos = src.m_absPos;
    m_value = src.m_value;
    m_indentation = src.m_indentation;
    return *this;
  }

  virtual void print(ostream & os ,string prefix) const override
  { 
    string new_string = prefix;
    new_string = "";
    os << new_string;
    os <<"[" << m_id << "]" << " Input " << "\"" << m_value << "\" " << m_absPos << "\n";
  }

  virtual Interface * clone ( ) const override
  {
    return new CInput(*this);
  }

    virtual void changePos(CRect & parent ) override
  {
    m_absPos.m_X = parent.m_X + ( parent.m_W * m_relPos.m_X);
    m_absPos.m_Y = parent.m_Y + (parent.m_H * m_relPos.m_Y);
    m_absPos.m_W = parent.m_W * m_relPos.m_W;
    m_absPos.m_H = parent.m_H * m_relPos.m_H;
  }


    virtual bool checkid(int given_id) override
  {
    return m_id == given_id;
  }



    virtual Interface * search(int given_id) override
  {
    if(m_id == given_id)
    {
      return this;
    }
    return nullptr;
  }


  // setValue
  void setValue(const string & new_value){ m_value = new_value;}
  // getValue
  string getValue()const{return m_value;}
};


//--------------------------------------------------------------------------------------------------------------------------------------------


class CLabel : public Interface
{
protected:
int m_id;
CRect m_relPos;
CRect m_absPos;
string m_label;
int m_indentation;
public:
  CLabel(int id,const CRect &relPos,const string &label):m_id(id),m_relPos(relPos),m_absPos(relPos),m_label(label),m_indentation(0){}

  CLabel(const CLabel & src):m_id(src.m_id),m_relPos(src.m_relPos),m_absPos(src.m_absPos),m_label(src.m_label),m_indentation(src.m_indentation){}

  CLabel & operator = (const CLabel & src)
  {
    if(this==&src)
      return *this;
    m_id = src.m_id;
    m_relPos = src.m_relPos;
    m_absPos = src.m_absPos;
    m_label = src.m_label;
    m_indentation = src.m_indentation;
    return *this;
  }

  virtual void print(ostream & os , string  prefix) const override
  {
    string new_string = prefix;
    new_string = "";
    os << new_string;
    os << "[" << m_id << "]" << " Label " << "\"" << m_label << "\" " << m_absPos << "\n";
  }

  virtual Interface * clone() const override
  {
    return new CLabel(*this);
  }



    virtual void changePos(CRect & parent ) override
  {
    m_absPos.m_X = parent.m_X + ( parent.m_W * m_relPos.m_X);
    m_absPos.m_Y = parent.m_Y + (parent.m_H * m_relPos.m_Y);
    m_absPos.m_W = parent.m_W * m_relPos.m_W;
    m_absPos.m_H = parent.m_H * m_relPos.m_H;
  }

    virtual bool checkid(int given_id) override
  {
    return m_id == given_id;
  }



    virtual Interface * search(int given_id) override
  {
    if(m_id == given_id)
    {
      return this;
    }
    return nullptr;
  }
};

//--------------------------------------------------------------------------------------------------------------------------------------------


class CComboBox : public Interface
{
protected:
int m_id;
CRect m_relPos;
CRect m_absPos;
vector<string> boxes;
size_t selected_index;
int m_indentation;
public:
  CComboBox(int id,const CRect &relPos):m_id(id),m_relPos(relPos),m_absPos(relPos),m_indentation(0){
    selected_index = 0;
  }

  CComboBox(const CComboBox & src):m_id(src.m_id),m_relPos(src.m_relPos),m_absPos(src.m_absPos),m_indentation(src.m_indentation)
  {
    for(size_t i = 0 ; i < src.boxes.size() ; ++i)
    {
      boxes.push_back(src.boxes[i]);
    }
    selected_index = src.selected_index;
  }

  CComboBox & operator = (const CComboBox & src)
  {
    if(this == &src)
      return *this;


    m_id = src.m_id;
    m_relPos = src.m_relPos;
    m_absPos = src.m_absPos;
    selected_index = src.selected_index;
    boxes.clear();
    for(size_t i = 0 ; i < src.boxes.size() ; ++i)
    {
      boxes.push_back(src.boxes[i]);
    }
    return *this;
  }

  // add
  CComboBox & add(const string & box)
  {
    boxes.push_back(box);
    return *this;
  }
  // setSelected
  void setSelected(size_t index)
  { 
    selected_index = index;
  }
  // getSelected
  size_t getSelected() const
  {
    return selected_index;
  }



  virtual void print(ostream & os , string prefix) const override
  {  
    os << "[" << m_id << "]" << " ComboBox " << m_absPos << "\n";
    //before the selected_index
    for(size_t i = 0 ; i < selected_index ; ++i)
    { 
      os << prefix;
      os << "+- " << boxes[i] << "\n";
    }

    //selected_index
    os << prefix;
    os << "+->" << boxes[selected_index] << "<\n";

    //after the selected_index
    for(size_t i = selected_index+1 ; i < boxes.size() ; ++i)
    { 
      os << prefix;
      os << "+- " << boxes[i] << endl;
    }
  }

  virtual Interface * clone() const override
  {
    return new CComboBox(*this);
  }

    virtual void changePos(CRect & parent ) override
  {
    m_absPos.m_X = parent.m_X + ( parent.m_W * m_relPos.m_X);
    m_absPos.m_Y = parent.m_Y + (parent.m_H * m_relPos.m_Y);
    m_absPos.m_W = parent.m_W * m_relPos.m_W;
    m_absPos.m_H = parent.m_H * m_relPos.m_H;
  }

    virtual bool checkid(int given_id) override
  {
    return m_id == given_id;
  }



    virtual Interface * search(int given_id) override
  {
    if(m_id == given_id)
    {
      return this;
    }
    return nullptr;
  }
};

//--------------------------------------------------------------------------------------------------------------------------------------------

// output operators

#ifndef __PROGTEST__
template <typename T_>
string toString ( const T_ & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );
//  cout << toString(a) << endl;
 assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (118,254.8,384,33.6)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
  p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  // cout << toString(b);
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (118,322,384,33.6)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  assert ( toString ( p ) ==
    "[12] Panel (70,154,480,336)\n"
    "+- [20] ComboBox (118,254.8,384,33.6)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (118,322,384,33.6)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         +->PA2<\n"
    "         +- OSY\n"
    "         +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|  |  +->PA2<\n"
    "|  |  +- OSY\n"
    "|  |  +- Both\n"
    "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "|     |  +- Karate\n"
    "|     |  +- Judo\n"
    "|     |  +- Box\n"
    "|     |  +->Progtest<\n"
    "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "|        +->PA2<\n"
    "|        +- OSY\n"
    "|        +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "            +->PA2<\n"
    "            +- OSY\n"
    "            +- Both\n" );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  |  +->PA2<\n"
    "   |  |  +- OSY\n"
    "   |  |  +- Both\n"
    "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |     |  +- Karate\n"
    "   |     |  +- Judo\n"
    "   |     |  +- Box\n"
    "   |     |  +->Progtest<\n"
    "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |        +->PA2<\n"
    "   |        +- OSY\n"
    "   |        +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  |  +- Karate\n"
    "      |  |  +- Judo\n"
    "      |  |  +- Box\n"
    "      |  |  +->Progtest<\n"
    "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |     +->PA2<\n"
    "      |     +- OSY\n"
    "      |     +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         |  +->PA2<\n"
    "         |  +- OSY\n"
    "         |  +- Both\n"
    "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "            |  +- Karate\n"
    "            |  +- Judo\n"
    "            |  +- Box\n"
    "            |  +->Progtest<\n"
    "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "               +->PA2<\n"
    "               +- OSY\n"
    "               +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
