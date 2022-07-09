#include "Light_write_node.h"

LWnode::LWnode(unsigned int _time, BPnode *_target){
    time  = _time;
    target = _target;
    next = nullptr;
}