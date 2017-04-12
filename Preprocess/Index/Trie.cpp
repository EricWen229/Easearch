#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cassert>

#include "Trie.h"

using namespace std;

//////////////////////////////

KeyInfo::KeyInfo() {
  this->next = nullptr;
  this->textIndex = -1;
  this->isTitle = false;
  this->timeOccur = -1;
}

KeyInfo::KeyInfo(int textIndex, bool isTitle) {
  this->next = nullptr;
  this->textIndex = textIndex;
  this->isTitle = isTitle;
  this->timeOccur = 1;
}

KeyInfo::~KeyInfo() {
}

//////////////////////////////


//////////////////////////////

TrieNode::TrieNode() {
  for (int i = 0; i < NUM_OF_BRANCH; ++i) {
    this->branch[i] = nullptr;
  }
  this->isEnd = false;
  this->info = nullptr;
}

TrieNode::~TrieNode() {
  this->DeleteInfo();
}

void TrieNode::AddInfo(int textIndex, bool isTitle) {
  if (this->info == nullptr) { //head to be specified
    this->info = new KeyInfo(textIndex, isTitle);
    assert(this->info != nullptr);
    return;
  }

  InfoPosition p = this->info, prev = nullptr;
  while (p != nullptr) {
    if (p->textIndex >= textIndex) { //keep info in ascending order
      break;
    }
    prev = p;
    p = p->next;
  }

  if (p == nullptr) {
    prev->next = new KeyInfo(textIndex, isTitle);
    assert(prev->next != nullptr);
  }
  else if (p->textIndex == textIndex) { //repeat
    ++(p->timeOccur);
    p->isTitle = p->isTitle || isTitle;
  }
  else {
    InfoPosition buf = new KeyInfo(textIndex, isTitle);
    buf->next = prev->next;
    prev->next = buf;
  }
}

void TrieNode::PrintInfo() {
  if (this->info != nullptr) {
    InfoPosition p = this->info;
    int count = 0;
    while (p != nullptr) {
      ++count;
      p = p->next;
    }
    indexFile << '\t' << count;
    p = this->info;
    while (p != nullptr) {
      indexFile << '\t' << p->textIndex << '\t' << p->isTitle;
      indexFile << '\t' << p->timeOccur;
      p = p->next;
    }
  }
  else {
    indexFile << '\t' << 0;
  }
}

void TrieNode::DeleteInfo() {
  if (this->info != nullptr) {
    InfoPosition prev = nullptr, p = this->info;
    while (p != nullptr) {
      prev = p;
      p = p->next;
      delete prev;
    }
    this->info = nullptr;
  }
}

//////////////////////////////


//////////////////////////////

Trie::Trie() {
}

Trie::~Trie() {
  this->EmptyTrie();
}

void Trie::EmptyTrie() {
  for (int i = 0; i < NUM_OF_BRANCH; ++i) {
    this->_EmptyTrie(root.branch[i]);
  }
}

void Trie::Insert(string str, int textIndex, bool isTitle) {
  TriePosition currPos = &this->root;
  for (int i = 0; i < str.length(); ++i) { //spread along given string
    int branchIndex = (isalpha(str[i]))? (str[i] - 'a') : (str[i] - '0' + 26);
    if (currPos->branch[branchIndex] == nullptr) {
      currPos->branch[branchIndex] = new TrieNode;
      assert(currPos->branch[branchIndex] != nullptr);
    }
    currPos = currPos->branch[branchIndex];
  }
  currPos->isEnd = true; //mark the end
  currPos->AddInfo(textIndex, isTitle);
}

void Trie::PrintAll() {
  string wordBuf("");
  _PrintAll(&this->root, wordBuf);
}

void Trie::_EmptyTrie(TrieNode *pos) {
  if (pos != nullptr) {
    for (int i = 0; i < NUM_OF_BRANCH; ++i) {
      this->_EmptyTrie(pos->branch[i]);
    }
    pos->DeleteInfo();
    delete pos;
  }
}

void Trie::_PrintAll(TriePosition pos, string &str) {
  if (pos->isEnd) { //end of a word
    indexFile << str;
    pos->PrintInfo();
    indexFile << '\n';
  }
  for (int i = 0; i < NUM_OF_BRANCH; ++i) {
    if (pos->branch[i] != nullptr) {
      char currChar = (i < 26)? ('a' + i) : ('0' + (i - 26));
      str.push_back(currChar);
      _PrintAll(pos->branch[i], str); //recursively print all words in trie
      str.pop_back();
    }
  }
}

//////////////////////////////
