#include<iostream>
#include<assert.h>

using namespace std;

template <typename T>
struct Node
{
    Node * leftChild;
    Node * rightChild;
    T value;


    Node ( T val)
    {
        value = val;
        leftChild = nullptr;
        rightChild = nullptr;
    }


};


template <typename T>
Node<T> * freeNode ( Node<T> * node)
{
    if(!node)
        return nullptr;
    {
        freeNode(node->leftChild);
        freeNode(node->rightChild);
        delete node;
    }
    return nullptr;

}


template <typename T>
struct BST
{
    Node<T> * root;

    BST()
    {
        root = freeNode(root);
    }



    bool validNode ( Node<T> * node , Node<T> * leftLimit, Node<T> * rightLimit)
    {
        if(!node)
            return true;
        if(leftLimit && node->value <= leftLimit->value)
            return false;
        if(rightLimit && node->value >= rightLimit->value)
            return false;
        
        return validNode(node->rightChild,node,rightLimit) &&
                validNode(node->leftChild,leftLimit,node);
    }


    bool validBST ()
    {
        return validNode(root,nullptr,nullptr);
    }


    bool contains ( T val)
    {
        Node<T> * node = root;

        while (node)
        {
            if(val == node->value)
                return true;
            else if (val < node->value)
                node = node->leftChild;
            else
                node = node->rightChild;
        }


        return false;
    }


    bool insert ( T val)
    {
        Node<T> ** node = &root;
        while(*node)
        {
            if(val == (*node)->value)
                return false;
            else if ( val < (*node)->value)
                node = &((*node)->leftChild);
            else
                node = &((*node)->rightChild);
        }

        *node = new Node<T> (val);
        
        if(!validBST())
        {
            throw std::out_of_range("NOT valid BST");
        }

        return true;

    }

    bool remove ( T val)
    {
        if(!contains(val))
            return false;
        Node<T> ** node = &root;
        while (*node)
        {
            if(val == (*node)->value)
            {
                Node<T> * n = *node;
                if(n->leftChild && n->rightChild)
                {
                    node = &(*node)->leftChild;
                    while((*node)->rightChild)
                    {
                        node = &(*node)->rightChild;
                    }
                    n->value = (*node)->value;
                    n = *node;
                }
                if(n->leftChild)
                    *node = n->leftChild;
                else
                    *node = n->rightChild;

                delete n;

                if(!validBST())
                    throw std::out_of_range("NOT VALID BST");

                
                return true;
            }
            else if ( val < (*node)->value)
                node = &((*node)->leftChild);
            else
                node = &((*node)->rightChild);
        }

        return false;
        
    }


    friend std::ostream & operator<< (std::ostream & os , const BST<T> & bst)
    {
        bst.printTree(os, bst.root);
        return os;
        
    }

    void printTree(std::ostream& os, Node<T>* node) const
    {
        if (node)
        {
            printTree(os, node->leftChild);
            os << node->value << " ";
            printTree(os, node->rightChild);
        }
    }

    




};



int main()
{
    BST<int> bst;
    

    // assert ( insert ( & bst, 6 ) );
	// assert ( insert ( & bst, 5 ) );
	// assert ( insert ( & bst, 16 ) );
	// assert ( insert ( & bst, 11 ) );
	// assert ( insert ( & bst, 3600 ) );
	// assert ( insert ( & bst, 38 ) );
	// assert ( insert ( & bst, 42 ) );
	// assert ( insert ( & bst, -1 ) );
	// assert ( ! insert ( & bst, 42 ) );
	// assert ( ! insert ( & bst, -1 ) );
	// assert ( ! insert ( & bst, 3600 ) );

	// assert ( contains ( & bst, 6 ) );
	// assert ( contains ( & bst, 42 ) );
	// assert ( contains ( & bst, -1 ) );
	// assert ( ! contains ( & bst, 2 ) );
	// assert ( ! contains ( & bst, 13 ) );

	// assert ( remove ( & bst, 42 ) );
	// assert ( ! remove ( & bst, 4 ) );
	// assert ( remove ( & bst, 3600 ) );
	// assert ( remove ( & bst, 6 ) );
	// assert ( remove ( & bst, 38 ) );
	// assert ( remove ( & bst, -1 ) );
	// assert ( ! remove ( & bst, 42 ) );
	// assert ( remove ( & bst, 5 ) );
	// assert ( remove ( & bst, 11 ) );
	// assert ( remove ( & bst, 16 ) );

    assert(bst.insert(6));
    assert(bst.insert(5));
    assert(bst.insert(16));
    assert(bst.insert(11));
    assert(bst.insert(3600));
    assert(bst.insert(38));
    assert(bst.insert(42));
    assert(bst.insert(-1));
    assert(!bst.insert(42));
    assert(!bst.insert(-1));
    assert(!bst.insert(3600));


    assert(bst.contains(6));
    assert(bst.contains(42));
    assert(bst.contains(-1));
    assert(!bst.contains(2));
    assert(!bst.contains(13));



    assert ( bst.remove ( 42 ) );
	assert ( ! bst.remove ( 4 ) );
	assert ( bst.remove ( 3600 ) );

    std::cout << "-------------------" << std::endl;
    std::cout << bst << std::endl;
    std::cout << "-------------------" << std::endl;

	assert ( bst.remove ( 6 ) );
	assert ( bst.remove ( 38 ) );
	assert ( bst.remove ( -1 ) );
	assert ( ! bst.remove ( 42 ) );
	assert ( bst.remove ( 5 ) );
	assert ( bst.remove ( 11 ) );
	assert ( bst.remove ( 16 ) );












}