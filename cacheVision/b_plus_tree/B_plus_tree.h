#ifndef B_PLUS_TREE_H 
#define B_PLUS_TREE_H

#include "B_plus_tree_node.h"
#include "../light_write/Light_write_linked_list.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class B_plus_tree{
private:
    BPnode *root;
    LWlinkedList *LWlist;
    unsigned int currentTime;

    //search related
    BPnode *findLeafNode(unsigned long long);

    //insert related
    void spiltLeafNode(BPnode *);
    void insertNonLeafNode(unsigned long long, BPnode *, BPnode *);
    void spiltNonLeafNode(BPnode *);

    //delete related
    void replaceKey(unsigned long long, unsigned long long, BPnode *);
    vector<BPnode *> findSiblingNode(unsigned long long, BPnode *, int &);
    int  balanceLeafNode(BPnode *);
    void brrowFromRightLeafNode(BPnode *, BPnode *, int);
    void brrowFromLeftLeafNode(BPnode *, BPnode *, int);
    void mergeLeafNode(BPnode *, BPnode *, int, int);
    void balanceNonLeafNode(BPnode *);
    void brrowFromRightNonLeafNode(BPnode *, BPnode *, int);
    void brrowFromLeftNonLeafNode(BPnode *, BPnode *, int);
    void mergeNonLeafNode(BPnode *, BPnode *, int, int);


    //output releated
    void _outputAllMetadata(BPnode * , std::ofstream &);
    void _outputAllNode(BPnode *, std::ofstream &);
    void outputdeleteNodeMetadata(BPnode *);

public:
    B_plus_tree();
    
    void search(unsigned long long);
    void insertKey(unsigned long long, unsigned int);
    void deleteKey(unsigned long long, unsigned int);
    void display();
    void display(BPnode *);
    void outputAllMetadata();
    void outputAllNode();


    friend class cache;
};
#endif