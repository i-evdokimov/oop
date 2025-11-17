#pragma once
#include <string>
#include <map>
#include <list>

class WordCounter {
public:
    WordCounter() : totalWords(0) {}

    bool processFile(const std::string& filename);
    bool saveCSV(const std::string& filename) const;

    static bool isWordChar(char c);

private:
    std::list<std::string> lines;
    std::map<std::string, int> wordCount;
    int totalWords;
};
