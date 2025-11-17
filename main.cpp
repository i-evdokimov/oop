#include <iostream>
#include "WordCounter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: word_count.exe input.txt output.csv\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    WordCounter wc;
    std::cout << "Reading and counting words...\n";
    if (!wc.processFile(inputFile)) return 1;

    std::cout << "Saving CSV...\n";
    if (!wc.saveCSV(outputFile)) return 1;

    std::cout << "Done! CSV saved to " << outputFile << "\n";
    return 0;
}