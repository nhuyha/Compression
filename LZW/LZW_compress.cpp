#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
using namespace std;

vector <long long> encoding(string s,unordered_map<string,long long>&table){
    int code=2;
    vector <long long> output;
    string p="",c="";
    p+=s[0];
    cout << "String\t\tOutput_Code\t\tAddition\n"; 
    for(int i=0; i<s.length();i++){
        if(i!=s.length()-1){
            c+=s[i+1];
        }
        if(table.find(p+c)!=table.end()){
            p=p+c;
        }else{
            std::cout << p << "\t\t" << table[p] << "\t\t\t"<< p + c << "\t\t" << code << std::endl; 
            output.push_back(table[p]);
            table[p+c]=code;
            code++;
            p=c;
        }
        c="";
    }
    output.push_back(table[p]);
    return output;
}

string decoder(vector<long long>input){
    unordered_map<long long,string>table;
    table[0]="0";
    table[1]="1";
    int code=2;
    string output="";
    int pre=input[0];
    string s=table[pre];
    string c=""+s[0];
    output+=s;
    for(int i=0; i<input.size()-1;i++){
        int curr=input[i+1];
        if (table.find(curr)!=table.end()){
            s=table[curr];
            
            
        }
        else{
            s=table[pre];
            s+=c;
        }
        output+=s;
        c=s[0];
        table[code]=table[pre]+c;
        code++;
        pre=curr;
    }

    return output;
}
int main() {
    string binaryString="01111111111000000000000101000000001000000000000000101001010100100";
    unordered_map<string,long long>table;
    table["0"]=0;
    table["1"]=1;cout<<"Table: "<<endl;
    vector<long long>output=encoding(binaryString,table);
    
    cout<<"Output: "<<endl;
    for(auto i:output){
        cout<<i<<" ";
    }
    cout<<decoder(output);
    return 0;
}
