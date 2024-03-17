#ifndef __PROGTEST__
#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <array>
#include <random>
#include <type_traits>

// We use std::vector as a reference to check our implementation.
// It is not available in progtest :)
#include <vector>

template <typename T>
struct Ref
{
  bool empty() const { return _data.empty(); }
  size_t size() const { return _data.size(); }

  const T &operator[](size_t index) const { return _data.at(index); }
  T &operator[](size_t index) { return _data.at(index); }

  void insert(size_t index, T value)
  {
    if (index > _data.size())
      throw std::out_of_range("oops");
    _data.insert(_data.begin() + index, std::move(value));
  }

  T erase(size_t index)
  {
    T ret = std::move(_data.at(index));
    _data.erase(_data.begin() + index);
    return ret;
  }

  auto begin() const { return _data.begin(); }
  auto end() const { return _data.end(); }

private:
  std::vector<T> _data;
};

#endif

namespace config
{
  inline constexpr bool PARENT_POINTERS = true;
  inline constexpr bool CHECK_DEPTH = true;
}

// TODO implement
template <typename T>
struct Array
{

  struct Node
  {
    Node *parent;
    Node *leftChild;
    Node *rightChild;
    T value;
    int depthOfLeftChild;
    int depthOfRightChild;
    size_t numberOfvertices;

    Node(T val)
    {
      value = val;
      leftChild = nullptr;
      rightChild = nullptr;
      parent = nullptr;
      numberOfvertices = 1;
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

  const T &operator[](size_t index) const
  {
    if (index >= elements)
      throw std::out_of_range("Index out of range");

    Node *node = root;
    while (true)
    {
      if (getNumberOfVertices(node->leftChild) == index)
        return (const T &)node->value;
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

  T &operator[](size_t index)
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

  void insert(size_t index, T value)
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
  T erase(size_t index)
  {
    if (index >= elements)
      throw std::out_of_range("Index Out of Range");

    Node **node = &root;
    Node *parent = nullptr;
    Node *n = nullptr;
    Node * current = nullptr;

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

    T result = n->value;
    delete n;
    elements--;

    while (current)
    {

      UpdateNumberOfVertices(current);

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

  // Needed to test the structure of the tree.
  // Replace Node with the real type of your nodes
  // and implementations with the ones matching
  // your attributes.
  struct TesterInterface
  {
    // using Node = ...
    static const Node *root(const Array *t) { return t->root; }
    // Parent of root must be nullptr, ignore if config::PARENT_POINTERS == false
    static const Node *parent(const Node *n) { return n->parent; }
    static const Node *right(const Node *n) { return n->rightChild; }
    static const Node *left(const Node *n) { return n->leftChild; }
    static const T &value(const Node *n) { return n->value; }
  };

  // Helper methods
  size_t getNumberOfVertices(Node *node) const
  {
    return node ? node->numberOfvertices : 0;
  }

  void UpdateNumberOfVertices(Node *node) const
  {
    node->numberOfvertices = getNumberOfVertices(node->leftChild) + getNumberOfVertices(node->rightChild) + 1;
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

    node->depthOfLeftChild = (node->leftChild) ? node->leftChild->depth() : -1;
    node->depthOfRightChild = (node->rightChild) ? node->rightChild->depth() : -1;

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = (newRoot->leftChild) ? newRoot->leftChild->depth() : -1;
    newRoot->depthOfRightChild = (newRoot->rightChild) ? newRoot->rightChild->depth() : -1;

    // Continue updating depth values up the tree
    Node *current = newRoot->parent;
    while (current)
    {
      UpdateNumberOfVertices(current);
      current->depthOfLeftChild = (current->leftChild) ? current->leftChild->depth() : -1;
      current->depthOfRightChild = (current->rightChild) ? current->rightChild->depth() : -1;
      current = current->parent;
    }
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

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = (newRoot->leftChild) ? newRoot->leftChild->depth() : -1;
    newRoot->depthOfRightChild = (newRoot->rightChild) ? newRoot->rightChild->depth() : -1;
    UpdateNumberOfVertices(newRoot);

    // Continue updating depth values up the tree
    Node *current = newRoot->parent;
    while (current)
    {
      current->depthOfLeftChild = (current->leftChild) ? current->leftChild->depth() : -1;
      current->depthOfRightChild = (current->rightChild) ? current->rightChild->depth() : -1;
      current = current->parent;
    }
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
      std::cout << node->value << std::endl;
      if (node->leftChild)
      {
        printTreeVisual(node->leftChild, indent + 8);
      }
    }
  }

  Node *root;
  size_t elements;
};

#ifndef __PROGTEST__

struct TestFailed : std::runtime_error
{
  using std::runtime_error::runtime_error;
};

std::string fmt(const char *f, ...)
{
  va_list args1;
  va_list args2;
  va_start(args1, f);
  va_copy(args2, args1);

  std::string buf(vsnprintf(nullptr, 0, f, args1), '\0');
  va_end(args1);

  vsnprintf(buf.data(), buf.size() + 1, f, args2);
  va_end(args2);

  return buf;
}

template <typename T>
struct Tester
{
  Tester() = default;

  size_t size() const
  {
    bool te = tested.empty();
    size_t r = ref.size();
    size_t t = tested.size();
    if (te != !t)
      throw TestFailed(fmt("Size: size %zu but empty is %s.",
                           t, te ? "true" : "false"));
    if (r != t)
      throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
    return r;
  }

  const T &operator[](size_t index) const
  {
    const T &r = ref[index];
    const T &t = tested[index];
    if (r != t)
      throw TestFailed("Op [] const mismatch.");
    return t;
  }

  void assign(size_t index, T x)
  {
    ref[index] = x;
    tested[index] = std::move(x);
    operator[](index);
  }

  void insert(size_t i, T x, bool check_tree_ = false)
  {
    ref.insert(i, x);
    tested.insert(i, std::move(x));
    size();
    if (check_tree_)
      check_tree();
  }

  T erase(size_t i, bool check_tree_ = false)
  {
    T r = ref.erase(i);
    T t = tested.erase(i);
    if (r != t)
      TestFailed(fmt("Erase mismatch at %zu.", i));
    size();
    if (check_tree_)
      check_tree();
    return t;
  }

  void check_tree() const
  {
    using TI = typename Array<T>::TesterInterface;
    auto ref_it = ref.begin();
    bool check_value_failed = false;
    auto check_value = [&](const T &v)
    {
      if (check_value_failed)
        return;
      check_value_failed = (ref_it == ref.end() || *ref_it != v);
      if (!check_value_failed)
        ++ref_it;
    };

    size();

    check_node(TI::root(&tested), decltype(TI::root(&tested))(nullptr), check_value);

    if (check_value_failed)
      throw TestFailed(
          "Check tree: element mismatch");
  }

  template <typename Node, typename F>
  int check_node(const Node *n, const Node *p, F &check_value) const
  {
    if (!n)
      return -1;

    using TI = typename Array<T>::TesterInterface;
    if constexpr (config::PARENT_POINTERS)
    {
      if (TI::parent(n) != p)
        throw TestFailed("Parent mismatch.");
    }

    auto l_depth = check_node(TI::left(n), n, check_value);
    check_value(TI::value(n));
    auto r_depth = check_node(TI::right(n), n, check_value);

    if (config::CHECK_DEPTH && abs(l_depth - r_depth) > 1)
      throw TestFailed(fmt(
          "Tree is not avl balanced: left depth %i and right depth %i.",
          l_depth, r_depth));

    return std::max(l_depth, r_depth) + 1;
  }

  static void _throw(const char *msg, bool s)
  {
    throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
  }

  Array<T> tested;
  Ref<T> ref;
};

void test_insert()
{
  Tester<int> t;

  for (int i = 0; i < 10; i++)
    t.insert(i, i, true);
  for (int i = 0; i < 10; i++)
    t.insert(i, -i, true);
  for (size_t i = 0; i < t.size(); i++)
    t[i];

  for (int i = 0; i < 5; i++)
    t.insert(15, (1 + i * 7) % 17, true);
  for (int i = 0; i < 10; i++)
    t.assign(2 * i, 3 * t[2 * i]);
  for (size_t i = 0; i < t.size(); i++)
    t[i];
}

void test_erase()
{
  Tester<int> t;

  for (int i = 0; i < 10; i++)
    t.insert(i, i, true);
  for (int i = 0; i < 10; i++)
    t.insert(i, -i, true);

  for (size_t i = 3; i < t.size(); i += 2)
    t.erase(i, true);
  for (size_t i = 0; i < t.size(); i++)
    t[i];

  for (int i = 0; i < 5; i++)
    t.insert(3, (1 + i * 7) % 17, true);
  for (size_t i = 1; i < t.size(); i += 3)
    t.erase(i, true);

  for (int i = 0; i < 20; i++)
    t.insert(3, 100 + i, true);

  for (int i = 0; i < 5; i++)
    t.erase(t.size() - 1, true);
  for (int i = 0; i < 5; i++)
    t.erase(0, true);

  for (int i = 0; i < 4; i++)
    t.insert(i, i, true);
  for (size_t i = 0; i < t.size(); i++)
    t[i];
}

enum RandomTestFlags : unsigned
{
  SEQ = 1,
  NO_ERASE = 2,
  CHECK_TREE = 4
};

void test_random(size_t size, unsigned flags = 0)
{
  Tester<size_t> t;
  std::mt19937 my_rand(24707 + size);

  bool seq = flags & SEQ;
  bool erase = !(flags & NO_ERASE);
  bool check_tree = flags & CHECK_TREE;

  for (size_t i = 0; i < size; i++)
  {
    size_t pos = seq ? 0 : my_rand() % (i + 1);
    t.insert(pos, my_rand() % (3 * size), check_tree);
  }

  t.check_tree();

  for (size_t i = 0; i < t.size(); i++)
    t[i];

  for (size_t i = 0; i < 30 * size; i++)
    switch (my_rand() % 7)
    {
    case 1:
    {
      if (!erase && i % 3 == 0)
        break;
      size_t pos = seq ? 0 : my_rand() % (t.size() + 1);
      t.insert(pos, my_rand() % 1'000'000, check_tree);
      break;
    }
    case 2:
      if (erase)
        t.erase(my_rand() % t.size(), check_tree);
      break;
    case 3:
      t.assign(my_rand() % t.size(), 155 + i);
      break;
    default:
      t[my_rand() % t.size()];
    }

  t.check_tree();
}

int main()
{
  try
  {
    std::cout << "Insert test..." << std::endl;
    test_insert();

    std::cout << "Erase test..." << std::endl;
    test_erase();

    std::cout << "Tiny random test..." << std::endl;
    test_random(20, CHECK_TREE);

    std::cout << "Small random test..." << std::endl;
    test_random(200, CHECK_TREE);

    std::cout << "Bigger random test..." << std::endl;
    test_random(5'000);

    std::cout << "Bigger sequential test..." << std::endl;
    test_random(5'000, SEQ);

    std::cout << "All tests passed." << std::endl;
  }
  catch (const TestFailed &e)
  {
    std::cout << "Test failed: " << e.what() << std::endl;
  }
}

#endif
