#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset>
#include <math.h>
using namespace std;

//Tạo huffman node
struct huffman_node{
    int freq; //lưu tần suất
    char c; 
    huffman_node *left;
    huffman_node *right;
};

struct huffman_compression
{
    bool operator()(huffman_node*const&h1, huffman_node*const&h2 ){
        return h1->freq>h2->freq;
    }
};

//Tạo min_heap huffman node
priority_queue<huffman_node*,vector<huffman_node*>,huffman_compression> min_node;

//Node mới
huffman_node* createNode(int freq, char c){
    huffman_node* temp=new huffman_node();
    temp -> freq=freq;
    temp ->c = c;
    temp->left=nullptr;
    temp->right=nullptr;
    return temp;
}

//Giải thuật huffman code
huffman_node* createTree(){
    huffman_node*root= new huffman_node();
    while(min_node.size()>1){
        huffman_node*p=new huffman_node();
        huffman_node*leaf_1=min_node.top();
        min_node.pop();
        huffman_node*leaf_2=min_node.top();
        min_node.pop();
        // p->c=leaf_1->c;
        p->left=leaf_1;
        p->right=leaf_2;
        p->freq=leaf_1->freq+leaf_2->freq;
        min_node.push(p);
        root=p;
    }
    return root;
}

//Tạo map kí tự -> chuỗi nhị phân 0 1
unordered_map<char,string>hashMap;
void createMap(huffman_node*root, string str){
    if(!root->left&&!root->right){
        hashMap[root->c]=str;
    }
    else{
        string str_left=str+'0';
        string str_right=str+'1';
        createMap(root->left,str_left);
        createMap(root->right,str_right);
    }
}


int main(int argc, char * argv[]) {
    //check input, argv -> file
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
    

    //đọc file, tính tần suất kí tự
    int freq[256]={0};
    char c;
    while(inputFile.get(c)){
        freq[c]++;
    }
    for(int i=0; i<256;i++){
        if(freq[i]){
            min_node.push(createNode(freq[i],(char)i));
        }
    }


    huffman_node*tree=createTree();

    //tạo hashMap  
    createMap(tree,"");
    // unordered_map<char,string>::iterator it;
    // for (it=hashMap.begin();it!=hashMap.end();it++){
    //     cout<<it->first<<' '<<it->second<<endl;
    // }

    inputFile.close();


    //create output file
    int pos=input.find(".");
    string filetype=input.substr(pos+1);
    string output=input.substr(0,pos)+"_compressed.bin";
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

    //lưu hashmap
    unordered_map<char,string>::iterator it;
    for (it=hashMap.begin();it!=hashMap.end();it++){
        string codes="";
        codes+=it->first+it->second;

        outputFile.write(codes.c_str(),codes.length()+1);
       
    }
    
    //tính pad
    int pad=0;
    string bitBuffer="";
    while(inputFile1.get(c)){
        string binaryString=hashMap[c];
        bitBuffer+=binaryString;
    }
    if(bitBuffer.length()%8!=0){
        pad=8-bitBuffer.length()%8;
    }
    // cout<<"pad: "<<pad<<endl;
    for(int i=0; i<pad;i++){
        bitBuffer='0'+bitBuffer;
    }
    //lưu pad
    char null='\0';
    outputFile.write(&null,1);
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


    inputFile1.close();
    outputFile.close();
    cout<<"Compression completed successfully!"<<endl;

    return 0; 
}