#include <iostream>
#include <map>
#include <string>
#include <bitset>
using namespace std;

#define MAX_VAL 512
#define BIT_SIZE 8

struct Node{
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


string sendCode(Node* Leaf){
    string code="";
    Node* curr=Leaf;
    while(curr->parent){
        if(curr->order==curr->parent->left->order){
            code="0"+code;
        }
        else{
            code="1"+code;
        }
        curr=curr->parent;
    }
    // if (code==""){
    //     code="0";
    // }
    return code;
}

void update(Node*leaf){
    Node*curr=leaf->parent;
    while(curr->parent){
        curr->weight++;
        Node*parent=curr->parent;
        if(curr->order == parent->left->order){
            if(curr->weight > parent->right->weight){
                int l = curr->order;
                int r = parent->right->order;
                Node*temp;
                temp = parent->right;
                parent->right = curr;
                parent->left = temp;
                parent->right->order = r;
                parent->left->order = l;
            }
        }
        curr = curr->parent;
    }
}

Node* AddNewLeaf(Node*currNYT,Node*NewLeaf){
    Node*newNYT = new Node();
    int curr_order = currNYT->order;
    currNYT->left = newNYT;
    currNYT->right = NewLeaf;
    newNYT->parent = currNYT;
    newNYT->order = curr_order-2;
    NewLeaf->order = curr_order-1;
    update(NewLeaf);
    return newNYT;
}

Node* HighestOrder(map<char,Node*> *LeafNodes,Node*Leaf){
    Node*max=Leaf;
    for(auto&l : *LeafNodes){
        if(l.second->weight == max->weight && l.second->order > max->order ){
            max=l.second;
        }
    }
    if(max->order != Leaf->order){
        int temp=Leaf->value;
        Leaf->value=max->value;
        max->value=temp;
        (*LeafNodes)[(char)max->value]=max;
        (*LeafNodes)[(char)Leaf->value]=Leaf;
        return max;
    }
    return Leaf;
}

void encoder(string word){
    map<char,Node*>LeafNodes;
    Node*root=new Node(0,0,MAX_VAL,nullptr,nullptr,nullptr);
    Node*currNYT=root;
    for(auto c:word){
        if(LeafNodes.find(c)==LeafNodes.end()){
            Node*NewLeaf=new Node(c,1,0,nullptr,nullptr,currNYT);
            cout<<sendCode(currNYT);
            bitset< BIT_SIZE > x(c);
            cout << x;  // print here
            currNYT=AddNewLeaf(currNYT,NewLeaf);
            LeafNodes.insert({c,NewLeaf});
        }
        else{
            Node*Leaf=LeafNodes[c];
            cout<<sendCode(Leaf);
            Node*maxOrder=HighestOrder(&LeafNodes,Leaf);
            maxOrder->weight++;
            update(maxOrder);
        }
    }
}

int main(){
    string word;
    cin>>word;
    encoder(word);
    return 0;
}