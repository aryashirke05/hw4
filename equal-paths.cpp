#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <cstdlib>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkPathsExist(Node* root, int depth, int& leafDepth){
  if(root == nullptr){
    return true;
  }
  // Check if leaf
  if(root->left == nullptr && root->right == nullptr){
    if(leafDepth == -1){
      leafDepth = depth;
      return true;
    }
    return depth == leafDepth;
  }
  // Check both paths (trees)
  return checkPathsExist(root->left, depth + 1, leafDepth) &&
          checkPathsExist(root->right, depth + 1, leafDepth);
}

bool equalPaths(Node * root)
{
    // Add your code below
  int leafDepth = -1;
  return checkPathsExist(root, 0, leafDepth);
}

