#include "B_plus_tree.h"

// initialization
B_plus_tree::B_plus_tree()
{
    root = NULL;
    LWlist = new LWlinkedList();
    currentTime = 0;
}

// search
BPnode *B_plus_tree::findLeafNode(unsigned long long x)
{
    BPnode *current = root;
    while (current->is_leaf == false)
    {
        for (int i = 0; i < current->size; i++)
        {
            if (x < current->key[i])
            {
                current = current->ptr[i];
                break;
            }
            if (i == current->size - 1)
            {
                current = current->ptr[i + 1];
                break;
            }
        }
    }
    return current;
}

void B_plus_tree::search(unsigned long long x)
{
    if (root == NULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }

    BPnode *current = findLeafNode(x);
    for (int i = 0; i < current->size; i++)
    {
        if (current->key[i] == x)
        {
            cout << "Found" << endl;
            return;
        }
    }
    cout << "Not found" << endl;
}

// insert
void B_plus_tree::spiltNonLeafNode(BPnode *oldNode)
{
    //分配新的節點
    BPnode *newNode = new BPnode(currentTime);

    //設定成non leaf node
    newNode->is_leaf = false;

    //計算分裂後新節點和舊節點key的數量
    oldNode->size = (NS + 1) / 2;
    newNode->size = NS - (NS + 1) / 2; //這裡比leaf少一是因為有一個要插到上面的internal node

    //紀錄寫入次數
    newNode->NW_num++;

    //將key搬到new node
    for (int i = 0, j = oldNode->size + 1; i < newNode->size; i++, j++)
    {
        newNode->key[i] = oldNode->key[j];
    }
    //將ptr搬到new node 和 更新parent
    for (int i = 0, j = oldNode->size + 1; i < newNode->size + 1; i++, j++)
    {
        newNode->ptr[i] = oldNode->ptr[j];
        newNode->ptr[i]->parent = newNode;
    }

    //複製parant指標
    newNode->parent = oldNode->parent;

    //將key插入到上一層
    if (oldNode == root)
    {
        BPnode *newRoot = new BPnode(currentTime);
        newRoot->key[0] = oldNode->key[oldNode->size];
        newRoot->ptr[0] = oldNode;
        newRoot->ptr[1] = newNode;

        // metadata
        oldNode->parent = newRoot;
        newNode->parent = newRoot;
        newRoot->is_leaf = false;
        newRoot->size = 1;

        //換新root
        root = newRoot;

        //紀錄寫入次數
        root->NW_num++;
    }
    else
    {
        insertNonLeafNode(oldNode->key[oldNode->size], newNode->parent, newNode);
    }
}

void B_plus_tree::insertNonLeafNode(unsigned long long x, BPnode *current, BPnode *child)
{
    //紀錄寫入次數
    current->NW_num++;

    //插入key
    current->insertKey(x, currentTime, child);

    //如果超過節點上限就分裂
    if (current->size > NS)
        spiltNonLeafNode(current); //分裂
}

void B_plus_tree::spiltLeafNode(BPnode *oldNode)
{
    //分配新的節點
    BPnode *newNode = new BPnode(currentTime);

    //設定成leaf node
    newNode->is_leaf = true;

    //計算分裂後新節點和舊節點key的數量
    oldNode->size = (NS + 1) / 2;
    newNode->size = NS + 1 - (NS + 1) / 2;

    //紀錄寫入次數
    newNode->NW_num++;

    //將key搬到new node
    for (int i = 0, j = oldNode->size; i < newNode->size; i++, j++)
    {
        newNode->key[i] = oldNode->key[j];
    }

    //更新next指標
    newNode->next = oldNode->next;
    oldNode->next = newNode;

    //複製parant指標
    newNode->parent = oldNode->parent;

    //將key插入到internal node
    if (oldNode == root)
    {
        BPnode *newRoot = new BPnode(currentTime);

        //紀錄寫入次數
        newRoot->NW_num++;

        newRoot->key[0] = newNode->key[0];
        newRoot->ptr[0] = oldNode;
        newRoot->ptr[1] = newNode;

        // metadata
        oldNode->parent = newRoot;
        newNode->parent = newRoot;
        newRoot->is_leaf = false;
        newRoot->size = 1;

        //換新root
        root = newRoot;
    }
    else
    {
        insertNonLeafNode(newNode->key[0], newNode->parent, newNode);
    }
}

void B_plus_tree::insertKey(unsigned long long x, unsigned int time)
{
    currentTime = time;

    if (root == NULL)
    {
        root = new BPnode(currentTime);
        root->key[0] = x;
        root->is_leaf = true;
        root->size = 1;
        root->NW_num++; //紀錄寫入次數
        return;
    }

    //插入key
    BPnode *current = findLeafNode(x);
    current->insertKey(x, currentTime, nullptr);

    //紀錄寫入次數
    current->NW_num++;

    //如果key的數量超過上限就分裂
    if (current->size > NS)
        spiltLeafNode(current); //分裂
}

// delete
vector<BPnode *> B_plus_tree::findSiblingNode(unsigned long long x, BPnode *current, int &pos)
{
    BPnode *leftNode, *rightNode;
    pos = 0;
    while (x >= current->key[pos] && pos < current->size)
        pos++;

    if (pos == 0)
    {
        leftNode = nullptr;
        rightNode = current->ptr[1];
    }
    else if (pos == current->size)
    {
        leftNode = current->ptr[pos - 1];
        rightNode = nullptr;
    }
    else
    {
        leftNode = current->ptr[pos - 1];
        rightNode = current->ptr[pos + 1];
    }

    return {leftNode, rightNode};
}

void B_plus_tree::mergeNonLeafNode(BPnode *leftNode, BPnode *rightNode, int pos, int whichIsCurrent)
{
    //紀錄寫入次數和紀錄更新時間
    if (whichIsCurrent)
    {   //右邊是current
        leftNode->NW_num++;
        leftNode->recordUpdateTime(currentTime);
    }
    leftNode->parent->NW_num++;
    leftNode->parent->recordUpdateTime(currentTime);

    //先從parent node借一個key下來
    leftNode->key[leftNode->size] = leftNode->parent->key[pos];
    leftNode->size++;

    //將right node的ptr的parent改成left node
    for (int i = 0; i < rightNode->size + 1; i++)
    {
        rightNode->ptr[i]->parent = leftNode;
    }

    //將右邊節點的key移到左邊的節點
    memcpy(&leftNode->key[leftNode->size], &rightNode->key[0], sizeof(unsigned long long) * (rightNode->size));
    //將右邊節點的ptr移到左邊的節點
    memcpy(&leftNode->ptr[leftNode->size], &rightNode->ptr[0], sizeof(BPnode *) * (rightNode->size + 1));
    leftNode->size += rightNode->size;

    //紀錄右邊節點的資訊
    outputdeleteNodeMetadata(rightNode);

    //刪除右邊節點
    delete rightNode;

    //將父節點的key和ptr刪除
    memcpy(&leftNode->parent->key[pos], &leftNode->parent->key[pos + 1], sizeof(unsigned long long) * (leftNode->parent->size - pos - 1));
    memcpy(&leftNode->parent->ptr[pos + 1], &leftNode->parent->ptr[pos + 2], sizeof(BPnode *) * (leftNode->parent->size - pos));
    leftNode->parent->size--;

    //如果父節點key的數量低於數量下限，就需要平衡tree
    if (leftNode->parent->size < NS / 2)
        balanceNonLeafNode(leftNode->parent);
}

void B_plus_tree::brrowFromRightNonLeafNode(BPnode *current, BPnode *rightNode, int pos)
{
    //紀錄寫入次數
    rightNode->NW_num++;
    current->parent->NW_num++;

    //紀錄跟新時間
    rightNode->recordUpdateTime(currentTime);
    current->parent->recordUpdateTime(currentTime);

    //從parent node借一個key下來
    current->key[current->size] = current->parent->key[pos];
    //將right node的第一個ptr移到current node
    current->ptr[current->size + 1] = rightNode->ptr[0];
    //並將移過來的ptr的parent改為current node
    current->ptr[current->size + 1]->parent = current;
    //增加current node的size個數
    current->size++;
    //將right node的第一個key移到parent node
    current->parent->key[pos] = rightNode->key[0];
    //將right node的第一個key和ptr刪除
    memcpy(&rightNode->key[0], &rightNode->key[1], sizeof(unsigned long long) * (rightNode->size - 1));
    memcpy(&rightNode->ptr[0], &rightNode->ptr[1], sizeof(BPnode *) * (rightNode->size));
    rightNode->size--;
}

void B_plus_tree::brrowFromLeftNonLeafNode(BPnode *current, BPnode *leftNode, int pos)
{
    //紀錄寫入次數
    leftNode->NW_num++;
    current->parent->NW_num++;

    //紀錄跟新時間
    leftNode->recordUpdateTime(currentTime);
    current->parent->recordUpdateTime(currentTime);

    // shift right by one in current so that first position becomes free
    memcpy(&current->key[1], &current->key[0], sizeof(unsigned long long) * (current->size));
    memcpy(&current->ptr[1], &current->ptr[0], sizeof(BPnode *) * (current->size + 1));
    //從parent node借一個key下來
    current->key[0] = current->parent->key[pos - 1];
    //將left node的第最後一個ptr移到current node的第一個ptr
    current->ptr[0] = leftNode->ptr[leftNode->size];
    //並將移過來的ptr的parent改為current node
    current->ptr[0]->parent = current;

    current->size++;

    //將left node的第最後一個key移到parent node
    current->parent->key[pos - 1] = leftNode->key[leftNode->size - 1];

    leftNode->size--;
}

void B_plus_tree::balanceNonLeafNode(BPnode *current)
{
    //如果root已經沒有key了，下降一層
    if (current == root && current->size == 0)
    {
        root = current->ptr[0];
        root->parent = nullptr;
        outputdeleteNodeMetadata(current); //紀錄節點的資訊
        delete current;
        return;
    }

    int pos;
    vector<BPnode *> sibling = findSiblingNode(current->key[0], current->parent, pos);
    BPnode *leftNode = sibling.at(0);
    BPnode *rightNode = sibling.at(1);

    if (leftNode != nullptr && leftNode->size > NS / 2)
        //向右邊的node借一個key
        brrowFromLeftNonLeafNode(current, leftNode, pos);
    else if (rightNode != nullptr && rightNode->size > NS / 2)
        //向左邊的node借一個key
        brrowFromRightNonLeafNode(current, rightNode, pos);
    else
    { //如果左右兩邊的節點都不夠就合併
        if (leftNode != nullptr)
            mergeNonLeafNode(leftNode, current, pos - 1, 1);
        else
            mergeNonLeafNode(current, rightNode, pos, 0);
    }
}

void B_plus_tree::mergeLeafNode(BPnode *leftNode, BPnode *rightNode, int pos, int whichIsCurrent)
{
    //紀錄寫入次數和紀錄更新時間
    if (whichIsCurrent)
    {   //右邊是current
        leftNode->NW_num++;
        leftNode->recordUpdateTime(currentTime);
    }
    leftNode->parent->NW_num++;
    leftNode->parent->recordUpdateTime(currentTime);

    //將右邊節點的key移到左邊的節點
    memcpy(&leftNode->key[leftNode->size], &rightNode->key[0], sizeof(unsigned long long) * (rightNode->size));
    leftNode->size += rightNode->size;

    //紀錄右邊節點的資訊
    outputdeleteNodeMetadata(rightNode);

    //刪除右邊的節點
    delete rightNode;

    //將父節點的key和ptr刪除
    memcpy(&leftNode->parent->key[pos], &leftNode->parent->key[pos + 1], sizeof(unsigned long long) * (leftNode->parent->size - pos - 1));
    memcpy(&leftNode->parent->ptr[pos + 1], &leftNode->parent->ptr[pos + 2], sizeof(BPnode *) * (leftNode->parent->size - pos));
    leftNode->parent->size--;

    //如果父節點key的數量低於數量下限，就需要平衡tree
    if (leftNode->parent->size < NS / 2)
        balanceNonLeafNode(leftNode->parent);
}

void B_plus_tree::brrowFromRightLeafNode(BPnode *current, BPnode *rightNode, int pos)
{
    //紀錄寫入次數
    rightNode->NW_num++;
    current->parent->NW_num++;

    //紀錄更新時間
    rightNode->recordUpdateTime(currentTime);
    current->parent->recordUpdateTime(currentTime);

    // borrow the first key of rightNode to the last position of current node
    current->key[current->size] = rightNode->key[0];
    current->size++;
    // shift by one key to left of the rightNode
    memcpy(&rightNode->key[0], &rightNode->key[1], sizeof(unsigned long long) * (rightNode->size - 1));
    // decrease number of keys by one
    rightNode->size--;

    current->parent->key[pos] = rightNode->key[0];
}

void B_plus_tree::brrowFromLeftLeafNode(BPnode *current, BPnode *leftNode, int pos)
{
    //紀錄寫入次數
    leftNode->NW_num++;
    current->parent->NW_num++;

    //紀錄更新時間
    leftNode->recordUpdateTime(currentTime);
    current->parent->recordUpdateTime(currentTime);

    // shift by one key to right of the current node so that we can free the first position
    memcpy(&current->key[1], &current->key[0], sizeof(unsigned long long) * (current->size));
    // borrow the last key of leftNode to the first position of current node
    current->key[0] = leftNode->key[leftNode->size - 1];
    // increase number of nodes by one
    current->size++;
    leftNode->size--;

    current->parent->key[pos - 1] = current->key[0];
}

int B_plus_tree::balanceLeafNode(BPnode *current)
{
    // if the root is the only leaf
    if (current == root)
    {
        if (current->size == 0)
        {
            //紀錄根節點的資訊
            outputdeleteNodeMetadata(root);
            //刪除root
            delete root;
            root = nullptr;
        }
        return 0;
    }

    //找到左右邊的節點
    int pos;
    vector<BPnode *> sibling = findSiblingNode(current->key[0], current->parent, pos);
    BPnode *leftNode = sibling.at(0);
    BPnode *rightNode = sibling.at(1);

    if (leftNode != nullptr && leftNode->size > NS / 2)
    { //向左邊的node借一個key
        brrowFromLeftLeafNode(current, leftNode, pos);
        //如果刪除的key在最左邊，需要向上檢查，並更改key值
        return 1;
    }
    else if (rightNode != nullptr && rightNode->size > NS / 2)
    { //向右邊的node借一個key
        brrowFromRightLeafNode(current, rightNode, pos);
        //如果刪除的key在最左邊，不需要向上檢查
        return 0;
    }
    else
    { //如果左右兩邊的節點都不夠就合併
        if (leftNode != nullptr)
        {
            mergeLeafNode(leftNode, current, pos - 1, 1);
            //如果刪除的key在最左邊，不需要向上檢查
            return 0;
        }
        else
        {
            mergeLeafNode(current, rightNode, pos, 0);
            //如果刪除的key在最左邊，需要向上檢查，並更改key值
            return 1;
        }
    }
}

void B_plus_tree::replaceKey(unsigned long long oldKey, unsigned long long newKey, BPnode *current)
{
    bool found = false;
    while (current != NULL && found == false)
    {
        for (int i = 0; i < current->size; i++)
        {
            if (current->key[i] == oldKey)
            {
                //紀錄寫入次數
                current->NW_num++;
                //紀錄跟新時間
                current->recordUpdateTime(currentTime);
                //更改key
                current->key[i] = newKey;
                found = true;
                break;
            }
        }
        current = current->parent;
    }
}

void B_plus_tree::deleteKey(unsigned long long x, unsigned int time)
{
    if (root == NULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }

    currentTime = time;

    //找到key所在的node
    BPnode *current = findLeafNode(x);

    //紀錄寫入次數
    current->NW_num++;

    //刪除key
    int deletePosition = current->deleteKey(x, time);

    int replace = 1;
    //如果key的數量低於數量下限，就需要平衡tree
    if (current->size < NS / 2)
        replace = balanceLeafNode(current);

    //如果刪除的key在最左邊，需要將上層一樣的key做更改
    if (deletePosition == 0 && replace == 1)
    {
        replaceKey(x, current->key[0], current->parent);
    }
}

//印出整棵樹
void B_plus_tree::display()
{
    if (root == NULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }

    // cout << "root: ";
    display(root);
}

void B_plus_tree::display(BPnode *current)
{
    if (current == NULL)
        return;

    if (current->is_leaf == true)
        cout << "Leaf: ";

    //印出node裡面所有的key
    for (int i = 0; i < current->size; i++)
    {
        cout << current->key[i] << " ";
    }
    cout << endl;

    //如果是leaf node就返回，否則繼續往下執行
    if (current->is_leaf == true)
        return;

    //印出子節點
    for (int i = 0; i < current->size + 1; i++)
    {
        display(current->ptr[i]);
    }
}

// output
void B_plus_tree::outputAllMetadata()
{
    if (root == NULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }

    ofstream outFile;
    outFile.open(".\\out\\upateTime.csv", ios::out);

    outFile << "create_time,";
    for (int i = 1; i <= RECORD_NUM; i++)
    {
        outFile << "update_time_" + to_string(i) + ",";
    }
    outFile << "is_leaf,is_LW,LW_NUM,NW_NUM" << endl;

    _outputAllMetadata(root, outFile);

    outFile.close();
}

void B_plus_tree::_outputAllMetadata(BPnode *current, ofstream &outFile)
{
    //印出節點所有的更新時間和創建時間
    outFile << current->createTime << ",";
    for (int i = 0; i < RECORD_NUM; i++)
    {
        outFile << current->lastUpdateTime[i] << ",";
    }
    outFile << current->is_leaf << "," << current->is_LW << "," << current->LW_num << "," << current->NW_num << endl;

    //如果是leaf node就返回，否則繼續往下執行
    if (current->is_leaf == true)
        return;

    //印出子節點
    for (int i = 0; i < current->size + 1; i++)
    {
        _outputAllMetadata(current->ptr[i], outFile);
    }
}

void B_plus_tree::outputdeleteNodeMetadata(BPnode *current)
{
    if (current == NULL)
        return;

    ofstream outFile;
    outFile.open(".\\out\\deleteNode.csv", ios::app);

    //印出節點所有的更新時間和創建時間
    outFile << current->createTime << ",";
    for (int i = 0; i < RECORD_NUM; i++)
    {
        outFile << current->lastUpdateTime[i] << ",";
    }
    outFile << current->is_leaf << "," << current->is_LW << "," << current->LW_num << "," << current->NW_num << endl;

    outFile.close();
}

void B_plus_tree::outputAllNode()
{
    if (root == NULL)
        return;

    ofstream outFile;
    outFile.open(".\\out\\node.csv", ios::out);

    _outputAllNode(root, outFile);

    outFile.close();
}

void B_plus_tree::_outputAllNode(BPnode *current, ofstream &outFile)
{
    //印出node裡面所有的key
    for (int i = 0; i < current->size; i++)
    {
        outFile << current->key[i] << ",";
    }
    outFile << endl;

    //如果是leaf node就返回，否則繼續往下執行
    if (current->is_leaf == true)
        return;

    //印出子節點
    for (int i = 0; i < current->size + 1; i++)
    {
        _outputAllNode(current->ptr[i], outFile);
    }
}
