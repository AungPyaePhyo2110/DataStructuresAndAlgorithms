#ifndef __PROGTEST__
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>

#endif

struct Array
{

  struct Node
  {
    Node *parent;
    Node *leftChild;
    Node *rightChild;
    char value;
    int depthOfLeftChild;
    int depthOfRightChild;
    size_t numberOfvertices;
    size_t numberOflines;

    Node(char val)
    {
      value = val;
      leftChild = nullptr;
      rightChild = nullptr;
      parent = nullptr;
      numberOfvertices = 1;
      numberOflines = 0;
    }

    int depth() const
    {
      if (!leftChild && !rightChild)
        return 0;
      if (!leftChild)
        return depthOfRightChild + 1;
      if (!rightChild)
        return depthOfLeftChild + 1;
      return (depthOfLeftChild > depthOfRightChild ? depthOfLeftChild : depthOfRightChild) + 1;
    }

    int balance() const
    {
      return depthOfLeftChild - depthOfRightChild;
    }

    size_t isNewline() const
    {
      if (value == '\n')
        return 1;
      return 0;
    }
  };

  Node *freeNode(Node *node)
  {
    if (!node)
      return nullptr;
    freeNode(node->leftChild);
    freeNode(node->rightChild);
    delete node;
    node = nullptr;
    return nullptr;
  }

  Array()
  {
    root = nullptr;
    elements = 0;
  }

  ~Array()
  {
    root = freeNode(root);
    elements = 0;
  }

  bool empty() const
  {
    return elements == 0;
  }
  size_t size() const
  {
    return elements;
  }

  const char &operator[](size_t index) const
  {
    if (index >= elements)
      throw std::out_of_range("Index out of range");

    Node *node = root;
    while (true)
    {
      if (getNumberOfVertices(node->leftChild) == index)
        return (const char &)node->value;
      else if (getNumberOfVertices(node->leftChild) < index)
      {
        index = index - getNumberOfVertices(node->leftChild) - 1;
        node = node->rightChild;
      }
      else
      {
        node = node->leftChild;
      }
    }
  }

  char &operator[](size_t index)
  {
    if (index >= elements)
      throw std::out_of_range("Index out of range");

    Node *node = root;
    while (true)
    {
      if (getNumberOfVertices(node->leftChild) == index)
        return node->value;
      else if (getNumberOfVertices(node->leftChild) < index)
      {
        index = index - getNumberOfVertices(node->leftChild) - 1;
        node = node->rightChild;
      }
      else
      {
        node = node->leftChild;
      }
    }
  }

  void insert(size_t index, char value)
  {

    // std::cout << "BEFORE " << std::endl;
    // printTreeVisual(root);
    // std::cout << "---------------------" << size() << "---------------------" << std::endl;

    // std::cout << "INSERTING " << value << "   in " << index << std::endl;

    if (index > elements)
      throw std::out_of_range("Index out of range");

    Node **node = &root;
    Node *parent = nullptr;

    while (*node)
    {
      parent = *node;
      if (index <= getNumberOfVertices((*node)->leftChild))
      {
        node = &(*node)->leftChild;
      }
      else
      {
        index -= getNumberOfVertices((*node)->leftChild) + 1;
        node = &(*node)->rightChild;
      }
    }

    Node *newNode = new Node(value);
    *node = newNode;
    newNode->parent = parent;
    newNode->numberOfvertices = 1;
    elements++;

    Node *current = newNode;
    while (current)
    {
      UpdateNumberOfVertices(current);
      UpdateNumberofLines(current);

      current->depthOfLeftChild = (current->leftChild) ? (current->leftChild->depth()) : -1;
      current->depthOfRightChild = (current->rightChild) ? (current->rightChild->depth()) : -1;

      if (current->balance() > 1)
      {
        if (current->leftChild->balance() >= 0)
        {
          rightRotate(current);
          current = current->parent;
          continue;
        }
        if (current->leftChild->balance() < 0)
        {
          leftRotate(current->leftChild);
          rightRotate(current);
          current = current->parent;
          continue;
        }
      }
      else if (current->balance() < -1)
      {
        if (current->rightChild->balance() <= 0)
        {
          leftRotate(current);
          current = current->parent;
          continue;
        }
        if (current->rightChild->balance() > 0)
        {
          rightRotate(current->rightChild);
          leftRotate(current);
          current = current->parent;
          continue;
        }
      }

      current = current->parent;
    }

    // std::cout << "After " << std::endl;
    // printTreeVisual(root);
    // std::cout << "---------------------" << size() << "---------------------" << std::endl;
  }
  char erase(size_t index)
  {
    if (index >= elements)
      throw std::out_of_range("Index Out of Range");

    // std::cout << "ERASING " << index << std::endl;


    Node **node = &root;
    Node *parent = nullptr;
    Node *n = nullptr;
    Node *current = nullptr;

    while (true)
    {
      if (getNumberOfVertices((*node)->leftChild) == index)
      {
        n = *node;
        if ((*node)->leftChild && (*node)->rightChild)
        {
          parent = (*node);
          node = &(*node)->leftChild;
          while ((*node)->rightChild)
          {
            parent = *node;
            node = &(*node)->rightChild;
          }

          std::swap(n->value, (*node)->value);
          n = *node;
          current = (*node)->parent;
        }
        if (n->leftChild)
        {
          *node = n->leftChild;
          current = *node;
          (*node)->parent = parent;
        }
        else if (n->rightChild)
        {
          *node = n->rightChild;
          current = *node;
          (*node)->parent = parent;
        }
        else
        {
          if (n->parent)
          {
            current = (*node)->parent;
            if (n->parent->leftChild && n->parent->leftChild == n)
              n->parent->leftChild = nullptr;
            else if (n->parent->rightChild && n->parent->rightChild == n)
              n->parent->rightChild = nullptr;
          }
        }

        break;
      }
      else if (getNumberOfVertices((*node)->leftChild) < index)
      {
        index = index - getNumberOfVertices((*node)->leftChild) - 1;
        parent = *node;
        node = &(*node)->rightChild;
      }
      else
      {
        parent = *node;
        node = &(*node)->leftChild;
      }
    }

    char result = n->value;
    delete n;
    elements--;

    while (current)
    {

      UpdateNumberOfVertices(current);
      UpdateNumberofLines(current);

      current->depthOfRightChild = (current->rightChild) ? (current->rightChild->depth()) : -1;
      current->depthOfLeftChild = (current->leftChild) ? (current->leftChild->depth()) : -1;

      if (current->balance() > 1 && current->leftChild->balance() >= 0)
      {
        rightRotate(current);
        current = current->parent;
        continue;
      }

      if (current->balance() < -1 && current->rightChild->balance() <= 0)
      {
        leftRotate(current);
        current = current->parent;
        continue;
      }

      if (current->balance() > 1 && current->leftChild->balance() < 0)
      {
        leftRotate(current->leftChild);
        rightRotate(current);
        current = current->parent;
        continue;
      }

      if (current->balance() < -1 && current->rightChild->balance() > 0)
      {
        rightRotate(current->rightChild);
        leftRotate(current);
        current = current->parent;
        continue;
      }
      current = current->parent;
    }

    return result;
  }

  size_t char2line(size_t k) const
  { 
    size_t result = 0;
    Node *node = root;
    while (true)
    {
      if (k == getNumberOfVertices(node->leftChild))
      {
        result +=  getNumberOfLines(node->leftChild);
        return result;
      } 
      else if (k > getNumberOfVertices(node->leftChild))
      {
        k -= getNumberOfVertices(node->leftChild) + 1;
        result += getNumberOfLines(node->leftChild)+node->isNewline();
        node = node->rightChild;
      }
      else
      {
        node = node->leftChild;
      }
    }

  }

  size_t lStart(size_t k) const
  { 
    //subtract 1 from k so that we can find the '\n' of the previous line
    k -= 1;
    Node * node = root;
    size_t result = 0;
    while(true)
    {
      if( k == getNumberOfLines(node->leftChild))
      {
        // std::cout << "reached "  << node->value << std::endl;
        // std::cout << result << std::endl;
        //reach the part of the line we wanted
        //now have to find the index of '\n' in the line
        //first check if we reach '\n' -- done return
        // otherwise
        //move one step to right '\n' is always on the right
        //check if there are lines before the node (on the left)
        //if yes, move to left
        //if no, move to right
        //move until we reach '\n' and no lines on left
        if(node->value == '\n')
        {
          result += getNumberOfVertices(node->leftChild);
          return result+1;
        }
        else
        {
          result += getNumberOfVertices(node->leftChild)+1;
          node = node->rightChild;
          // std::cout << "MOVED TO RIGHT" << result <<  std::endl;
          while(true)
          {
            if(node->value=='\n' && getNumberOfLines(node->leftChild) == 0)
            {
              return result+ getNumberOfVertices(node->leftChild) + 1 ;
            }
            else if (getNumberOfLines(node->leftChild)>=1)
            {
              node = node->leftChild;

            }
            else
            {
              result+= getNumberOfVertices(node->leftChild)+1;
              node = node->rightChild;
            }

          }
        }
      }
      else if ( k > getNumberOfLines(node->leftChild))
      {
        k -= getNumberOfLines(node->leftChild) + node->isNewline();
        result += getNumberOfVertices(node->leftChild)+1;
        // std::cout << "AFTER MOVING TO RIGHT  " << result << std::endl;
        node = node->rightChild;
      }
      else
      {
        node = node->leftChild;
      }

    }


  }

  // Helper methods
  size_t getNumberOfVertices(Node *node) const
  {
    return node ? node->numberOfvertices : 0;
  }

  void UpdateNumberOfVertices(Node *node) const
  {
    node->numberOfvertices = getNumberOfVertices(node->leftChild) + getNumberOfVertices(node->rightChild) + 1;
  }

  size_t getNumberOfLines(Node *node) const
  {
    return node ? node->numberOflines : 0;
  }

  void UpdateNumberofLines(Node *node) const
  {
    node->numberOflines = getNumberOfLines(node->leftChild) + getNumberOfLines(node->rightChild) + node->isNewline();
  }

  void leftRotate(Node *node)
  {
    // std::cout << "LEFT ROTATING" << std::endl;
    Node *newRoot = node->rightChild;
    node->rightChild = newRoot->leftChild;
    if (node->rightChild)
      node->rightChild->parent = node;

    newRoot->parent = node->parent;
    if (!node->parent)
      root = newRoot;
    else if (node == node->parent->leftChild)
      node->parent->leftChild = newRoot;
    else
      node->parent->rightChild = newRoot;

    newRoot->leftChild = node;
    node->parent = newRoot;

    UpdateNumberOfVertices(node);
    UpdateNumberOfVertices(newRoot);

    UpdateNumberofLines(node);
    UpdateNumberofLines(newRoot);

    node->depthOfLeftChild = (node->leftChild) ? node->leftChild->depth() : -1;
    node->depthOfRightChild = (node->rightChild) ? node->rightChild->depth() : -1;

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = (newRoot->leftChild) ? newRoot->leftChild->depth() : -1;
    newRoot->depthOfRightChild = (newRoot->rightChild) ? newRoot->rightChild->depth() : -1;

    // Continue updating depth values up the tree
  }

  void rightRotate(Node *node)
  {

    // std::cout << "RIGHT ROTATING" << std::endl;

    Node *newRoot = node->leftChild;
    node->leftChild = newRoot->rightChild;
    if (node->leftChild)
      node->leftChild->parent = node;

    newRoot->parent = node->parent;
    if (!node->parent)
      root = newRoot;
    else if (node == node->parent->leftChild)
      node->parent->leftChild = newRoot;
    else
      node->parent->rightChild = newRoot;

    newRoot->rightChild = node;
    node->parent = newRoot;

    // Update depth values for the rotated nodes
    node->depthOfLeftChild = (node->leftChild) ? node->leftChild->depth() : -1;
    node->depthOfRightChild = (node->rightChild) ? node->rightChild->depth() : -1;
    UpdateNumberOfVertices(node);
    UpdateNumberofLines(node);

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = (newRoot->leftChild) ? newRoot->leftChild->depth() : -1;
    newRoot->depthOfRightChild = (newRoot->rightChild) ? newRoot->rightChild->depth() : -1;
    UpdateNumberOfVertices(newRoot);
    UpdateNumberofLines(newRoot);


  }

  void printTreeVisual(const Node *node, int indent = 0)
  {

    if (node)
    {
      if (node->rightChild)
      {
        printTreeVisual(node->rightChild, indent + 8);
      }
      if (indent > 0)
      {
        std::cout << std::setw(indent) << " ";
      }
        if(node->value == '\n')
          std::cout << "| + " << node->numberOflines  << " + "  << node->numberOfvertices << std::endl;
        else
          std::cout << node->value << " +  " << node->numberOflines << " + "  << node->numberOfvertices << std::endl;
      if (node->leftChild)
      {
        printTreeVisual(node->leftChild, indent + 8);
      }
    }
  }

  Node *root;
  size_t elements;
};

struct TextEditorBackend
{
  TextEditorBackend(const std::string &text)
  {
    charArray = new Array();
    numberOflines = 1;
    for (char c : text)
    {
      charArray->insert(charArray->size(), c);
      if (c == '\n')
        numberOflines++;
    // std::cout << "----------------------------" << lines() << "----------------------------" << std::endl;

    }
  }

  ~TextEditorBackend()
  {
    delete charArray;
  }

  size_t size() const
  {
    return charArray->size();
  }
  size_t lines() const
  {
    return numberOflines;
  }

  char at(size_t i) const
  {
    if (i >= size())
      throw std::out_of_range("std::out_of_range ");

    return (*charArray)[i];
  }
  void edit(size_t i, char c)
  {
    if (i >= size())
      throw std::out_of_range("std::out_of_range ");
    // do something when the changing chars are newline
    erase(i);
    insert(i, c);
  }
  void insert(size_t i, char c)
  {
    if (i > size())
      throw std::out_of_range("std::out_of_range ");

    (*charArray).insert(i, c);
    if (c == '\n')
      numberOflines++;

    
  }
  void erase(size_t i)
  {
    if (i > size())
      throw std::out_of_range("std::out_of_range ");
    if ((*charArray).erase(i) == '\n')
      numberOflines--;
  }

  size_t line_start(size_t r) const
  {
    if (r >= lines())
      throw std::out_of_range("Line out of range");
    if (r == 0)
      return 0;
    return charArray->lStart(r);
  }
  size_t line_length(size_t r) const
  {
    if (r >= lines())
      throw std::out_of_range("Line out of range");
    if (r == lines() - 1)
      return size() - line_start(r);


    // std::cout << "line length:  " <<  line_start(r+1) << " - " << line_start(r) << std::endl; 

    return line_start(r+1) - line_start(r);
  }
  size_t char_to_line(size_t i) const
  {
    if (i >= size())
      throw std::out_of_range("Index out of range");

    return charArray->char2line(i);
  }

  Array *charArray;
  size_t numberOflines;
};

#ifndef __PROGTEST__

////////////////// Dark magic, ignore ////////////////////////

template < typename T >
auto quote(const T& t) { return t; }

std::string quote(const std::string& s) {
  std::string ret = "\"";
  for (char c : s) if (c != '\n') ret += c; else ret += "\\n";
  return ret + "\"";
}

#define STR_(a) #a
#define STR(a) STR_(a)

#define CHECK_(a, b, a_str, b_str) do { \
    auto _a = (a); \
    decltype(a) _b = (b); \
    if (_a != _b) { \
      std::cout << "Line " << __LINE__ << ": Assertion " \
        << a_str << " == " << b_str << " failed!" \
        << " (lhs: " << quote(_a) << ")" << std::endl; \
      fail++; \
    } else ok++; \
  } while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)

#define CHECK_ALL(expr, ...) do { \
    std::array _arr = { __VA_ARGS__ }; \
    for (size_t _i = 0; _i < _arr.size(); _i++) \
      CHECK_((expr)(_i), _arr[_i], STR(expr) "(" << _i << ")", _arr[_i]); \
  } while (0)

#define CHECK_EX(expr, ex) do { \
    try { \
      (expr); \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but no exception was raised." << std::endl; \
    } catch (const ex&) { ok++; \
    } catch (...) { \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but got different exception." << std::endl; \
    } \
  } while (0)
 
////////////////// End of dark magic ////////////////////////


std::string text(const TextEditorBackend& t) {
  std::string ret;
  for (size_t i = 0; i < t.size(); i++) ret.push_back(t.at(i));
  return ret;
}

void test1(int& ok, int& fail) {
	TextEditorBackend s("123\n456\n789");
  CHECK(s.size(), 11);
	CHECK(text(s), "123\n456\n789");
  CHECK(s.lines(), 3);
  CHECK_ALL(s.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2);
  CHECK_ALL(s.line_start, 0, 4, 8);
  CHECK_ALL(s.line_length, 4, 4, 3);
}

void test2(int& ok, int& fail) {
	TextEditorBackend t("123\n456\n789\n");
  CHECK(t.size(), 12);
	CHECK(text(t), "123\n456\n789\n");
  CHECK(t.lines(), 4);
  CHECK_ALL(t.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2,2);
  CHECK_ALL(t.line_start, 0, 4, 8, 12);
  CHECK_ALL(t.line_length, 4, 4, 4, 0);
}

void test3(int& ok, int& fail) {
	TextEditorBackend t("asdfasdfasdf");

  CHECK(t.size(), 12);
  CHECK(text(t), "asdfasdfasdf");
	CHECK(t.lines(), 1);
  CHECK_ALL(t.char_to_line, 0,0,0,0, 0,0,0,0, 0,0,0,0);
	CHECK(t.line_start(0), 0);
	CHECK(t.line_length(0), 12);
	
	t.insert(0, '\n');
  CHECK(t.size(), 13);
  CHECK(text(t), "\nasdfasdfasdf");
	CHECK(t.lines(), 2);
  CHECK_ALL(t.line_start, 0, 1);
	
  t.insert(4, '\n');
  CHECK(t.size(), 14);
  CHECK(text(t), "\nasd\nfasdfasdf");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);
	
  t.insert(t.size(), '\n');
  CHECK(t.size(), 15);
  CHECK(text(t), "\nasd\nfasdfasdf\n");
	CHECK(t.lines(), 4);
  CHECK_ALL(t.line_start, 0, 1, 5, 15);
	
	t.edit(t.size() - 1, 'H');
  CHECK(t.size(), 15);
  CHECK(text(t), "\nasd\nfasdfasdfH");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);

	t.erase(8);
  CHECK(t.size(), 14);
  CHECK(text(t), "\nasd\nfasfasdfH");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);

  t.erase(4);
  CHECK(t.size(), 13);
  CHECK(text(t), "\nasdfasfasdfH");
	CHECK(t.lines(), 2);
  CHECK_ALL(t.line_start, 0, 1);
}

void test_ex(int& ok, int& fail) {
	TextEditorBackend t("123\n456\n789\n");
  CHECK_EX(t.at(12), std::out_of_range);

  CHECK_EX(t.insert(13, 'a'), std::out_of_range);
  CHECK_EX(t.edit(12, 'x'), std::out_of_range);
  CHECK_EX(t.erase(12), std::out_of_range);

  CHECK_EX(t.line_start(4), std::out_of_range);
  CHECK_EX(t.line_start(40), std::out_of_range);
  CHECK_EX(t.line_length(4), std::out_of_range);
  CHECK_EX(t.line_length(6), std::out_of_range);
  CHECK_EX(t.char_to_line(12), std::out_of_range);
  CHECK_EX(t.char_to_line(25), std::out_of_range);
}

void runTest(const char* testName, bool condition) {
    if (condition) {
        std::cout << testName << ": Passed\n";
    } else {
        std::cout << testName << ": Failed\n";
    }
}

int main() {
  int ok = 0, fail = 0;
  if (!fail) test1(ok, fail);
  if (!fail) test2(ok, fail);
  if (!fail) test3(ok, fail);
  if (!fail) test_ex(ok, fail);
  
  if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
  else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;


    TextEditorBackend editor1("Hello, World!");
    runTest("Test Case 1 - size()", editor1.size() == 13);
    runTest("Test Case 1 - lines()", editor1.lines() == 1);
    runTest("Test Case 1 - at()", editor1.at(7) == 'W');
    editor1.edit(7, 'w');
    runTest("Test Case 1 - edit() and at()", editor1.at(7) == 'w');
    editor1.insert(5, ',');
    runTest("Test Case 1 - insert() and size()", editor1.size() == 14);
    editor1.erase(5);
    runTest("Test Case 1 - erase() and size()", editor1.size() == 13);

    // Test Case 2: Multiple lines
    TextEditorBackend editor2("Line1\nLine2\nLine3");
    // editor2.charArray->printTreeVisual(editor2.charArray->root);
    runTest("Test Case 2 - lines()", editor2.lines() == 3);
    runTest("Test Case 2 - line_start(0)", editor2.line_start(0) == 0);
    runTest("Test Case 2 - line_start(1)", editor2.line_start(1) == 6);
    runTest("Test Case 2 - line_length(0)", editor2.line_length(0) == 6);
    runTest("Test Case 2 - line_length(1)", editor2.line_length(1) == 6);
    

    std::cout << editor2.line_length(1) << std::endl;

    // Test Case 3: Char to line conversion
    runTest("Test Case 3 - char_to_line()", editor2.char_to_line(5) == 0);
    runTest("Test Case 3 - char_to_line()", editor2.char_to_line(8) == 1);
    runTest("Test Case 3 - char_to_line()", editor2.char_to_line(15) == 2);


        // Test Case 4: Empty text
    TextEditorBackend editor3("");
    runTest("Test Case 4 - lines()", editor3.lines() == 1);
    runTest("Test Case 4 - size()", editor3.size() == 0);

    // Test Case 5: Insert at the end
    TextEditorBackend editor4("123");
    editor4.insert(3, '4');
    runTest("Test Case 5 - insert() at the end", editor4.size() == 4 && editor4.at(3) == '4');

    // Test Case 6: Edge case - line_start and line_length for last line
    TextEditorBackend editor5("abc\ndef");
    runTest("Test Case 6 - line_start() for last line", editor5.line_start(1) == 4);
    runTest("Test Case 6 - line_length() for last line", editor5.line_length(1) == 3);


}

#endif


