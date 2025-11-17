#include "WordCounter.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cctype>

bool WordCounter::isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || (static_cast<unsigned char>(c) & 0x80);
}

bool WordCounter::processFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);

        std::string currentWord;
        for (size_t i = 0; i < line.size(); ++i) {
            unsigned char c = line[i];
            if (isWordChar(c)) {
                currentWord += c;
            } else if (!currentWord.empty()) {
                for (auto& ch : currentWord)
                    if ((unsigned char)ch < 128) ch = std::tolower(ch);
                wordCount[currentWord]++;
                totalWords++;
                currentWord.clear();
            }
        }
        if (!currentWord.empty()) {
            for (auto& ch : currentWord)
                if ((unsigned char)ch < 128) ch = std::tolower(ch);
            wordCount[currentWord]++;
            totalWords++;
        }
    }

    inputFile.close();
    return true;
}

bool WordCounter::saveCSV(const std::string& filename) const {
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Cannot write file: " << filename << "\n";
        return false;
    }

    unsigned char bom[] = {0xEF,0xBB,0xBF};
    outputFile.write(reinterpret_cast<char*>(bom), 3);

    outputFile << "Word;Count;Percentage (%)\n";

    std::vector<std::pair<std::string,int>> wordsVec(wordCount.begin(), wordCount.end());
    std::sort(wordsVec.begin(), wordsVec.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    for (const auto& p : wordsVec) {
        double percent = 100.0 * p.second / totalWords;
        outputFile << p.first << ";" << p.second << ";" << percent << "\n";
    }

    outputFile.close();
    return true;
}
