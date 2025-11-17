compile and run tests:
  g++ tests/test_runner.cpp WordCounter.cpp -o tests.exe
  .\tests.exe
compile and run main:
  g++ main.cpp WordCounter.cpp -o wordcounter.exe -std=c++17
  .\wordcounter.exe input.txt ouput.csv 
