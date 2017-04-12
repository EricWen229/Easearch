#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream rawText;
  ofstream formattedText;

  rawText.open(argv[1]); //specified in Makefile, contains raw text path
  assert(rawText.is_open());

  while (!rawText.eof()) {
    string inputBuf;
    int textIndex = 0;

    rawText >> inputBuf;
    string::iterator it;
    for (it = inputBuf.begin(); it != inputBuf.end(); ++it) {
      if (*it == '#') {
        inputBuf.erase(it);
        --it;
      }
    }
    rawText.ignore(1);

    formattedText.open(argv[2] + inputBuf + ".txt"); //argv[2] specified in Makefile, contains formatted data path
    assert(formattedText.is_open());
    getline(rawText, inputBuf);
    formattedText << inputBuf << '\n';
    getline(rawText, inputBuf);
    formattedText << inputBuf << '\n';
    formattedText.close();
  }
  rawText.close();
  return 0;
}
