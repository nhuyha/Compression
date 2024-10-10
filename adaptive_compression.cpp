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

string encoder(char c,map<char,Node*>&LeafNodes,Node*&currNYT){
    string codes="";
    if(LeafNodes.find(c)==LeafNodes.end()){
            Node*NewLeaf=new Node(c,1,0,nullptr,nullptr,currNYT);
            codes=codes+sendCode(currNYT);
            bitset< BIT_SIZE > x(c);
            codes=codes+ x.to_string();    // print here
            currNYT=AddNewLeaf(currNYT,NewLeaf);
            LeafNodes.insert({c,NewLeaf});
        }
    else{
            Node*Leaf=LeafNodes[c];
            codes=codes+sendCode(Leaf);
            Node*maxOrder=HighestOrder(&LeafNodes,Leaf);
            maxOrder->weight++;
            update(maxOrder);
        }
    return codes;
}

int main(int argc, char * argv[]){
     if (argc!=2){
        cout<<"Invalid cmd"<<endl;
        return 0;
    }
    string input=argv[1];//input file.txt
    ifstream inputFile(input);
    if(!inputFile.is_open()){
        cout<<"Error opening input file"<<endl;
        return 0;
    } 

    //create output file
    int pos=input.find(".");
    string filetype=input.substr(pos+1);
    string output=input.substr(0,pos)+"_compressed2.bin";
    ofstream outputFile(output,ios::binary);  
    ifstream inputFile1(input);
    if(!outputFile.is_open()){
        cout<<"Error create output file"<<endl;
        return 0;
    } else{
        cout<<output<<endl;
    }

    //lưu file type vào output
    int filetype_size=filetype.length();
    outputFile.write((char*)(&filetype_size),sizeof(filetype_size));
    outputFile.write(filetype.c_str(),filetype_size);

    string bitBuffer="";
    char c;
    map<char,Node*>LeafNodes;
    Node*root=new Node(-1,0,MAX_VAL,nullptr,nullptr,nullptr);
    Node*currNYT=root;
    
    while(inputFile.get(c)){
        bitBuffer+=encoder(c,LeafNodes,currNYT);
    }
    // cout<<bitBuffer<<endl;


    //tính pad
    int pad=0;
    if(bitBuffer.length()%8!=0){
        pad=8-bitBuffer.length()%8;
    }
    cout<<"pad: "<<pad<<endl;
    for(int i=0; i<pad;i++){
        bitBuffer='0'+bitBuffer;
    }

    //lưu pad
    outputFile.write((char*)&pad, sizeof(pad));

        //chuyển binary -> dec -> char + lưu output file
unsigned char byte = 0; // Use unsigned char to store each byte
int bitCount = 0; // Count of bits collected

for (size_t i = 0; i < bitBuffer.size(); i++) {
    // Shift bit into the byte
    byte = (byte << 1) | (bitBuffer[i] - '0'); // Shift left and add current bit
    bitCount++;

    // If we've collected 8 bits, write the byte to the output file
    if (bitCount == 8) {
        // cout<<byte;
        outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
        byte = 0; // Reset the byte for the next 8 bits
        bitCount = 0; // Reset the bit count
    }
}

// If there are any leftover bits that didn't make a full byte
if (bitCount > 0) {
    byte <<= (8 - bitCount); // Shift left to fill the remaining bits
    outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte)); // Write the last byte
}

    inputFile.close();
    outputFile.close();
    cout<<"Compression completed successfully!"<<endl;
    return 0;
}