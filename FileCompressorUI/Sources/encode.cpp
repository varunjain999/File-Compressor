#include <iostream>
#include "huffman.hpp"
using namespace std;

void encodeFile(const char* inputFile, const char* outputFile) {
    huffman f(inputFile, outputFile);
    f.compress();
    cout << "Compressed successfully" << endl;
}
