#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// Employee
class CEmployee
{

public:
  string m_name;
  string m_surname;
  string m_email;
  unsigned int m_salary;

  // constructor
  CEmployee(const string &name, const string &surname, const string &email, unsigned int salary)
      : m_name(name), m_surname(surname), m_email(email), m_salary(salary)
  {
  }

  // printing function
  friend ostream &operator<<(ostream &os, const CEmployee &employee)
  {
    return os << "Name: " << employee.m_name << " ,Surname: " << employee.m_surname << " ,Email: " << employee.m_email << " ,Salary: " << employee.m_salary << endl;
  }
};

// my personal agenda class will have three vectors of CEmployee pointers
//  one vector will be sorted by name(surname,name)
//  one vector will be sorted by email
//  one vector will be sorted by salaries(if the salary is the same it will be sorted by name)
class CPersonalAgenda
{
public:
  // constructor
  CPersonalAgenda(void)
  {
  }
  // destructor
  // since I have stored the pointers... I need to delete the objects only from one vector
  ~CPersonalAgenda(void)
  {
    for (size_t i = 0; i < db_sortedby_email.size(); i++)
    {
      delete db_sortedby_email[i];
    }
  }

  // have to add into three vectors
  // find the poistion by lower_bound and respective compare function.. and insert at the position
  bool add(const string &name,
           const string &surname,
           const string &email,
           unsigned int salary)
  {
    CEmployee *new_employee = new CEmployee(name, surname, email, salary);
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    auto index_salary = lower_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), new_employee, comparebySalaryname);
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      delete new_employee;
      return false;
    }
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      delete new_employee;
      return false;
    }
    db_sortedby_name.insert(index_name, new_employee);
    db_sortedby_email.insert(index_email, new_employee);
    db_sortedby_salary.insert(index_salary, new_employee);
    return true;
  }

  // since I am storing pointers... I have to delete the object just from one vector ... but have to remove the pointers from all three vectors
  bool del(const string &name,
           const string &surname)
  {
    CEmployee *new_employee = new CEmployee(name, surname, "", 0);
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), *index_name, comparebyEmail);
      auto index_salary = lower_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), *index_name, comparebySalaryname);
      delete *index_name;
      db_sortedby_name.erase(index_name);
      db_sortedby_email.erase(index_email);
      db_sortedby_salary.erase(index_salary);
      delete new_employee;
      return true;
    }
    else
    {
      delete new_employee;
      return false;
    }
  }

  bool del(const string &email)
  {
    CEmployee *new_employee = new CEmployee("", "", email, 0);
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      delete new_employee;
      return del((*index_email)->m_name, (*index_email)->m_surname);
    }
    else
    {
      delete new_employee;
      return false;
    }
  }

  // since I stored the pointers I dont need to change in all of my vectors....changing name in one vector will also do the work for other vectors
  bool changeName(const string &email,
                  const string &newName,
                  const string &newSurname)
  {
    CEmployee *new_employee = new CEmployee(newName, newSurname, email, 0);

    // checking if the new name has already existed
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == newName && (*index_name)->m_surname == newSurname)
    {
      delete new_employee;
      return false;
    }
    // search the given email
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    // if it exists..change the name
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      // first delete the old name from the vector sorted by name
      auto old_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), *index_email, comparebyName);
      db_sortedby_name.erase(old_name);
      // change the name in the vector sorted by email (this will also change the name in the vector sorted by salary)
      (*index_email)->m_name = newName;
      (*index_email)->m_surname = newSurname;
      // insert that pointer into the vector sorted by name again
      CEmployee *new_name = *index_email;
      auto new_index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_name, comparebyName);
      db_sortedby_name.insert(new_index_name, new_name);
      delete new_employee;
      return true;
    }
    // if the email doesn't exist... return false
    delete new_employee;
    return false;
  }

  bool changeEmail(const string &name,
                   const string &surname,
                   const string &newEmail)
  {
    // checking if the new email has already existed
    CEmployee *new_employee = new CEmployee(name, surname, newEmail, 0);
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == newEmail)
    {
      delete new_employee;
      return false;
    }

    // search the given name
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);

    // if it exist...change the email
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      // remove the old mail from the vector sorted by email
      auto old_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), *index_name, comparebyEmail);
      db_sortedby_email.erase(old_email);
      // change the email in the vector sorted by name (this will also change the email in the vector sorted by salary)
      (*index_name)->m_email = newEmail;
      // insert that pointer into the vector sorted by email again
      CEmployee *new_email = *index_name;
      auto new_index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), *index_name, comparebyEmail);
      db_sortedby_email.insert(new_index_email, new_email);
      delete new_employee;
      return true;
    }
    // if the name does not exist, return false
    delete new_employee;
    return false;
  }

  bool setSalary(const string &name,
                 const string &surname,
                 unsigned int salary)
  {
    CEmployee *new_employee = new CEmployee(name, surname, "", salary);

    // check if the name exists
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    // if the name exist...change the salary
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      // first remove the pointer from the vector sorted by salary
      auto old_salary = lower_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), *index_name, comparebySalaryname);
      db_sortedby_salary.erase(old_salary);
      // change the salary in the vector sorted by name (this will also change the salary in the vecotr sored by email);
      (*index_name)->m_salary = salary;
      // insert the pointer into the vector sorted by salary again
      CEmployee *new_salary = *index_name;
      auto index_salary = lower_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), *index_name, comparebySalaryname);
      db_sortedby_salary.insert(index_salary, new_salary);
      delete new_employee;
      return true;
    }
    // if the name does not exist...return false
    delete new_employee;
    return false;
  }
  bool setSalary(const string &email,
                 unsigned int salary)
  {
    // check if the email exists
    CEmployee *new_employee = new CEmployee("", "", email, salary);
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    // if the email exists...change the salary
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      delete new_employee;
      // change the salary by name of the found email
      return setSalary((*index_email)->m_name, (*index_email)->m_surname, salary);
    }
    // if the email does not exist...return false
    delete new_employee;
    return false;
  }
  unsigned int getSalary(const string &name,
                         const string &surname) const
  {

    CEmployee *new_employee = new CEmployee(name, surname, "", 0);
    // search the name in the vector sorted by name
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    // if it exist...return salary
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      delete new_employee;
      return (*index_name)->m_salary;
    }
    // if it does not exist...return 0
    delete new_employee;
    return 0;
  }
  unsigned int getSalary(const string &email) const
  {
    CEmployee *new_employee = new CEmployee("", "", email, 0);
    // search the email in the vector sorted by email
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    // if it exist...return salary
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      delete new_employee;
      return (*index_email)->m_salary;
    }
    // if it does not exist...return 0
    delete new_employee;
    return 0;
  }
  bool getRank(const string &name,
               const string &surname,
               int &rankMin,
               int &rankMax) const
  {

    CEmployee *new_employee = new CEmployee(name, surname, "", 0);
    // search the name in the vector sorted by name
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    // if it exists find the lower_bound and upper bound of the salary of the name in the vector sorted by name
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      auto lower = lower_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), *index_name, comparebySalary);
      auto upper = upper_bound(db_sortedby_salary.begin(), db_sortedby_salary.end(), *index_name, comparebySalary);
      rankMin = distance(db_sortedby_salary.begin(), lower);
      rankMax = distance(db_sortedby_salary.begin(), upper - 1);
      // cout << rankMin << endl;
      // cout << rankMax << endl;
      delete new_employee;
      return true;
    }
    // if the name does not exist...return false
    delete new_employee;
    return false;
  }
  bool getRank(const string &email,
               int &rankMin,
               int &rankMax) const
  {
    CEmployee *new_employee = new CEmployee("", "", email, 0);
    // search the email in the vector sorted by email
    auto index_email = lower_bound(db_sortedby_email.begin(), db_sortedby_email.end(), new_employee, comparebyEmail);
    // if it exits reuse the previous function by passing the name of the email found;
    if (index_email != db_sortedby_email.end() && (*index_email)->m_email == email)
    {
      delete new_employee;
      return getRank((*index_email)->m_name, (*index_email)->m_surname, rankMin, rankMax);
    }
    // if the email does not exist...return false
    delete new_employee;
    return false;
  }
  bool getFirst(string &outName,
                string &outSurname) const
  {
    // if there is no employee...return false
    if (db_sortedby_name.size() == 0)
    {
      return false;
    }
    // output the first name from the vector sorted by name
    outName = db_sortedby_name[0]->m_name;
    outSurname = db_sortedby_name[0]->m_surname;
    return true;
  }

  bool getNext(const string &name,
               const string &surname,
               string &outName,
               string &outSurname) const
  {
    // check if the name is the last in the vector sorted by name
    int last_index = db_sortedby_name.size() - 1;
    if (db_sortedby_name[last_index]->m_name == name && db_sortedby_name[last_index]->m_surname == surname)
    {
      // if the name is the last.... return false
      return false;
    }
    CEmployee *new_employee = new CEmployee(name, surname, "", 0);
    // search the name in the vector sorted by name
    auto index_name = lower_bound(db_sortedby_name.begin(), db_sortedby_name.end(), new_employee, comparebyName);
    // if it is found...return the next name
    if (index_name != db_sortedby_name.end() && (*index_name)->m_name == name && (*index_name)->m_surname == surname)
    {
      outName = (*(index_name + 1))->m_name;
      outSurname = (*(index_name + 1))->m_surname;
      delete new_employee;
      return true;
    }
    // if the name is not found...retrun false;
    delete new_employee;
    return false;
  }

  // printing all thre vectors to debug
  void print()
  {
    printdatabasebyemail();
    printdatabasebyname();
    printdatabasebysalary();
  }

private:
  // todo
  vector<CEmployee *> db_sortedby_name;
  vector<CEmployee *> db_sortedby_email;
  vector<CEmployee *> db_sortedby_salary;

  // comparison function
  static bool comparebyName(const CEmployee *a, const CEmployee *b)
  {
    if (a->m_surname != b->m_surname)
      return a->m_surname < b->m_surname;
    return a->m_name < b->m_name;
  }
  static bool comparebyEmail(const CEmployee *a, const CEmployee *b)
  {
    return a->m_email < b->m_email;
  }
  static bool comparebySalary(const CEmployee *a, const CEmployee *b)
  {
    return a->m_salary < b->m_salary;
  }
  static bool comparebySalaryname(const CEmployee *a, const CEmployee *b)
  {
    if (a->m_salary == b->m_salary)
    {
      return comparebyName(a, b);
    }
    return a->m_salary < b->m_salary;
  }

  // printing each vector
  void printdatabasebyemail()
  {
    cout << "EMAIL" << endl;
    for (CEmployee *employee : db_sortedby_email)
    {
      cout << *employee << endl;
    }
    cout << "_____________________________________________________" << endl;
  }
  void printdatabasebyname()
  {
    cout << "NAME" << endl;
    for (CEmployee *employee : db_sortedby_name)
    {
      cout << *employee << endl;
    }
    cout << "_____________________________________________________" << endl;
  }
  void printdatabasebysalary()
  {
    cout << "SALARY" << endl;
    for (CEmployee *employee : db_sortedby_salary)
    {
      cout << *employee << endl;
    }
    cout << "_____________________________________________________" << endl;
  }
};

#ifndef __PROGTEST__
int main(void)
{

  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert(b1.add("John", "Smith", "john", 30000));
  assert(b1.add("John", "Miller", "johnm", 35000));
  assert(b1.add("Peter", "Smith", "peter", 23000));
  assert(!b1.add("abc", "cde", "peter", 3000));
  assert(!b1.add("John", "Smith", "smithhhh", 30023));

  assert(!b1.del("Johnyyyy", "Miller"));
  assert(!b1.del("peterrrrr"));

  assert(!b1.changeEmail("MR", "MR", "Peter"));
  assert(!b1.changeEmail("John", "Smith", "peter"));
  assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Miller");
  assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(b1.getNext("John", "Smith", outName, outSurname) && outName == "Peter" && outSurname == "Smith");
  assert(!b1.getNext("Peter", "Smith", outName, outSurname));

  assert(b1.setSalary("john", 32000));
  assert(b1.getSalary("john") == 32000);
  assert(b1.getSalary("John", "Smith") == 32000);
  assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 1);
  assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 1);
  assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getRank("johnm", lo, hi) && lo == 2 && hi == 2);

  assert(b1.setSalary("John", "Smith", 35000));
  assert(b1.getSalary("John", "Smith") == 35000);
  assert(b1.getSalary("john") == 35000);
  assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 2);
  assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 2);
  assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getRank("johnm", lo, hi) && lo == 1 && hi == 2);

  assert(b1.changeName("peter", "James", "Bond"));

  assert(b1.getSalary("peter") == 23000);
  assert(b1.getSalary("James", "Bond") == 23000);
  assert(b1.getSalary("Peter", "Smith") == 0);
  assert(b1.getFirst(outName, outSurname) && outName == "James" && outSurname == "Bond");
  assert(b1.getNext("James", "Bond", outName, outSurname) && outName == "John" && outSurname == "Miller");
  assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(!b1.getNext("John", "Smith", outName, outSurname));

  assert(b1.changeEmail("James", "Bond", "james"));
  assert(b1.getSalary("James", "Bond") == 23000);
  assert(b1.getSalary("james") == 23000);
  assert(b1.getSalary("peter") == 0);
  // b1.print();

  assert(b1.del("james"));

  // b1.print();
  assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 1);

  assert(b1.del("John", "Miller"));
  assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(!b1.getNext("John", "Smith", outName, outSurname));
  assert(b1.del("john"));
  assert(!b1.getFirst(outName, outSurname));
  assert(b1.add("John", "Smith", "john", 31000));
  assert(b1.add("john", "Smith", "joHn", 31000));
  assert(b1.add("John", "smith", "jOhn", 31000));
  // //      b1.printdatabasebyemail();
  // //   b1.printdatabasebyname();
  // // b1.printdatabasebysalary();

  CPersonalAgenda b2;
  assert(!b2.getFirst(outName, outSurname));
  assert(b2.add("James", "Bond", "james", 70000));
  assert(b2.add("James", "Smith", "james2", 30000));
  assert(b2.add("Peter", "Smith", "peter", 40000));
  assert(!b2.add("James", "Bond", "james3", 60000));
  assert(!b2.add("Peter", "Bond", "peter", 50000));
  assert(!b2.changeName("joe", "Joe", "Black"));
  assert(!b2.changeEmail("Joe", "Black", "joe"));
  assert(!b2.setSalary("Joe", "Black", 90000));
  assert(!b2.setSalary("joe", 90000));
  assert(b2.getSalary("Joe", "Black") == 0);
  assert(b2.getSalary("joe") == 0);
  assert(!b2.getRank("Joe", "Black", lo, hi));
  assert(!b2.getRank("joe", lo, hi));
  assert(!b2.changeName("joe", "Joe", "Black"));
  assert(!b2.changeEmail("Joe", "Black", "joe"));
  assert(!b2.del("Joe", "Black"));
  assert(!b2.del("joe"));
  assert(!b2.changeName("james2", "James", "Bond"));
  assert(!b2.changeEmail("Peter", "Smith", "james"));
  assert(!b2.changeName("peter", "Peter", "Smith"));
  assert(!b2.changeEmail("Peter", "Smith", "peter"));
  assert(b2.del("Peter", "Smith"));
  assert(!b2.changeEmail("Peter", "Smith", "peter2"));
  assert(!b2.setSalary("Peter", "Smith", 35000));
  assert(b2.getSalary("Peter", "Smith") == 0);
  assert(!b2.getRank("Peter", "Smith", lo, hi));
  assert(!b2.changeName("peter", "Peter", "Falcon"));
  assert(!b2.setSalary("peter", 37000));
  assert(b2.getSalary("peter") == 0);
  assert(!b2.getRank("peter", lo, hi));
  assert(!b2.del("Peter", "Smith"));
  assert(!b2.del("peter"));
  assert(b2.add("Peter", "Smith", "peter", 40000));
  assert(b2.getSalary("peter") == 40000);

  // CPersonalAgenda b3;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
