#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int mindepth(Node * root) {
    if (root == nullptr) {
        return 0;
    }
    int leftdepth = mindepth(root->left);
    int rightdepth = mindepth(root->right);

    if (leftdepth != 0 && leftdepth < rightdepth) {
        return leftdepth+1;
    } else if (rightdepth != 0){
        return rightdepth+1;
    } else {
        return leftdepth+1;
    }
}

int maxdepth(Node * root) {
    if (root == nullptr) {
        return 0;
    }
    int leftdepth = maxdepth(root->left);
    int rightdepth = maxdepth(root->right);

    if (leftdepth > rightdepth) {
        return leftdepth+1;
    } else {
        return rightdepth+1;
    }
}

bool equalPaths(Node * root)
{
    // Add your code below
    return mindepth(root) == maxdepth(root);

}

