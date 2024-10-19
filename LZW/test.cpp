#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "LZW_compress.h"

using namespace std;

long long numbits(long long n){
    if (n == 0) {
        return 1;
    }
    return ceil(log2(n));
}
vector <long long> encoding(string s,long long &totalBits, long long &code){
    unordered_map<string,long long>table;
    table["0"]=0;
    table["1"]=1;
    // cout<<"Table: "<<endl;
    
    code=2;
    vector <long long> output;
    string p="",c="";
    p+=s[0];
    for(int i=0; i<s.length();i++){
        if(i!=s.length()-1){
            c+=s[i+1];
        }
        if(table.find(p+c)!=table.end()){
            p=p+c;
        }else{
            output.push_back(table[p]);
            totalBits+=numbits(table[p])+1;
            table[p+c]=code;
            code++;
            p=c;
        }
        c="";
    }
    output.push_back(table[p]);
    totalBits+=numbits(table[p]);
    cout<<"Max length of codewords: "<<code<<endl;
    return output;
}

int main() {
    // Create an output file stream (ofstream) object
    std::ofstream outFile("output.txt");

    // Check if the file is opened successfully
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return 1;
    }

    long long length;
    double p0 = 0.8; 
    double p1 = 0.2;
    double entropy = -p0 * log2(p0) - p1 * log2(p1);
    outFile << "Entropy: " << entropy << endl;

    double compressionRatio ;
    for (length = 1; length <= 1000000000; length *= 10) {
        cout<<"Length: "<<length<<endl;
        auto start = chrono::high_resolution_clock::now();
        string binaryString = generateBinaryString(length, p0, p1);
        long long totalBits = 0; long long code=0;
        vector<long long>output=encoding(binaryString, totalBits, code);
        
        string decoded = decoder(output);
        if (binaryString == decoded) {
            cout << "Success!" << endl;
        } else {
            cout << "Failed!" << endl;
        }
        compressionRatio = (double)totalBits/length;
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::duration<double>>(stop - start);
        outFile << std::fixed << setprecision(2);
        outFile << "Length: " << length <<" Max length of codewords: "<<numbits(code-1)<<"bits " << " Compression Ratio: " << compressionRatio<< " Time: "<<duration.count() <<"s"<< endl;
    }

    outFile.close();
    return 0;
}
