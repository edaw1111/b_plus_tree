#ifndef LIGHT_WRITE_LINKED_LIST_H 
#define LIGHT_WRITE_LINKED_LIST_H

#include "Light_write_node.h"
#include "../parameter/parameter.h"


class LWlinkedList{
private:
    LWnode *head;
    LWnode *tail;

public:
    LWlinkedList();
    void insertAtTail(unsigned int, BPnode *);
    void updateTime(unsigned int, BPnode *);
    void remove(BPnode *);
    void check(unsigned int);
    void removeAll();
};

#endif