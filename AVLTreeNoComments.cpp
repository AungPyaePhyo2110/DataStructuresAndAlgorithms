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

// We use std::set as a reference to check our implementation.
// It is not available in progtest :)
#include <set>

template < typename T >
struct Ref {
  size_t size() const { return _data.size(); }
  const T* find(const T& value) const {
    auto it = _data.find(value);
    if (it == _data.end()) return nullptr;
    return &*it;
  }
  bool insert(const T& value) { return _data.insert(value).second; }
  bool erase(const T& value) { return _data.erase(value); }

  auto begin() const { return _data.begin(); }
  auto end() const { return _data.end(); }

  private:
  std::set<T> _data;
};

#endif


namespace config {
  // Enable to check that the tree is AVL balanced.
  inline constexpr bool CHECK_DEPTH = true;

  // Disable if your implementation does not have parent pointers
  inline constexpr bool PARENT_POINTERS = true;
}

// TODO implement
template <typename T>
struct Node
{ 
    Node * parent;
    Node * leftChild;
    Node * rightChild;
    T value;
    int depthOfLeftChild;
    int depthOfRightChild;


    Node ( T val)
    {
      value = val;
      leftChild = nullptr;
      rightChild = nullptr;
      parent = nullptr;
    }




    int depth ( ) const
    {
      if(!leftChild && !rightChild)
        return 0;
      if(!leftChild)
        return depthOfRightChild+1;
      if(!rightChild)
        return depthOfLeftChild+1;
      return (depthOfLeftChild > depthOfRightChild ? depthOfLeftChild : depthOfRightChild) + 1;
    }


    int balance () const
    {
      return depthOfLeftChild - depthOfRightChild;
    }

};


template<typename T>
Node<T> * freeNode ( Node<T> * node)
{
  if(!node)
    return nullptr;
  freeNode(node->leftChild);
  freeNode(node->rightChild);
  delete node;
  node = nullptr;
  return nullptr;
}



template < typename T >
struct Tree {

  Tree()
  {
    root = nullptr;
    numberOfVertices = 0 ;
  }

  ~Tree()
  {
    root = freeNode(root);
    numberOfVertices = 0;
  }

  size_t size() const
  {
    return numberOfVertices;
  }
  const T* find(const T& value) const
  {
    Node<T> * node = root;
    while(node)
    {
      if(value == node->value)
      {
        return &node->value;
      }
      else if ( value < node->value)
      {
        node = node->leftChild;
      }
      else
      {
        node = node->rightChild;
      }

    }
    return nullptr;
  }
  bool insert(T value)
  {

    Node<T> ** node = &root;
    Node<T> * parent = nullptr;
    while(*node)
    {
      if(value == (*node)->value)
      {
        return false;
      }
      else if( value < (*node)->value)
      { 
        parent = *node;
        node = &(*node)->leftChild;
      }
      else
      {
        parent = *node;
        node = &(*node)->rightChild;
      }
    }
    
    Node<T> * newNode = new Node(value);
    *node = newNode;
    newNode->parent = parent;
    numberOfVertices++;

    // change the depth of parent

    Node<T> * current = newNode;
    while(current)
    {
      current->depthOfLeftChild = (current->leftChild) ?( current->leftChild->depth()) : -1 ;
      current->depthOfRightChild = (current->rightChild) ? (current->rightChild->depth()) : -1;
      //      *
      //     /
      //    *(node)
      //   /
      //  * (value is inserted in this subtree)
      // left left case ---->rightrotate with node
      if(current->balance() > 1 && current->leftChild->balance() >= 0)
      { 
        rightRotate(current);
        current = current->parent;
        continue;
      }

      //  *
      //   
      //    *
      //     
      //      * (value is inserted in this subtree)
      //  right right case ---> left rotate

      if(current->balance() < -1 && current->rightChild->balance() <= 0)
      {
        leftRotate(current);
        current = current->parent;
        continue;
      }
      //     *
      //    /
      //    *(node)
      //     
      //      *(value is inserted in this subtree)
      // left right case ---> leftrotate with node and right rotate
      if(current->balance() > 1 && current->leftChild->balance() < 0)
      {

        leftRotate(current->leftChild);
        rightRotate(current);
        current = current->parent;
        continue;
      }

       //     *
      //        
      //         *(node)
      //        /
      //        *(value is inserted in this subtree)
      // right left case ---> rigthrotate with node and left rotate
      if( current->balance() < -1 && current->rightChild->balance() > 0)
      {
        rightRotate(current->rightChild);
        leftRotate(current);
        current = current->parent;
        continue;
      }
      current = current->parent;
    }


  //   printTreeVisual(root);

    

  // std::cout << "---------------------------" << std::endl;



    return true;
  }



  bool erase(const T& value)
  { 
    
    
    if(size()==0)
      return false;

    bool deleted = false;

    Node<T> ** node = &root;
    Node<T> * parent = nullptr;
    Node<T> * current = nullptr;
    while(*node)
    {
      if(value == (*node)->value)
      {
        Node<T> * n = *node;
        if((*node)->leftChild && (*node)->rightChild)
        { 
          parent = (*node);
          node = &(*node)->leftChild;
          while((*node)->rightChild)
          {
            parent = *node;
            node = &(*node)->rightChild;
          }

          std::swap(n->value,(*node)->value);
          n = *node;
          current = (*node)->parent;
        }
        if(n->leftChild)
        {
            *node = n->leftChild;
            current = *node;
          (*node)->parent = parent;
        }
        else if(n->rightChild)
        {
            *node  = n->rightChild;
            current = *node;
            (*node)->parent = parent;
        }
        else
        {
          if(n->parent)
          {
            current = (*node)->parent;
            if(n->parent->leftChild && n->parent->leftChild == n)
              n->parent->leftChild = nullptr;
            else if (n->parent->rightChild && n->parent->rightChild ==n)
              n->parent->rightChild = nullptr;
            
          }

        }

 
        delete n;
        

        numberOfVertices--;
        deleted = true;
        break;
      }
      else if ( value < (*node)->value)
      {
        parent = *node;
        node = &(*node)->leftChild;
      }
      else
      {
        parent = *node;
        node = &(*node)->rightChild;
      }
    }




    

   if(!deleted)
   {
    return false;
   }




    while(current)
    {
      current->depthOfRightChild = (current->rightChild) ? (current->rightChild->depth()) : -1;
      current->depthOfLeftChild = (current->leftChild) ?( current->leftChild->depth()) : -1 ;

      if(current->balance() > 1 && current->leftChild->balance() >= 0)
      { 
        rightRotate(current);
        current = current->parent;
        continue;
      }


      if(current->balance() < -1 && current->rightChild->balance() <= 0)
      {
        leftRotate(current);
        current = current->parent;
        continue;
      }

      if(current->balance() > 1 && current->leftChild->balance() < 0)
      {
        leftRotate(current->leftChild);
        rightRotate(current);
        current = current->parent;
        continue;
      }
      
 

      if( current->balance() < -1 && current->rightChild->balance() > 0)
      {
        rightRotate(current->rightChild);
        leftRotate(current);
        current = current->parent;
        continue;
      }
      current = current->parent;

    }

    
        // printTreeVisual(root);

  // std::cout << "---------------------------" << std::endl;


    return true;

  }

  // Needed to test the structure of the tree.
  // Replace Node with the real type of your nodes
  // and implementations with the ones matching
  // your attributes.
  struct TesterInterface {
    // using Node = Node<T>;
    static const Node<T> *root(const Tree *t) { return t->root; }
    // Parent of root must be nullptr, ignore if config::PARENT_POINTERS == false
    static const Node<T> *parent(const Node<T> *n) { return n->parent; }
    static const Node<T> *right(const Node<T> *n) { return n->rightChild; }
    static const Node<T> *left(const Node<T> *n) { return n->leftChild; }
    static const T& value(const Node<T> *n) { return n->value; }
  };

  void leftRotate ( Node<T> *  node)
  {
    // std::cout << "LEFT ROTATING" << std::endl;
    Node<T> * newRoot = node->rightChild;
    node->rightChild = newRoot->leftChild;
    if(node->rightChild)
      node->rightChild->parent = node;
    

    newRoot->parent = node->parent;
    if(!node->parent)
      root = newRoot;
    else if(node == node->parent->leftChild)
      node->parent->leftChild = newRoot;
    else
      node->parent->rightChild = newRoot;



    newRoot->leftChild = node;
    node->parent = newRoot;

    node->depthOfLeftChild = (node->leftChild) ? node->leftChild->depth() : -1;
    node->depthOfRightChild = (node->rightChild) ? node->rightChild->depth() : -1;

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = node->depth() - 1;
    newRoot->depthOfRightChild = (newRoot->rightChild) ? newRoot->rightChild->depth() : -1;
    
    // Continue updating depth values up the tree
    Node<T>* current = newRoot->parent;
    while (current) {
        current->depthOfLeftChild = (current->leftChild) ? current->leftChild->depth() : -1;
        current->depthOfRightChild = (current->rightChild) ? current->rightChild->depth() : -1;
        current = current->parent;
    }


  }

  void rightRotate ( Node<T> *  node)
  {

    // std::cout << "RIGHT ROTATING" << std::endl;

    Node<T> * newRoot = node->leftChild;
    node->leftChild = newRoot->rightChild;;
    if(node->leftChild)
      node->leftChild->parent = node;
    
    newRoot->parent = node->parent;
    if(!node->parent)
      root = newRoot;
    else if(node == node->parent->leftChild)
      node->parent->leftChild = newRoot;
    else
      node->parent->rightChild = newRoot;

    newRoot->rightChild = node;
    node->parent = newRoot;


    // Update depth values for the rotated nodes
    node->depthOfLeftChild = (node->leftChild) ? node->leftChild->depth() : -1;
    node->depthOfRightChild = (node->rightChild) ? node->rightChild->depth() : -1;

    // Update depth values for the new root (newRoot in this case)
    newRoot->depthOfLeftChild = (newRoot->leftChild) ? newRoot->leftChild->depth() : -1;
    newRoot->depthOfRightChild = node->depth() - 1;

    // Continue updating depth values up the tree
    Node<T>* current = newRoot->parent;
    while (current) {
        current->depthOfLeftChild = (current->leftChild) ? current->leftChild->depth() : -1;
        current->depthOfRightChild = (current->rightChild) ? current->rightChild->depth() : -1;
        current = current->parent;
    }

  }

void printTreeVisual(const Node<T>* node, int indent = 0) {

    if (node) {
        if (node->rightChild) {
            printTreeVisual(node->rightChild, indent + 4);
        }
        if (indent > 0) {
            std::cout << std::setw(indent) << " ";
        }
        std::cout << node->value << std::endl;
        if (node->leftChild) {
            printTreeVisual(node->leftChild, indent + 4);
        }
    }

}



  Node<T> * root;
  size_t numberOfVertices;

  

};


#ifndef __PROGTEST__

struct TestFailed : std::runtime_error {
  using std::runtime_error::runtime_error;
};

std::string fmt(const char *f, ...) {
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

template < typename T >
struct Tester {
  Tester() = default;

  void size() const {
    size_t r = ref.size();
    size_t t = tested.size();
    if (r != t) throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
  }

  void find(const T& x) const {
    auto r = ref.find(x);
    auto t = tested.find(x);
    bool found_r = r != nullptr;
    bool found_t = t != nullptr;

    if (found_r != found_t) _throw("Find mismatch", found_r);
    if (found_r && *t != x) throw TestFailed("Find: found different value");
  }

  void insert(const T& x, bool check_tree_ = false) {
    auto succ_r = ref.insert(x);
    auto succ_t = tested.insert(x);
    if (succ_r != succ_t) _throw("Insert mismatch", succ_r);
    size();
    if (check_tree_) check_tree();
  }

  void erase(const T& x, bool check_tree_ = false) {
    bool succ_r = ref.erase(x);
    auto succ_t = tested.erase(x);
    if (succ_r != succ_t) _throw("Erase mismatch", succ_r);
    size();
    if (check_tree_) check_tree();
  }

  struct NodeCheckResult {
    const T* min = nullptr;
    const T* max = nullptr;
    int depth = -1;
    size_t size = 0;
  };

  void check_tree() const {
    using TI = typename Tree<T>::TesterInterface;
    auto ref_it = ref.begin();
    bool check_value_failed = false;
    auto check_value = [&](const T& v) {
      if (check_value_failed) return;
      check_value_failed = (ref_it == ref.end() || *ref_it != v);
      if (!check_value_failed) ++ref_it;
    };

    auto r = check_node(TI::root(&tested), decltype(TI::root(&tested))(nullptr), check_value);
    size_t t_size = tested.size();

    if (t_size != r.size) throw TestFailed(
      fmt("Check tree: size() reports %zu but expected %zu.", t_size, r.size));

    if (check_value_failed) throw TestFailed(
      "Check tree: element mismatch");

    size();
  }

  template < typename Node, typename F >
  NodeCheckResult check_node(const Node* n, const Node* p, F& check_value) const {
    if (!n) return {};
    
    using TI = typename Tree<T>::TesterInterface;
    if constexpr(config::PARENT_POINTERS) {
      if (TI::parent(n) != p) throw TestFailed("Parent mismatch.");
    }

    auto l = check_node(TI::left(n), n, check_value);
    check_value(TI::value(n));
    auto r = check_node(TI::right(n), n, check_value);

    if (l.max && !(*l.max < TI::value(n)))
      throw TestFailed("Max of left subtree is too big.");
    if (r.min && !(TI::value(n) < *r.min))
      throw TestFailed("Min of right subtree is too small.");

    if (config::CHECK_DEPTH && abs(l.depth - r.depth) > 1) throw TestFailed(fmt(
      "Tree is not avl balanced: left depth %i and right depth %i.",
      l.depth, r.depth
    ));

    return {
      l.min ? l.min : &TI::value(n),
      r.max ? r.max : &TI::value(n),
      std::max(l.depth, r.depth) + 1, 1 + l.size + r.size
    };
  }

  static void _throw(const char *msg, bool s) {
    throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
  }

  Tree<T> tested;
  Ref<T> ref;
};


void test_insert() {
  Tester<int> t;

  for (int i = 0; i < 10; i++) t.insert(i, true);
  for (int i = -10; i < 20; i++) t.find(i);

  for (int i = 0; i < 10; i++) t.insert((1 + i * 7) % 17, true);
  for (int i = -10; i < 20; i++) t.find(i);
}

void test_erase() {
  Tester<int> t;

  for (int i = 0; i < 10; i++) t.insert((1 + i * 7) % 17, true);
  for (int i = -10; i < 20; i++) t.find(i);
  
  for (int i = 3; i < 22; i += 2) t.erase(i, true);
  for (int i = -10; i < 20; i++) t.find(i);
  
  for (int i = 0; i < 10; i++) t.insert((1 + i * 13) % 17 - 8, true);
  for (int i = -10; i < 20; i++) t.find(i);
  
  for (int i = -4; i < 10; i++) t.erase(i, true);
  for (int i = -10; i < 20; i++) t.find(i);
}

enum RandomTestFlags : unsigned {
  SEQ = 1, NO_ERASE = 2, CHECK_TREE = 4
};

void test_random(size_t size, unsigned flags = 0) {
  Tester<size_t> t;
  std::mt19937 my_rand(24707 + size);

  bool seq = flags & SEQ;
  bool erase = !(flags & NO_ERASE);
  bool check_tree = flags & CHECK_TREE;

  for (size_t i = 0; i < size; i++)
    t.insert(seq ? 2*i : my_rand() % (3*size), check_tree);

  t.check_tree();

  for (size_t i = 0; i < 3*size + 1; i++) t.find(i);

  for (size_t i = 0; i < 30*size; i++) switch (my_rand() % 5) {
    case 1: t.insert(my_rand() % (3*size), check_tree);
      break;
    case 2: if (erase) t.erase(my_rand() % (3*size), check_tree);
      break;
    default:
      t.find(my_rand() % (3*size));
  }

  t.check_tree();
}

int main() {
  try {
    std::cout << "Insert test..." << std::endl;
    test_insert();

    std::cout << "Erase test..." << std::endl;
    test_erase();
    
    std::cout << "Tiny random test..." << std::endl;
    test_random(20, CHECK_TREE);
    
    std::cout << "Small random test..." << std::endl;
    test_random(200, CHECK_TREE);
    
    std::cout << "Big random test..." << std::endl;
    test_random(50'000);
    
    std::cout << "Big sequential test..." << std::endl;
    test_random(50'000, SEQ);

    std::cout << "All tests passed." << std::endl;
  } catch (const TestFailed& e) {
    std::cout << "Test failed: " << e.what() << std::endl;
  }
}

#endif


