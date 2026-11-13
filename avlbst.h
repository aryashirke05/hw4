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
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int height(AVLNode<Key, Value>* node) const;
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);
    void updateBalance(AVLNode<Key, Value>* node, int diff, bool inserting);
    AVLNode<Key, Value>* rebalance(AVLNode<Key, Value>* node);
    void rebalanceUp(AVLNode<Key, Value>* node);

};

// rotateLeft() helper method
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
  AVLNode<Key, Value>* rightChild = node->getRight();
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* middle = rightChild->getLeft();
  rightChild->setParent(parent);

  if(parent == NULL){
    this->root_ = rightChild;
  }
  else if(parent->getLeft() == node){
    parent->setLeft(rightChild);
  }
  else {
    parent->setRight(rightChild);
  }

  // Change the nodes
  rightChild->setLeft(node);
  node->setParent(rightChild);
  node->setRight(middle);

  if(middle != NULL){
    middle->setParent(node);
  }
  node->setBalance(height(node->getLeft()) - height(node->getRight()));
  rightChild->setBalance(height(rightChild->getLeft()) - height(rightChild->getRight()));
  return rightChild;
}

// rotateRight() helper method
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
  AVLNode<Key, Value>* leftChild = node->getLeft();
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* middle = leftChild->getRight();
  leftChild->setParent(parent);

  if(parent == NULL){
    this->root_ = leftChild;
  }
  else if(parent->getLeft() == node){
    parent->setLeft(leftChild);
  }
  else {
    parent->setRight(leftChild);
  }

  // Change the nodes
  leftChild->setRight(node);
  node->setParent(leftChild);
  node->setLeft(middle);

  if(middle != NULL){
    middle->setParent(node);
  }
  node->setBalance(height(node->getLeft()) - height(node->getRight()));
  leftChild->setBalance(height(leftChild->getLeft()) - height(leftChild->getRight()));
  return leftChild;
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

// Height of subtree
template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const
{
  if (node == NULL){
    return 0;
  }

  int leftHeight = height(node->getLeft());
  int rightHeight = height(node->getRight());

  if(leftHeight >rightHeight){
    return 1 + leftHeight;
  }
  else {
    return 1 + rightHeight;
  }
}
// rebalance() helper method
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node)
{
  if(node == NULL){
    return NULL;
  }
  int balance = height(node->getLeft()) - height(node->getRight());
  node->setBalance(balance);

  // Heavy on left side
  if(balance > 1){
    AVLNode<Key, Value>* leftChild = node->getLeft();
    if(height(leftChild->getLeft()) < height(leftChild->getRight())){
      rotateLeft(leftChild);
    }
    return rotateRight(node);
  }

  // Heavy on right side
  if(balance < -1){
    AVLNode<Key, Value>* rightChild = node->getRight();
    if(height(rightChild->getRight()) < height(rightChild->getLeft())){
      rotateRight(rightChild);
    }
    return rotateLeft(node);
  }
  return node;
}

// rebalanceUp() helper method
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceUp(AVLNode<Key, Value>* node){
  while(node != NULL){
    AVLNode<Key, Value>* newRoot = rebalance(node);
    node = newRoot->getParent();
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;

    while(current != NULL){
      parent = current;
      if(new_item.first == current->getKey()){
        current->setValue(new_item.second);
        return;
      }
      else if (new_item.first < current->getKey()){
        current = current->getLeft();
      }
      else{
        current = current->getRight();
      }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(new_item.first < parent->getKey()){
      parent->setLeft(newNode);
    }
    else {
      parent->setRight(newNode);
    }
    rebalanceUp(parent);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(target == NULL){
      return;
    }

    // Swap with predecessor if two children present.
    if (target->getLeft() != NULL && target->getRight() != NULL){
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(target));
      nodeSwap(target, pred);
    }

    // At least one child in target.
    AVLNode<Key, Value>* child = NULL;
    if(target->getLeft() != NULL){
      child = target->getLeft();
    }
    else {
      child = target->getRight();
    }
    AVLNode<Key, Value>* parent = target->getParent();

    if(child != NULL){
      child->setParent(parent);
    }
    if(parent == NULL){
      this->root_ = child;
    }
    else if(parent->getLeft() == target){
      parent->setLeft(child);
    }
    else{
      parent->setRight(child);
    }
    delete target;
    rebalanceUp(parent);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
