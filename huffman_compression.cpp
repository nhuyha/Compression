#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <bitset>
using namespace std;

//Tạo huffman node
struct huffman_node{
    int freq; //lưu tần suất
    char c; //lưu nhị phân 0 1
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
        p->c=leaf_1->c;
        p->left=leaf_1;
        p->right=leaf_2;
        p->freq=leaf_1->freq+leaf_2->freq;
        min_node.push(p);
        root=p;
    }
    return root;
}

// trái -> phải -> root
void postOrder(huffman_node* node) {
    if (node != nullptr) {
        postOrder(node->left);  
        postOrder(node->right);  
        std::cout << node->freq << " "<<node->c<<endl; 
    }
}

//Tạo map kí tự -> chuỗi nhị phân 0 1
map<char,string>hashMap;
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

// write byte to binary file
void writeBinaryFile(ofstream &outputFile, const string &bits){
    bitset<8> bitset(bits);
    unsigned char byte =static_cast<unsigned char>(bitset.to_ulong()); // Convert bitset to unsigned char (byte)
    outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte)); // Write byte to binary file
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

    // //create node
    // min_node.push(createNode(29,'e'));
    // min_node.push(createNode(30,'c'));
    // min_node.push(createNode(15,'b'));
    // min_node.push(createNode(10,'a'));
    // min_node.push(createNode(16,'d'));

    // // test create tree
    huffman_node*tree=createTree();
    // // postOrder(tree); 

    //tạo hashMap  
    createMap(tree,"");
    // map<char,string>::iterator it;
    // for (it=hashMap.begin();it!=hashMap.end();it++){
    //     cout<<it->first<<' '<<it->second<<endl;
    // }

    inputFile.close();


    //create output file
    int pos=input.find(".");
    string output=input.substr(0,pos)+"_compressed.bin";
    ofstream outputFile(output,ios::binary);
    ifstream inputFile1(input);
    if(!outputFile.is_open()){
        cout<<"Error create output file"<<endl;
        return 0;
    }

    // chuyển từ .txt -> .bin
    string bitBuffer="";
    while(inputFile1.get(c)){
        if(hashMap.find(c)!=hashMap.end()){
            string binaryString=hashMap[c];
            bitBuffer+=binaryString;

            while(bitBuffer.size()>=8){
                string byteString=bitBuffer.substr(0,8);
                writeBinaryFile(outputFile,byteString);
                bitBuffer.erase(0,8);
            }
        } else{
            cout<<c<<" not found in hashmap"<<endl;
            return 0;
        }
    }

    if(!bitBuffer.empty()){
        while(bitBuffer.size()<8){
            bitBuffer+='0';
        }
        writeBinaryFile(outputFile,bitBuffer);
    }


    inputFile1.close();
    outputFile.close();
    cout<<"Compression completed successfully!"<<endl;

    return 0; 
}