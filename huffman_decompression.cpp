#include<iostream>
#include <fstream>
#include<unordered_map>
#include <cstdint>
using namespace std;

string dec2bin(int dec){
    if (dec==0){
        return "00000000";
    }
    string bin="";
    while(dec>0){
        if(dec%2==0){
            bin="0"+bin;
        }
        else{
            bin="1"+bin;
        }
        dec=dec/2;
    }
    while (bin.length() < 8) {
        bin = "0" + bin;
    }
    return bin;
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
    string output=input.substr(0,pos)+"_decompressed"+'.'+filetype;
    ofstream outputFile(output);
  

    if(!outputFile.is_open()){
        cout<<"Error create output file"<<endl;
        return 0;
    }
    else {
        cout<<output<<endl;
    }

    //đọc hashmap
    int flag=0;
    string codes="";
    unordered_map<string,char>decode_map;
    while(inputFile.read(&c,1)){
        if (c!='\0'){
            flag=0;
            codes=codes+c;
        }
        else{
            flag++;
            if (flag==2) {
                break;
            }
            decode_map[codes.substr(1,codes.size()-1)]=codes[0];
            codes="";

        }
    }
    // unordered_map<string,char>::iterator it;
    // for(it=decode_map.begin();it!=decode_map.end();it++){
    //     cout<<it->first<<' '<<it->second<<endl;
        
    // }

    //đọc pad
    int pad;
    inputFile.read((char*)(&pad), sizeof(pad));
       
    //đọc char -> dec -> binary
    string content="";
    unsigned int dec;
    while(inputFile.read((char*)(&dec), sizeof(dec))){
        content+=dec2bin(dec); 
    }
    content.erase(0,pad);

    // binary -> hashmap -> ghi vào output
    for (int i=1; content.size()>0;i++){
        if(decode_map.find(content.substr(0,i))!=decode_map.end()){
            c=decode_map[content.substr(0,i)];
            outputFile<<c;
            content.erase(0,i);
            i=1;
        }
    }

    inputFile.close();
    outputFile.close();
    cout<<"Decompression completed successfully!"<<endl;
    return 0;

}