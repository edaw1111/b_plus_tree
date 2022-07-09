#include "B_plus_tree_node.h"

BPnode::BPnode(unsigned int time)
{
    parent = nullptr;
    next = nullptr;

    //initialize metadata
    is_LW = false;
    createTime = time;
    for (int i = 1; i < RECORD_NUM; i++)
    {
        lastUpdateTime[i] = 0;
    }
    lastUpdateTime[0] = time;
    LW_num = 0;
    NW_num = 0;
}

// record update time
void BPnode::recordUpdateTime(unsigned int time)
{
    memcpy(&lastUpdateTime[1], &lastUpdateTime[0], sizeof(unsigned int) * (RECORD_NUM - 1));
    lastUpdateTime[0] = time;
}

//將key插入到node裡
void BPnode::insertKey(unsigned long long x, unsigned int time, BPnode *pointer)
{
    // find insert key place
    int i = 0;
    while (x > key[i] && i < size)
        i++;

    // insert the key
    memcpy(&key[i+1], &key[i], sizeof(unsigned long long) * (size - i));
    key[i] = x;

    // insert the pointer
    if(pointer != nullptr){
        memcpy(&ptr[i+2], &ptr[i+1], sizeof(BPnode *) * (size - i));
        ptr[i + 1] = pointer;
    }

    //更新metadata
    recordUpdateTime(time);
    size++;
}

//將key從node裡刪除
int BPnode::deleteKey(unsigned long long x, unsigned int time)
{
    // find delete key
    int i = 0;
    while (x > key[i] && i < size)
        i++;
    
    //if deleted key is not exist, return function
    if(x != key[i])
        return -1;

    // delete the key
    memcpy(&key[i], &key[i+1], sizeof(unsigned long long) * (size - i - 1));

    //更新metadata
    recordUpdateTime(time);
    size--;

    //回傳key的位置
    return i;
}