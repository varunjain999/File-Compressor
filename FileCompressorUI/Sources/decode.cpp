#include <iostream>
#include "huffman.hpp"
using namespace std;

void decodeFile(const char* inputFile, const char* outputFile) {
    huffman f(inputFile, outputFile);
    f.decompress();
    cout << "Decompressed successfully" << endl;
}
