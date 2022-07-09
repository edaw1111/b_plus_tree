#include "Light_write_linked_list.h"

LWlinkedList::LWlinkedList()
{
    head = nullptr;
    tail = nullptr;
}

void LWlinkedList::insertAtTail(unsigned int time, BPnode *target)
{
    if (head == nullptr)
    {
        head = new LWnode(time, target);
        tail = head;
    }
    else
    {
        tail->next = new LWnode(time, target);
        tail = tail->next;
    }

    target->LW_num++;
    target->is_LW = true;
}

void LWlinkedList::updateTime(unsigned int time, BPnode *target)
{
    if (head == nullptr)
        return;

    LWnode *current = head, *prev = nullptr;

    //找到要更新的節點
    while (current->target != target)
    {
        prev = current;
        current = current->next;
    }

    //更新時間和紀錄light write次數
    current->time = time;
    target->LW_num++;

    //如果節點在最後，則不用移動直接返回函數
    if(current == tail)
        return;

    //移到最後面
    if(current == head){
        head = head->next;
    }
    else{
        prev->next = current->next;
    }
    tail->next = current;
    current->next = nullptr;
    tail = current;
}

//將retention time時間到的node清除掉
void LWlinkedList::check(unsigned int time)
{
    if (head == nullptr)
        return;

    LWnode *current = head, *deleteNode = nullptr;
    unsigned int deadline = time <= LWR ? 0 : time - LWR;
    while (current && current->time <= deadline)
    {
        current->target->NW_num++;
        current->target->is_LW = false;
        deleteNode = current;
        current = current->next;
        delete deleteNode;
    }
    head = current;

    //如果整個linked list空了，將tail清除
    if (head == nullptr)
        tail = nullptr;
}

//刪除被被刪除的node
void LWlinkedList::remove(BPnode *target)
{
    if (head == nullptr)
        return;

    if(head->target == target){
        //只有一個節點
        if(head == tail){
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else{
            LWnode *deleteNode = head;
            head = head->next;
            delete deleteNode;
        }
        return;
    }

    LWnode *current = head, *prev = nullptr;

    //找到要刪除的節點
    while (current->target != target)
    {
        prev = current;
        current = current->next;
    }

    //刪除節點
    prev->next = current->next;
    delete current;
}

//清空整個linked list
void LWlinkedList::removeAll()
{
    if (head == nullptr)
        return;

    LWnode *current = head, *deleteNode = nullptr;
    while (current)
    {
        current->target->NW_num++;
        current->target->is_LW = false;
        deleteNode = current;
        current = current->next;
        delete deleteNode;
    }
    tail = nullptr;
}