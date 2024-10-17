#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>

using namespace std;

string generateBinaryString(long long length, double p0, double p1) {
    string binaryString;
    random_device rd;  // Seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine
    uniform_real_distribution<> dis(0.0, 1.0); // Distribution range [0.0, 1.0]

    for (long long i = 0; i < length; ++i) {
        double randomValue = dis(gen); // Generate random number between 0 and 1
        if (randomValue < p0) {
            binaryString += '0';
        } else {
            binaryString += '1';
        }
    }
    
    return binaryString;
}

vector <long long> encoding(string s){
    unordered_map<string,long long>table;
    table["0"]=0;
    table["1"]=1;
    cout<<"Table: "<<endl;
    
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
    long long length = 100;
    double p0 = 0.8; 
    double p1 = 0.2;
    string binaryString = generateBinaryString(length, p0, p1);
    vector<long long>output=encoding(binaryString);
    
    cout<<"Output: "<<endl;
    for(auto i:output){
        cout<<i<<" ";
    }
    cout<<decoder(output);
    return 0;
}
