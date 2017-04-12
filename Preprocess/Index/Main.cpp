#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <cctype>
#include <cassert>

#include "Trie.h"

enum {INDEX, TITLE, ABSTRACT};

using namespace std;

ifstream rawTextFile;
ofstream indexFile;

int main(int argc, char *argv[]) {
  rawTextFile.open(argv[1]); //specified in Makefile, contains raw text file path
  assert(rawTextFile.is_open());
  string inputBuf("");
  Trie t;
  int status = INDEX;
  int textIndex = 0;
  while (rawTextFile >> inputBuf) {
    if (inputBuf.length() <= 2) { //unvalid string
      continue;
    }
    if (inputBuf[0] == '#' && inputBuf[1] == '#') {
      assert(status == INDEX);
      string::iterator it;
      for (it = inputBuf.begin(); it != inputBuf.end(); ++it) {
        if (*it == '#') {
          inputBuf.erase(it);
          --it;
        }
      }
      textIndex = stoi(inputBuf, nullptr, 10); //retrieve text index
      status = TITLE;
      continue;
    }
    assert(status == TITLE || status == ABSTRACT);
    getline(rawTextFile, inputBuf);
    regex exprRegex("[a-zA-Z0-9]+"); //use regular expression to extract all words from line
    auto wordsBegin = sregex_iterator(inputBuf.begin(), inputBuf.end(), exprRegex);
    auto wordsEnd = sregex_iterator();
    for (sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
      smatch match = *i;
      string matchStr = match.str();
      if (matchStr.length() <= 2) { //ignore words whose lengths are no greater than 2
        continue;
      }
      for (int j = 0; j < matchStr.length(); ++j) {
        if (isalpha(matchStr[j])) {
          matchStr[j] = tolower(matchStr[j]); //preprocess of word
        }
      }
      t.Insert(matchStr, textIndex, (status == TITLE)); //add word to trie
    }
    status = (status == TITLE)? ABSTRACT : INDEX;
  }
  rawTextFile.close();
  indexFile.open(argv[2]); //specified in Makefile, contains index file path
  assert(indexFile.is_open());
  t.PrintAll();
  indexFile.close();
  return 0;
}
