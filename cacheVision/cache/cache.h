#ifndef CACHE_H 
#define CACHE_H

#include "../parameter/parameter.h"
#include "../b_plus_tree/B_plus_tree.h"


class cache{
private:
    unsigned long long key[(NS/CLS)][CLS];
    unsigned int tag[(NS/CLS)];
    BPnode *ptr[(NS/CLS)];
    B_plus_tree *tree;
    

public:
    void write(unsigned long long, unsigned int); 
    void writeBack();
    void replacement();
    
    

};

#endif