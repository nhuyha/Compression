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
    cout << "Entropy: " << entropy << endl;

   length=10;
    
    double compressionRatio ;
    string binaryString = generateBinaryString(length, p0, p1);
    cout << "Binary String: " << binaryString << endl;
    long long totalBits = 0;
    vector<long long>output=encoding(binaryString, totalBits);
    
    cout<<"encoder: "<<endl;
    for(auto i:output){
        cout<<i<<" ";
    }
    cout<<endl;
    string decoded = decoder(output);
    cout<<"decoder: "<<decoded<<endl;

    if (binaryString == decoded) {
        cout << "Success!" << endl;
    } else {
        cout << "Failed!" << endl;
    }

    cout<<(float)totalBits/length<<endl;

    outFile.close();
    return 0;
}