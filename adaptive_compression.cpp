#include <iostream>
#include <map>
#include <assert.h> 
#include <bitset>
#include <vector>
#include <sstream> 
#include <string>
#define MAX_VAL 512
#define BIT_SIZE 8

using namespace std;

struct Node
{
    int value;
    int weight;
    int order;
    Node* left;
    Node* right;
    Node* parent;

    Node(int v, int w, int o, Node*l, Node*r, Node*p):
        value(v),weight(w),order(o),left(l),right(r),parent(p)
        {}
    

    Node():
        value(0),weight(0),order(0),left(nullptr),right(nullptr),parent(nullptr)
        {}

};
void update(Node*leaf){
    leaf->weight++;
    Node*curr = leaf->parent;

    while(curr->parent){
        curr->weight++;
        if(curr->order == curr->parent->left->order){
            if(curr->weight > curr->parent->right->weight){
                int l = curr->order;
                int r = curr->parent->right->order;
                Node*temp;
                temp = curr->parent->right;
                curr->parent->right = curr;
                curr->parent->left = temp;
                curr->parent->right->order = r;
                curr->parent->left->order = l;
            }
        }
        curr = curr->parent;
    }
}
void NewNodeNYT(Node*currNYT, Node*newNYT, Node*newChar, char c){
    currNYT->left = newNYT;
    currNYT->value = 0;
    currNYT->right = newChar;
    newNYT->value = -1;
    newNYT->parent = currNYT;
    newNYT->order = currNYT->order - 2;
    newChar->value = c;
    newChar->parent = currNYT;
    newChar->order = currNYT->order - 1;
    update(newChar);
}

Node* makeHighestOrder(map<char, Node*>*leafNodes, Node*leaf){
    Node*max = leaf;
    for(auto&l:*leafNodes){
        if(l.second->weight == max->weight && l.second->order > max->order && leaf->parent->order != l.second->order){
            max = l.second;
        }
    }
    if(max->order != leaf->order){
        int temp = leaf->value;
        leaf->value = max->value;
        max->value = temp;
        leafNodes->find((char)max->value)->second = max;
        leafNodes->find((char)leaf->value)->second = leaf;
        return max;
    }
    return leaf;
}
void sendCode(Node*tree){
    Node*curr = tree;
    string code;
    while(curr->parent){
        if(curr->order == curr->parent->left->order){
            code = "0" + code;
        }else{
            code = "1" + code;
        }
        curr = curr->parent;
    }
    if(code == ""){
        code = "0";
    }
    cout << code;
}



string encoder(string word){
    map<char, Node*>leafNodes;
    Node*root = new Node(0,0,MAX_VAL,nullptr,nullptr,nullptr);
    Node*currNYT = root;
    string result;
    for(auto c:word){
        if(leafNodes.find(c) == leafNodes.end()){
            Node*newNYT = new Node(0,0,MAX_VAL-1,nullptr,nullptr,currNYT);
            Node*newChar = new Node(c,0,MAX_VAL-2,nullptr,nullptr,currNYT);
            leafNodes[c] = newChar;
            NewNodeNYT(currNYT,newNYT,newChar,c);
            currNYT = newNYT;
            Node*node = root;
            while(node->parent){
                node = makeHighestOrder(&leafNodes,node);
                sendCode(node);
                node = node->parent;
            }
        }else{
            Node*node = leafNodes[c];
            while(node->parent){
                node = makeHighestOrder(&leafNodes,node);
                sendCode(node);
                node = node->parent;
            }
        }
    }
    return result;
}

int main(){
    std::string word;
    std::stringstream ss;
    ss << std::cin.rdbuf();
    encoder( ss.str() ) ;
    std::cout << "\n";
    return 0;
}