#ifndef LIGHT_WRITE_NODE_H 
#define LIGHT_WRITE_NODE_H

#include "B_plus_tree_node.h"

class LWnode{
private:
    unsigned int time;
    LWnode *next;
    BPnode *target;

public:
    LWnode(unsigned int, BPnode *);

    friend class LWlinkedList;
};

#endif