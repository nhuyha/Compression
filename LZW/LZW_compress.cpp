#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cmath>
#include <fstream>
#include "LZW_compress.h"
using namespace std;

string generateBinaryString(long long length, double p0, double p1) {
    string binaryString="";
    random_device rd;  // Seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine
    uniform_real_distribution<> dis(0.0, 1.0); // Distribution range [0.0, 1.0]

    for (long long i = 0; i < length; ++i) {
        double randomValue = dis(gen); // Generate random number between 0 and 1
        if (randomValue < p0) {
            binaryString += "0";
        } else {
            binaryString += "1";
        }
    }
    
    return binaryString;
}

vector <long long> encoding(string s,long long &totalBits){
    unordered_map<string,long long>table;
    table["0"]=0;
    table["1"]=1;
    // cout<<"Table: "<<endl;
    
    int code=2;
    vector <long long> output;
    string p="",c="";
    p+=s[0];
    // cout << "String\t\tOutput_Code\t\tAddition\n"; 
    for(int i=0; i<s.length();i++){
        if(i!=s.length()-1){
            c+=s[i+1];
        }
        if(table.find(p+c)!=table.end()){
            p=p+c;
        }else{
            // cout << p << "\t\t" << table[p] << "\t\t\t"<< p + c << "\t\t" << code << endl; 
            output.push_back(table[p]);
            totalBits+=ceil(log2(code));
            table[p+c]=code;
            code++;
            p=c;
        }
        c="";
    }
    output.push_back(table[p]);
    totalBits+=ceil(log2(code));
    return output;
}

string decoder(vector<long long>input){
    unordered_map<long long,string>table;
    table[0]="0";
    table[1]="1";
    // cout<<"Table: "<<endl;

    int code=2;
    string output="";
    int pre=input[0];
    string s=table[pre];  
    string c;
    c+=s[0];
    output+=s;

    for(int i=1; i<input.size();i++){
        int curr=input[i];
        
        if (table.find(curr)!=table.end()){
            s=table[curr];
        }
        else{
            s=table[pre];
            s=s+c;
        }

        // cout<<code<<" "<<s<<endl;
        output+=s;
        c=s[0];
        table[code]=table[pre]+c;
        code++;
        pre=curr;
    }

    return output;
}


