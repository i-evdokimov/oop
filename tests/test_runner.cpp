#include <iostream>
#include <fstream>
#include <map>
#include "../WordCounter.h"

// ------------------- Utility assertions -------------------

bool assertTrue(bool cond, const std::string& name) {
    if (cond) {
        std::cout << "[OK]  " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        return false;
    }
}

bool assertEq(int a, int b, const std::string& name) {
    if (a == b) {
        std::cout << "[OK]  " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << name << " (expected "
                  << b << " got " << a << ")\n";
        return false;
    }
}

bool assertStrEq(const std::string& a, const std::string& b, const std::string& name) {
    if (a == b) {
        std::cout << "[OK]  " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << name << " (expected '" << b << "' got '" << a << "')\n";
        return false;
    }
}

// ------------------- Tests -------------------

int main() {
    int passed = 0;
    int total = 0;

    // ---------- isWordChar ----------
    total++;
    passed += assertTrue(WordCounter::isWordChar('A'), "isWordChar ASCII letter");

    total++;
    passed += assertTrue(!WordCounter::isWordChar(' '), "isWordChar space");

    unsigned char utf = 0xD0; // начало UTF-8 буквы
    total++;
    passed += assertTrue(WordCounter::isWordChar(utf), "isWordChar UTF-8");

    total++;
    passed += assertTrue(!WordCounter::isWordChar('-'), "isWordChar hyphen should be false");

    total++;
    passed += assertTrue(!WordCounter::isWordChar('\t'), "isWordChar tab");

    total++;
    passed += assertTrue(!WordCounter::isWordChar(','), "isWordChar comma");

    // ---------- processFile: basic ----------
    {
        std::ofstream f("test1.txt");
        f << "Hello, world! HELLO\nПривет мир";
        f.close();

        WordCounter wc;
        total++;
        passed += assertTrue(wc.processFile("test1.txt"), "processFile loads file (basic)");
    }

    // ---------- processFile: repeated words ----------
    {
        std::ofstream f("test2.txt");
        f << "cat cat cat dog";
        f.close();

        WordCounter wc;
        wc.processFile("test2.txt");

        total++;
        passed += assertEq(wc.saveCSV("tmp2.csv"), true, "processFile no crash on repeated words");
    }

    // ---------- processFile: tricky separators ----------
    {
        std::ofstream f("test3.txt");
        f << "Hi!!!...there???man---ok";
        f.close();

        WordCounter wc;
        wc.processFile("test3.txt");

        // Ожидаемые слова: hi, there, man, ok
        total++;
        passed += assertEq(wc.saveCSV("tmp3.csv"), true, "processFile complex separators");
    }

    // ---------- processFile: ending without delimiter ----------
    {
        std::ofstream f("test4.txt");
        f << "hello world test";
        f.close();

        WordCounter wc;
        wc.processFile("test4.txt");

        total++;
        passed += assertEq(wc.saveCSV("tmp4.csv"), true, "processFile ending without delimiter");
    }

    // ---------- saveCSV: check BOM ----------
    {
        WordCounter wc;
        std::ofstream f("tmp5.txt");
        f << "hello world hello";
        f.close();
        wc.processFile("tmp5.txt");

        wc.saveCSV("out.csv");

        std::ifstream out("out.csv", std::ios::binary);
        unsigned char bom[3];
        out.read((char*)bom, 3);

        total++;
        passed += assertTrue(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF,
                             "CSV BOM present");
    }

    // ---------- saveCSV: check header ----------
    {
        std::ofstream f("tmp6.txt");
        f << "alpha beta beta";
        f.close();

        WordCounter wc;
        wc.processFile("tmp6.txt");
        wc.saveCSV("out2.csv");

        std::ifstream in("out2.csv", std::ios::binary);
        char bom[3];
        in.read(bom, 3);

        std::string header;
        std::getline(in, header);

        total++;
        passed += assertStrEq(header, "Word;Count;Percentage (%)", "CSV header");
    }

    // ---------- saveCSV: check sorting ----------
    {
        std::ofstream f("tmp7.txt");
        f << "x y y y z z";
        f.close();

        WordCounter wc;
        wc.processFile("tmp7.txt");
        wc.saveCSV("out3.csv");

        std::ifstream in("out3.csv", std::ios::binary);
        char bom[3];
        in.read(bom, 3);

        std::string header;
        std::getline(in, header);

        std::string firstWord;
        std::getline(in, firstWord);

        total++;
        passed += assertTrue(firstWord.find("y;3") != std::string::npos,
                             "CSV sorted by descending count");
    }

    // ---------- Summary ----------
    std::cout << "\n========== RESULT ==========\n";
    std::cout << passed << " / " << total << " tests passed\n";

    return 0;
}
