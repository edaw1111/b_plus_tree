#include "b_plus_tree/B_plus_tree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

B_plus_tree *node = new B_plus_tree;

void test_case_1()
{
    node->insertKey(238, 1);
    node->insertKey(7, 2);
    node->insertKey(436, 3);
    node->insertKey(719, 4);
    node->insertKey(629, 5);
    node->insertKey(237, 6);
    node->insertKey(975, 7);
    node->insertKey(949, 8);
    node->insertKey(492, 9);
    node->insertKey(887, 10);
    node->insertKey(263, 11);
    node->insertKey(157, 12);
    node->insertKey(374, 13);

    node->deleteKey(238, 14);
    node->deleteKey(374, 15);
    node->deleteKey(263, 16);
    node->deleteKey(157, 17);
    node->deleteKey(7, 18);
    node->deleteKey(719, 19);
    node->deleteKey(949, 20);
    node->deleteKey(975, 21);

    node->outputAllMetadata();
    node->outputAllNode();
}

void test_case_2()
{
    for(int i = 1; i <= 10; i++){
        node->insertKey(i, i);
    }
    node->outputAllMetadata();
    node->outputAllNode();
    
}

vector<string> trace_line_split(const string &str, const char &delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string tok;

    while (getline(ss, tok, delimiter)) {
        result.push_back(tok);
    }
    return result;
}

int read_requirement(string trace_file_name, vector<vector<string>> &all_requirement)
{
    ifstream  trace_file;
    string trace_line;
    vector<string> temp;

    trace_file.open(trace_file_name, ios::in);

    if(!trace_file.is_open()){
        cout << "File is not exist" << endl;
        return 1;
    }
    
    while(getline(trace_file, trace_line)){
        temp = trace_line_split(trace_line, ' ');
        all_requirement.push_back(temp);
    }

    trace_file.close();

    return 0;
}

void execute(B_plus_tree *node, vector<vector<string>> &all_requirement)
{
    int all_requirement_number = all_requirement.size(), operation_time, insertNum = 0, deleteNum = 0;
    unsigned long long key_number;
    string operation;

    for(int i = 0; i < all_requirement_number; i++){
        char *pEnd = nullptr;
        operation_time = stoi(all_requirement[i][0]);
        operation = all_requirement[i][1];
        key_number = strtoull(all_requirement[i][2].c_str(), &pEnd, 16);
        //cout << operation_time << " " << operation << " " << key_number << endl;
        if(operation.compare("insert") == 0)
        {
            node->insertKey(key_number, operation_time);
            insertNum++;
        }
        else{
            node->deleteKey(key_number, operation_time);
            deleteNum++;
        }
        //break;
    }
    cout << "insert: " << insertNum << ", delete: " << deleteNum << endl;
}

void start_simulate()
{
    vector<vector<string>> all_requirement;

    //read file
    cout << "start read file... " << endl;

    if(read_requirement("..\\..\\trace/IBMObject(InsertDelete)\\IBMObjectStoreTrace010Part0", all_requirement))
        return;

    cout << "finish" << endl;
    cout << endl;


    //execute requirement
    cout << "start execute... " << endl;

    execute(node, all_requirement);

    cout << "finish" << endl;
    cout << endl;


    //output record
    cout << "start output... " << endl;
    node->outputAllMetadata();
    node->outputAllNode();
    cout << "finish" << endl;
}

int main()
{
    ofstream {".\\out\\deleteNode.csv"};
    ofstream {".\\out\\node.csv"};
    ofstream {".\\out\\upateTime.csv"};

    //test_case_1();
    //test_case_2();
    //start_simulate();


    return 0;
}