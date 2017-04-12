#include <string>
#include <iostream>
#include <cctype>
#include <cassert>

#include "Trie.h"

using namespace std;

////////////////////////////////////////

KeyInfo::KeyInfo() {
  this->next = nullptr;
  this->textIndex = -1;
  this->isTitle = false;
  this->timeOccur = -1;
}

KeyInfo::KeyInfo(int textIndex, bool isTitle, int timeOccur) {
  this->next = nullptr;
  this->textIndex = textIndex;
  this->isTitle = isTitle;
  this->timeOccur = timeOccur;
}

void KeyInfo::DeleteList(InfoList list) {
  InfoPosition prev = nullptr, p = list;
  while (p != nullptr) {
    prev = p;
    p = p->next;
    delete prev;
  }
}

int FindDecisiveOperand(int *tokenType, int leftIndex, int rightIndex) {
  int opSoFar = leftIndex, weightSoFar = 100;
  int weight[2];
  weight[AND] = 10;
  weight[OR] = 5;
  for (int i = leftIndex; i <= rightIndex; ++i) {
    //cout << tokenType[i] << ' ';
    if (tokenType[i] != KEY && weight[tokenType[i]] <= weightSoFar) { //find operand with least priority
      opSoFar = i;
      weightSoFar = weight[tokenType[i]];
    }
  }
  assert(tokenType[opSoFar] != KEY);
  //cout << '\n';
  return opSoFar;
}

InfoList KeyInfo::LogicAnd(InfoList listA, InfoList listB) {
  InfoList resultHead = nullptr, resultTail = nullptr;
  InfoPosition aPos = listA, bPos = listB;
  while (aPos != nullptr && bPos != nullptr) {
    if (aPos->textIndex == bPos->textIndex) {
      InfoPosition buf = new KeyInfo;
      buf->textIndex = aPos->textIndex;
      buf->isTitle = aPos->isTitle || bPos->isTitle;
      buf->timeOccur = aPos->timeOccur + bPos->timeOccur;
      if (resultHead == nullptr) { //match then merge
        resultHead = buf;
        resultTail = buf;
      }
      else {
        resultTail->next = buf;
        resultTail = buf;
      }
      aPos = aPos->next;
      bPos = bPos->next;
    }
    else if (aPos->textIndex < bPos->textIndex) { //unmatch then skip
      aPos = aPos->next;
    }
    else {
      bPos = bPos->next;
    }
  }
  DeleteList(listA);
  DeleteList(listB);
  return resultHead;
}

InfoList KeyInfo::LogicOr(InfoList listA, InfoList listB) {
  InfoList resultHead = nullptr, resultTail = nullptr;
  InfoPosition aPos = listA, bPos = listB;
  while (aPos != nullptr || bPos != nullptr) {
    InfoPosition buf = new KeyInfo;
    assert(buf != nullptr);
    if (aPos == nullptr) {
      buf->textIndex = bPos->textIndex;
      buf->isTitle = bPos->isTitle;
      buf->timeOccur = bPos->timeOccur;
      bPos = bPos->next;
    }
    else if (bPos == nullptr) {
      buf->textIndex = aPos->textIndex;
      buf->isTitle = aPos->isTitle;
      buf->timeOccur = aPos->timeOccur;
      aPos = aPos->next;
    }
    else if (aPos->textIndex < bPos->textIndex) {
      buf->textIndex = aPos->textIndex;
      buf->isTitle = aPos->isTitle;
      buf->timeOccur = aPos->timeOccur;
      aPos = aPos->next;
    }
    else if (bPos->textIndex < aPos->textIndex) {
      buf->textIndex = bPos->textIndex;
      buf->isTitle = bPos->isTitle;
      buf->timeOccur = bPos->timeOccur;
      bPos = bPos->next;
    }
    else { //match then merge
      buf->textIndex = aPos->textIndex;
      buf->isTitle = aPos->isTitle || bPos->isTitle;
      buf->timeOccur = aPos->timeOccur + bPos->timeOccur;
      aPos = aPos->next;
      bPos = bPos->next;
    }
    if (resultHead == nullptr) {
      resultHead = buf;
      resultTail = buf;
    }
    else {
      resultTail->next = buf;
      resultTail = buf;
    }
  }
  DeleteList(listA);
  DeleteList(listB);
  return resultHead;
}

InfoList KeyInfo::eval(int *tokenType, InfoList *keyValue, int leftIndex, int rightIndex) {
  if (leftIndex > rightIndex) { //unvalid expression
    return nullptr;
  }
  InfoList result = nullptr;
  if (leftIndex == rightIndex) {
    if (tokenType[leftIndex] == KEY) { //value is already stored
      InfoPosition currPos = keyValue[leftIndex], resultTail = nullptr; //so make a copy of it
      while (currPos != nullptr) {
        InfoPosition buf = new KeyInfo;
        buf->textIndex = currPos->textIndex;
        buf->isTitle = currPos->isTitle;
        buf->timeOccur = currPos->timeOccur;
        if (result == nullptr) {
          result = buf;
          resultTail = buf;
        }
        else {
          resultTail->next = buf;
          resultTail = buf;
        }
        currPos = currPos->next;
      }
      return result;
    }
    else {
      return nullptr;
    }
  }
  else {
    int pivot = FindDecisiveOperand(tokenType, leftIndex, rightIndex);
    //cout << pivot << '\n';
    InfoList left = eval(tokenType, keyValue, leftIndex, pivot - 1);
    InfoList right = eval(tokenType, keyValue, pivot + 1, rightIndex);
    if (left == nullptr || right == nullptr) { //special cases when null pointers occur
      if (left == nullptr && right == nullptr) {
        return nullptr;
      }
      else {
        return (left == nullptr)? right : left;
      }
    }
    else {
      if (tokenType[pivot] == AND) {
        return KeyInfo::LogicAnd(left, right);
      }
      else if (tokenType[pivot] == OR){
        return KeyInfo::LogicOr(left, right);
      }
      else {
        assert(0);
      }
    }
  }
}

int KeyInfo::Compare(InfoPosition nodeA, InfoPosition nodeB, double rankA, double rankB) {
  assert(nodeA != nullptr && nodeB != nullptr);
  //priority: is title > page rank > time occur
  if (nodeA->isTitle && !nodeB->isTitle) {
    return 1;
  }
  else if (!nodeA->isTitle && nodeB->isTitle) {
    return -1;
  }
  else if (rankA > rankB) {
    return 1;
  }
  else if (rankA < rankB) {
    return -1;
  }
  else if (nodeA->timeOccur > nodeB->timeOccur) {
    return 1;
  }
  else if (nodeA->timeOccur < nodeB->timeOccur) {
    return -1;
  }
  else {
    return 0;
  }
}

InfoList KeyInfo::SortList(InfoList head, double *rank) {
  if (head == nullptr || head->next == nullptr) {
    return head;
  }
  //insertion sort
  KeyInfo result;
  InfoPosition pList = head, pResult = nullptr;
  while (pList != nullptr) {
    pResult = &result;
    while (pResult->next != nullptr) {
      if (KeyInfo::Compare(pList, pResult->next, rank[pList->textIndex], rank[pResult->next->textIndex]) == 1) {
        break;
      }
      pResult = pResult->next;
    }
    InfoPosition pListPrev = pList;
    pList = pList->next;
    pListPrev->next = pResult->next;
    pResult->next = pListPrev;
  }
  return result.next;
}

KeyInfo::~KeyInfo() {
}

////////////////////////////////////////

////////////////////////////////////////

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

////////////////////////////////////////

////////////////////////////////////////

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

InfoPosition *Trie::Insert(string str) {
  TriePosition currPos = &this->root;
  for (int i = 0; i < str.length(); ++i) {
    int branchIndex = (isalpha(str[i]))? (str[i] - 'a') : (str[i] - '0' + 26);
    if (currPos->branch[branchIndex] == nullptr) {
      currPos->branch[branchIndex] = new TrieNode;
      assert(currPos->branch[branchIndex] != nullptr);
    }
    currPos = currPos->branch[branchIndex];
  }
  currPos->isEnd = true;
  currPos->info = nullptr;
  return &currPos->info;
}

InfoList Trie::Search(string str, string &result) {
  TriePosition currPos = &this->root;
  result = "";
  int nextPos = 0;

  for (int i = 0; i < str.length(); ++i) {
    if (isalpha(str[i])) { //proprocess to find next branch
      str[i] = tolower(str[i]);
      nextPos = str[i] - 'a';
    }
    else if (isdigit(str[i])) {
      nextPos = str[i] - '0' + 26;
    }
    else {
      assert(0);
    }

    if (currPos->branch[nextPos] != nullptr) { //matched
      currPos = currPos->branch[nextPos];
      result.push_back(str[i]);
    }
    else { //unmatched: find the first key after current match
      char nextChar;
      while (!currPos->isEnd) {
        for (int i = 0; i < NUM_OF_BRANCH; ++i) {
          if (currPos->branch[i] != nullptr) {
            if (i < 26) nextChar = 'a' + i;
            else nextChar = '0' + i;
            currPos = currPos->branch[i];
            result.push_back(nextChar);
            break;
          }
        }
      }
      return currPos->info;
    }
  }

  if (currPos->isEnd) { //precise match
    return currPos->info;
  }
  else { //substring matched: find the first key after current match
    char nextChar;
    while (!currPos->isEnd) {
      for (int i = 0; i < NUM_OF_BRANCH; ++i) {
        if (currPos->branch[i] != NULL) {
          if (i < 26) nextChar = 'a' + i;
          else nextChar = '0' + i;
          currPos = currPos->branch[i];
          result.push_back(nextChar);
          break;
        }
      }
    }
    return currPos->info;
  }
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

////////////////////////////////////////
