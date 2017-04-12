#ifndef __UI_H__
#define __UI_H__

//#define DEBUG

#include "Trie.h"

const int maxInputLength = 50;
const int maxTokenNum = 10;

class Ui;
class ResultNode;
class ResultList;

class Ui : public cppcms::application {
public:
	Ui(cppcms::service &srv) : cppcms::application(srv) {}
	virtual void main(std::string url);
};

class ResultNode {
	friend class ResultList;
public:
	ResultNode();
	~ResultNode();
private:
	std::string title;
	std::string abstr;
	ResultNode *prev;
	ResultNode *next;
};

class ResultList {
	friend void RetrieveResult(InfoList result);
	friend class Ui;
public:
	ResultList();
	~ResultList();
	void Empty();
	void Add(int textIndex);
	void FillSlot();
	void NextPage();
	void PrevPage();
	void PrintAll();
	int TotalNum() {return this->resultNum;}
private:
	int resultNum;
	ResultNode *currDisplay;
	int currNo;
	ResultNode *head;
	ResultNode *tail;
};

void InitDictionary();
void InitSort();

#endif
