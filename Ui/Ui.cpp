#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

#include "Content.h"
#include "Trie.h"
#include "Ui.h"

using namespace std;

////////////////////////////////////////

static Trie s_dictionary;
static string s_key = "";
static ResultList s_result;
static string *s_slotTitle[10];
static string *s_slotAbstr[10];
static double *s_rank;
static string s_indexPath = "FormattedData/Index.txt";
static string s_rankPath = "FormattedData/Rank.txt";
static int s_pageNum = 16980;

////////////////////////////////////////


////////////////////////////////////////

InfoPosition *AddInfo(InfoPosition *last, int textIndex, bool isTitle, int timeOccur) {
  (*last) = new KeyInfo(textIndex, isTitle, timeOccur);
  return &((*last)->next);
}

void InitDictionary() {
	ifstream indexFile;
	indexFile.open(s_indexPath);
	assert(indexFile.is_open());
	string keyBuf;
	int indexBuf = 0;
	bool isTitleBuf = 0;
	int timeOccurBuf = 0;
	int numOfInfoBuf = 0;
	InfoPosition *positionBuf = nullptr;
	while (!indexFile.eof()) { //insert key and add info
		indexFile >> keyBuf >> numOfInfoBuf;
		positionBuf = s_dictionary.Insert(keyBuf);
		for (int i = 0; i < numOfInfoBuf; ++i) {
			indexFile >> indexBuf >> isTitleBuf >> timeOccurBuf;
			positionBuf = AddInfo(positionBuf, indexBuf, isTitleBuf, timeOccurBuf);
		}
	}
	indexFile.close();
}

void InitSort() {
  ifstream rankFile;
  rankFile.open(s_rankPath);
  assert(rankFile.is_open());
  s_rank = new double[s_pageNum + 1];
  assert(s_rank != nullptr);
  int pageIndex = 0;
  while (rankFile >> pageIndex) {
    rankFile >> s_rank[pageIndex];
  }
  rankFile.close();
}

InfoList BoolSearch(string &result) {
  string token[maxTokenNum];
  int tokenType[maxTokenNum];
  InfoList keyValue[maxTokenNum];
  int tokenCount = 0;

  regex exprRegex("[a-zA-Z0-9]+"); //extract the tokens
  auto wordsBegin = sregex_iterator(s_key.begin(), s_key.end(), exprRegex);
  auto wordsEnd = sregex_iterator();
  for (sregex_iterator i = wordsBegin; i != wordsEnd && tokenCount < maxTokenNum; ++i) {
    smatch match = *i;
    string matchStr = match.str();
    if (matchStr == "AND") {
      tokenType[tokenCount] = AND;
    }
    else if (matchStr == "OR") {
      tokenType[tokenCount] = OR;
    }
    else {
      if (tokenCount - 1 >= 0 && tokenType[tokenCount - 1] == KEY) { //no continuous keys in expression
        tokenType[tokenCount] = AND;
        ++tokenCount;
        if (!(tokenCount < maxTokenNum)) {
          break;
        }
      }
      for (int j = 0; j < matchStr.length(); ++j) {
        if (isalpha(matchStr[j])) {
          matchStr[j] = tolower(matchStr[j]);
        }
      }
      token[tokenCount] = matchStr;
      tokenType[tokenCount] = KEY;
      string realResultBuf = "";
      //when a key is found
      //search it and get its info immediately for future use
      keyValue[tokenCount] = s_dictionary.Search(token[tokenCount], realResultBuf);
      token[tokenCount] = realResultBuf;
    }
    ++tokenCount;
  }

  for (int i = 0; i < tokenCount; ++i) { //rebuild the expression for display
    if (i != 0) {
      result += " ";
    }
    if (tokenType[i] == AND) {
      result += "AND";
    }
    else if (tokenType[i] == OR) {
      result += "OR";
    }
    else {
      result += token[i];
    }
  }
  return KeyInfo::eval(tokenType, keyValue, 0, tokenCount - 1);
}

InfoList SortSearchResult(InfoList result) {
  return KeyInfo::SortList(result, s_rank);
}

void RetrieveResult(InfoList result) {
  s_result.Empty();
  InfoPosition currPos = result;
#ifdef DEBUG
	cout << "Result List:\n";
#endif
  while (currPos != nullptr) {
    s_result.Add(currPos->textIndex);
#ifdef DEBUG
    cout << currPos->textIndex << ' ';
#endif
    currPos = currPos->next;
  }
#ifdef DEBUG
  cout << '\n';
#endif
  s_result.FillSlot();
  KeyInfo::DeleteList(result);
}

////////////////////////////////////////


////////////////////////////////////////

ResultNode::ResultNode() {
	this->title = "invalid title";
	this->abstr = "invalid abstr";
	this->prev = nullptr;
	this->next = nullptr;
}

ResultNode::~ResultNode() {
}

////////////////////////////////////////


////////////////////////////////////////

ResultList::ResultList() {
	this->resultNum = 0;
  this->currDisplay = nullptr;
  this->currNo = -1;
	this->head = nullptr;
	this->tail = nullptr;
}

ResultList::~ResultList() {
	this->Empty();
}

void ResultList::Empty() {
  ResultNode *currPos = this->head, *prev = nullptr;
	while (currPos != nullptr) {
		prev = currPos;
		currPos = currPos->next;
		delete prev;
	}
  this->resultNum = 0;
  this->head = nullptr;
  this->tail = nullptr;
}

void ResultList::Add(int textIndex) {
  ifstream textFile;
  ostringstream pathBuf;
  pathBuf << "FormattedData/" << textIndex << ".txt"; //retrieve content from file
  textFile.open(pathBuf.str());
  assert(textFile.is_open());
  ResultNode *resultBuf = new ResultNode;
  getline(textFile, resultBuf->title);
  getline(textFile, resultBuf->abstr);
  textFile.close();
  if (this->tail == nullptr) {
    this->head = resultBuf;
    this->tail = resultBuf;
    this->currDisplay = head;
    this->currNo = 0;
  }
  else {
    this->tail->next = resultBuf;
    resultBuf->prev = this->tail;
    this->tail = resultBuf;
  }
  ++this->resultNum;
}

void ResultList::FillSlot() {
  ResultNode *currPos = this->currDisplay;
  for (int i = 0; i < 10; ++i) { //fill the slots displayed on web page
    if (i + currNo < this->resultNum) {
      (*(s_slotTitle[i])) = currPos->title;
      (*(s_slotAbstr[i])) = currPos->abstr;
      currPos = currPos->next;
    }
    else {
      (*(s_slotTitle[i])) = "";
      (*(s_slotAbstr[i])) = "";
    }
  }
}

void ResultList::NextPage() {
  if (this->currNo + 10 < this->resultNum) {
    for (int i = 0; i < 10; ++i) {
      this->currDisplay = this->currDisplay->next;
    }
    this->currNo += 10;
  }
  this->FillSlot();
}

void ResultList::PrevPage() {
  if (this->currNo - 10 >= 0) {
    for (int i = 0; i < 10; ++i) {
      this->currDisplay = this->currDisplay->prev;
    }
    this->currNo -= 10;
  }
  this->FillSlot();
}

void ResultList::PrintAll() {
  ResultNode *currPos = head;
  while (currPos != nullptr) {
    cout << currPos->title << '\n';
    cout << currPos->abstr << '\n' << '\n';
    currPos = currPos->next;
  }
}

////////////////////////////////////////


////////////////////////////////////////

void Ui::main(string url) {
	content::searchMessage c;
  s_slotTitle[0] = &c.result_0_title;
  s_slotTitle[1] = &c.result_1_title;
  s_slotTitle[2] = &c.result_2_title;
  s_slotTitle[3] = &c.result_3_title;
  s_slotTitle[4] = &c.result_4_title;
  s_slotTitle[5] = &c.result_5_title;
  s_slotTitle[6] = &c.result_6_title;
  s_slotTitle[7] = &c.result_7_title;
  s_slotTitle[8] = &c.result_8_title;
  s_slotTitle[9] = &c.result_9_title;
  s_slotAbstr[0] = &c.result_0_abstr;
  s_slotAbstr[1] = &c.result_1_abstr;
  s_slotAbstr[2] = &c.result_2_abstr;
  s_slotAbstr[3] = &c.result_3_abstr;
  s_slotAbstr[4] = &c.result_4_abstr;
  s_slotAbstr[5] = &c.result_5_abstr;
  s_slotAbstr[6] = &c.result_6_abstr;
  s_slotAbstr[7] = &c.result_7_abstr;
  s_slotAbstr[8] = &c.result_8_abstr;
  s_slotAbstr[9] = &c.result_9_abstr;
	if (request().request_method() == "POST") {
    if (request().post("submit") == "GO") { //search
      c.info.load(context());
  		if (c.info.validate()) {
  			s_key = c.info.key.value();
        if (s_key.length() > maxInputLength) {
          c.valid = "";
          c.result_info = "Come oooooooon! You know the key is toooooooo long!";
        }
        else {
          string realKey = "";
          InfoList result = BoolSearch(realKey);
          if (result == nullptr) { //nothing found
            c.valid = "";
            c.result_info = "What the hell was that? Better luck next time!";
          }
          else { //if result valid
            result = SortSearchResult(result);
            RetrieveResult(result);
#ifdef DEBUG
            cout << "Key: " << realKey << "\n\n";
#endif
            s_key = realKey;
            c.key = s_key;
            c.valid = "valid";
            ostringstream infoBuf;
            infoBuf << "Search: " << c.key << ", " << s_result.TotalNum();
            infoBuf << " results, page " << s_result.currNo/10 + 1 << "/" << s_result.TotalNum()/10 + 1;
            c.result_info = infoBuf.str();
          }
        }
  			c.info.clear(); //clear input text area
  		}
    }
    else {
      if (request().post("submit") == "previous page") {
        s_result.PrevPage();
      }
      else if (request().post("submit") == "next page") {
        s_result.NextPage();
      }
      else {
        assert(0);
      }
      if (!s_key.empty()) { //only display the content when former search is valid
        c.valid = "valid";
        c.key = s_key;
        ostringstream infoBuf;
        infoBuf << "Search: " << c.key << ", " << s_result.TotalNum();
        infoBuf << " results, page " << s_result.currNo/10 + 1 << "/" << s_result.TotalNum()/10 + 1;
        c.result_info = infoBuf.str();
      }
    }
	}
	render("searchMessage", c);
}

////////////////////////////////////////
