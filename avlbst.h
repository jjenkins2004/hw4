#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
    void printBalance();
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void printNodeBalance(AVLNode<Key, Value>* n);

    // Add helper functions here
    std::pair<bool, AVLNode<Key, Value>*> insert_node(const std::pair<const Key, Value> &new_item);
    void rotate(AVLNode<Key, Value>* node, std::pair<AVLNode<Key, Value>*, AVLNode<Key, Value>*> prev);
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);

    AVLNode<Key, Value>* root_ = nullptr;

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    //inserts node into correct place and returns whether grandparent balance val needs to be changed
    //(balance has already been changed for the parent of the inserted node) and a pointer to the node
    //that was put into the tree
    std::pair<bool, AVLNode<Key, Value>*> insertInfo = this->insert_node(new_item);

    //if the insertInfo second was nullptr, then no further balancing is needed and function can terminate
    if (insertInfo.second == nullptr) {
        return;
    }

    //change variable tells next loop iteration whether the balance value needs to be changed. The parent balance has to be changed
    //when the child's balance value changes to 1 or -1.
    bool change = insertInfo.first;

    //keeps track of two previously visited nodes for rotations
    std::pair<AVLNode<Key, Value>*, AVLNode<Key, Value>*> prev = std::make_pair(insertInfo.second, nullptr);

    AVLNode<Key, Value>* curr = prev.first->getParent();

    while (curr->getParent() != nullptr) {
        prev.second = prev.first; prev.first = curr;
        curr = curr->getParent();
        
        if (change) {
            if (curr->getRight() == prev.first) {
                curr->updateBalance(1);
            }
            else if (curr->getLeft() == prev.first) {
                curr->updateBalance(-1);
            }
            else {
                std::cout << "this is not supposed to happen" << std::endl;
            }

            if (curr->getBalance() == 0) {
                return;
            }
            else if (curr->getBalance() == -1 || curr->getBalance() == 1) {
                continue;
            }
            //in this case a rotation needs to happen
            else {
                this->rotate(curr, prev);
                return;
            }
        }
        //if there is no change needed to balance values, then the tree is still an AVL tree
        else {
            return;
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
std::pair<bool, AVLNode<Key, Value>*> AVLTree<Key, Value>::insert_node(const std::pair<const Key, Value> &new_item) {
    AVLNode<Key, Value>* node = root_;
    if (node != nullptr) {
        while (node != nullptr) {
            bool left = false;
            if (new_item.first < node->getKey()) {
                left = true;
            } else if (new_item.first == node->getKey()) {
                node->setValue(new_item.second);
                return std::make_pair(false, nullptr);
            }
            if (left) {
                if (node->getLeft() == nullptr) {
                    node->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, node));
                    node->updateBalance(-1);
                    if (node->getRight() == nullptr) {
                        return std::make_pair(true, node->getLeft());
                    } else {
                        return std::make_pair(false, node->getLeft());
                    }
                }
                node = node->getLeft();
                continue;
            } else {
                if (node->getRight() == nullptr) {
                    node->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, node));
                    node->updateBalance(1);
                    if (node->getLeft() == nullptr) {
                        return std::make_pair(true, node->getRight());
                    } else {
                        return std::make_pair(false, node->getRight());
                    }
                        
                }
                node = node->getRight();
                continue;
            }
        }
    }
    else {
        root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, node);
        BinarySearchTree<Key, Value>::root_ = root_;
        return std::make_pair(false, nullptr);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotate(AVLNode<Key, Value>* node, std::pair<AVLNode<Key, Value>*, AVLNode<Key, Value>*> prev) {

    //left rotation
    if (node->getRight() == prev.first) {
        //Zig-zig
        if (prev.first->getRight() == prev.second) {
            this->rotateLeft(prev.first);
            prev.first->setBalance(0);
            node->setBalance(0);
        }
        //Zig-Zag
        else if (prev.first->getLeft() == prev.second) {
            this->rotateRight(prev.second);
            this->rotateLeft(prev.second);
            if (prev.second->getBalance() == 0) {
                node->setBalance(0);
                prev.first->setBalance(0);
            } else if (prev.second->getBalance() == 1) {
                prev.second->setBalance(0);
                prev.first->setBalance(-1);
                node->setBalance(0);
            }
            else {
                prev.second->setBalance(0);
                prev.first->setBalance(0);
                node->setBalance(1);
            }
        } else {
            std::cout <<"something went wrong in rotation" << std::endl;
        }
    }

    //right rotation
    else if (node->getLeft() == prev.first) {
        //zig-zig
        if (prev.first->getLeft() == prev.second) {
            this->rotateRight(prev.first);
            prev.first->setBalance(0);
            node->setBalance(0);
        }
        //Zig-Zag
        else if (prev.first->getRight() == prev.second) {
            this->rotateLeft(prev.second);
            this->rotateRight(prev.second);
            if (prev.second->getBalance() == 0) {
                node->setBalance(0);
                prev.first->setBalance(0);
            } else if (prev.second->getBalance() == 1) {
                prev.second->setBalance(0);
                prev.first->setBalance(-1);
                node->setBalance(0);
            }
            else {
                prev.second->setBalance(0);
                prev.first->setBalance(0);
                node->setBalance(1);
            }
        }
        else {
            std::cout <<"something went wrong in rotation" << std::endl;
        }
    }
}

//rotates child, parent, grandparent nodes left using the parent node
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* temp = n->getLeft();
    n->setLeft(n->getParent());
    n->getLeft()->setRight(temp);
    if (n->getLeft()->getParent() != nullptr) {
        if (n->getLeft()->getParent()->getLeft() == n->getLeft()) {
            n->getLeft()->getParent()->setLeft(n);
        } else {
            n->getLeft()->getParent()->setRight(n);
        }
    } else {
        root_ = n;
        BinarySearchTree<Key, Value>::root_ = n;
    }
    n->setParent(n->getLeft()->getParent());
    n->getLeft()->setParent(n);
}

//rotates child, parent, grandparent nodes right using the parent node
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* temp = n->getRight();
    n->setRight(n->getParent());
    n->getRight()->setLeft(temp);
    if (n->getRight()->getParent() != nullptr) {
        if (n->getRight()->getParent()->getLeft() == n->getRight()) {
            n->getRight()->getParent()->setLeft(n);
        } else {
            n->getRight()->getParent()->setRight(n);
        }
    } else {
        root_ = n;
        BinarySearchTree<Key, Value>::root_ = n;
    }
    n->setParent(n->getRight()->getParent());
    n->getRight()->setParent(n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::printBalance() {
    printNodeBalance(root_);
}

template<class Key, class Value>
void AVLTree<Key, Value>::printNodeBalance(AVLNode<Key, Value>* n) {
    if (n == nullptr) {
        return;
    }
    int balance = n->getBalance();
    std::cout << "Balance of key " << n->getKey() << ": " << balance << std::endl;
    printNodeBalance(n->getLeft());
    printNodeBalance(n->getRight());
}

#endif
