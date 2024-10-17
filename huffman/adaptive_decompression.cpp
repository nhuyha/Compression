#include <iostream>
#include <fstream>
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
        value(-1),weight(0),order(0),left(nullptr),right(nullptr),parent(nullptr)
        {}
};

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
    currNYT->value = 0;
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

string decoder(string word){
    map<char,Node*>LeafNodes;
    string codes="";
    Node*root = new Node(-1,0,MAX_VAL,nullptr,nullptr,nullptr);
    
    int i=0;
    while(i<word.length()){
        Node*curr = root;
        while(curr->left){
            if (word[i]=='0'){
                curr=curr->left;
            } else{
                curr=curr->right;
            }
            i++;
        }
        if (curr->value == -1){
            Node* currNYT = curr;
            string new_word=word.substr(i,BIT_SIZE);
            i+=BIT_SIZE;
            bitset<BIT_SIZE>bits(new_word);
            char c = char(bits.to_ulong());
            codes+=c;
            Node*newLeaf = new Node(c,1,0,nullptr,nullptr,currNYT);
            currNYT = AddNewLeaf(curr,newLeaf);
            LeafNodes.insert({c,newLeaf});
            
        } else{
            char c= (char) curr->value;
            codes+=c;
            Node*Leaf = LeafNodes[c];
            Node*maxOrder = HighestOrder(&LeafNodes,Leaf);
            maxOrder->weight++;
            update(maxOrder);
        }   
    }
    return codes;
}


int main(int argc, char*argv[]){
    if (argc!=2){
        cout<<"Invalid cmd"<<endl;
        return 0;
    }
    string input=argv[1];
    ifstream inputFile(input, ios::binary);
    if(!inputFile.is_open()){
        cout<<"Error opening input file"<<endl;
        return 0;
    }

    //đọc filetype
    string filetype="";
    char c;
    int filetype_size;
    inputFile.read((char*)(&filetype_size), sizeof(filetype_size)); 
    while(filetype_size--){
        inputFile.read(&c,1);
        filetype=filetype+c;
    }

    //tạo outputfile
    int pos=input.find('_');
    string output=input.substr(0,pos)+"_decompressed2"+'.'+filetype;
    ofstream outputFile(output);
  

    if(!outputFile.is_open()){
        cout<<"Error create output file"<<endl;
        return 0;
    }
    else {
        cout<<output<<endl;
    }

    //đọc pad
    int pad;
    inputFile.read((char*)(&pad), sizeof(pad));
    cout<<pad<<endl;

    string content="";
    unsigned char dec;
    while(inputFile.read(reinterpret_cast<char*>(&c), sizeof(c))){
        content+=bitset<8>(c).to_string();
    }
    content.erase(0,pad);

    outputFile<<decoder(content);

    inputFile.close();
    outputFile.close();
    cout<<"Decompression completed successfully!"<<endl;
    return 0;
}