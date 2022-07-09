#ifndef B_PLUS_TREE_NODE_H 
#define B_PLUS_TREE_NODE_H

#include <bits/stdc++.h>
#include "../parameter/parameter.h"

class BPnode{
private:
    unsigned long long key[NS + 1];
    unsigned int nodeID;
    BPnode *ptr[NS + 2];
    BPnode *next;
    BPnode *parent;

    //metadate
    bool is_leaf;
    bool is_LW;
    unsigned int size;
    unsigned int createTime;
    unsigned int lastUpdateTime[RECORD_NUM];
    unsigned int LW_num;
    unsigned int NW_num;    

public:
    BPnode(unsigned int);
    void recordUpdateTime(unsigned int);
    void insertKey(unsigned long long, unsigned int, BPnode *);
    int  deleteKey(unsigned long long, unsigned int);
    

    friend class B_plus_tree;
    friend class LWlinkedList;
    friend class cache;
};

#endif