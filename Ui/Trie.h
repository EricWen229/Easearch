#ifndef __TRIE_H__
#define __TRIE_H__

#define NUM_OF_BRANCH 36

enum {AND, OR, KEY};

class KeyInfo;
class TrieNode;
class Trie;

typedef KeyInfo *InfoPosition;
typedef KeyInfo *InfoList;
typedef TrieNode *TriePosition;

class KeyInfo {
  friend class TrieNode;
  friend InfoPosition *AddInfo(InfoPosition *last, int textIndex, bool isTitle, int timeOccur);
  friend InfoList eval(int *tokenType, InfoList *keyValue, int leftIndex, int rightIndex, bool isRoot);
  friend void RetrieveResult(InfoList result);
public:
  KeyInfo();
  KeyInfo(int textIndex, bool isTitle, int timeOccur);
  ~KeyInfo();
  static void DeleteList(InfoList list);
  static InfoList LogicAnd(InfoList listA, InfoList listB);
  static InfoList LogicOr(InfoList listA, InfoList listB);
  static InfoList eval(int *tokenType, InfoList *keyValue, int leftIndex, int rightIndex);
  static int Compare(InfoPosition nodeA, InfoPosition nodeB, double rankA, double rankB);
  static InfoList SortList(InfoList head, double *rank);
private:
  KeyInfo *next;

  int textIndex;
  bool isTitle;
  int timeOccur;
};

class TrieNode {
  friend class Trie;
public:
  TrieNode();
  ~TrieNode();
  void AddInfo(int textIndex, bool isTitle);
  void PrintInfo();
private:
  void DeleteInfo();

  TrieNode *branch[NUM_OF_BRANCH];
  bool isEnd;
  KeyInfo *info;
};

class Trie {
public:
  Trie();
  ~Trie();
  void EmptyTrie();
  InfoPosition *Insert(std::string str);
  InfoList Search(std::string str, std::string &result);
  void PrintAll();
private:
  void _EmptyTrie(TrieNode *pos);
  void _PrintAll(TriePosition pos, std::string &str);
  TrieNode root;
};

#endif
