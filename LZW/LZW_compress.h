#include <iostream>
#include <string>
#include <vector>

using namespace std;
#ifndef LZW_compress_H
#define LZW_compress_H

string generateBinaryString(long long length, double p0, double p1);
vector <long long> encoding(string s,long long &totalBits);
string decoder(vector<long long>input);


#endif