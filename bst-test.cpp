#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<int,int> bt;
    bt.insert(std::make_pair(50,1));
    bt.insert(std::make_pair(20,1));
    bt.insert(std::make_pair(30, 1));
    bt.insert(std::make_pair(10, 1));
    bt.insert(std::make_pair(40, 1));
    bt.insert(std::make_pair(60, 1));
    bt.insert(std::make_pair(55, 1));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<int,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first <<endl;
    }
    if(bt.find(40) != bt.end()) {
        cout << "Found 40" << endl;
    }
    else {
        cout << "Did not find 40" << endl;
    }
    cout << "Erasing 10" << endl;
    bt.remove(10);

    for(BinarySearchTree<int,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first <<endl;
    }

    /*// AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b'); */

    return 0;
}
