#ifndef __TRIE_H__
#define __TRIE_H__

#define NUM_OF_BRANCH 36

class KeyInfo;
class TrieNode;
class Trie;

typedef KeyInfo *InfoPosition;
typedef TrieNode *TriePosition;

extern std::ofstream indexFile;

class KeyInfo {
  friend class TrieNode;
public:
  KeyInfo();
  KeyInfo(int textIndex, bool isTitle);
  ~KeyInfo();
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
  void Insert(std::string str, int textIndex, bool isTitle);
  void PrintAll();
private:
  void _EmptyTrie(TrieNode *pos);
  void _PrintAll(TriePosition pos, std::string &str);
  TrieNode root;
};

#endif
