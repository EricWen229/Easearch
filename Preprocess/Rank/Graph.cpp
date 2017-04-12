#include <iostream>
#include <cassert>

#include "Graph.h"

GraphNode::GraphNode() {
  this->nodeNo = -1;
  this->next = nullptr;
}

GraphNode::GraphNode(int nodeNo) {
  this->nodeNo = nodeNo;
  this->next = nullptr;
}

void GraphNode::DeleteList(GraphNode *head) {
  GraphNode *p = head, *prev = nullptr;
  while (p != nullptr) {
    prev = p;
    p = p->next;
    delete prev;
  }
}

GraphNode::~GraphNode() {
}

Graph::Graph() {
  this->numOfNode = -1;
  this->nodes = nullptr;
}

Graph::Graph(int numOfNode) {
  this->numOfNode = numOfNode;
  this->nodes = new GraphNode[this->numOfNode];
  assert(this->nodes != nullptr);
}

GraphPosition *Graph::Insert(int inIndex, int outIndex) {
  this->nodes[inIndex].next = new GraphNode(outIndex);
  assert(this->nodes[inIndex].next != nullptr);
  return &(this->nodes[inIndex].next->next); //return next position for further insertion
}

GraphPosition *Graph::Insert(int outIndex, GraphPosition *pos) {
  assert(pos != nullptr);
  GraphNode *buf = new GraphNode(outIndex);
  assert(buf != nullptr);
  (*(pos)) = buf;
  return &(buf->next); //return next position for further insertion
}

void Graph::PrintAll() {
  for (int i = 1; i < numOfNode; ++i) {
    GraphPosition p = this->nodes[i].next;
    std::cout << i << ":";
    while (p != nullptr) {
      std::cout << ' ' << p->nodeNo;
      p = p->next;
    }
    std::cout << '\n';
  }
}

double Graph::MultipleVector(int rowIndex, int pageNum, int *outDegree, double escape, double *vector) {
  //used by PageRank
  double result = 0.0;
  double escapeWeight = 1.0 / ((double)pageNum); //average weight of all nodes
  GraphPosition currGraphPos = this->nodes[rowIndex].next;
  for (int i = 1; i <= pageNum; ++i) {
    if (currGraphPos != nullptr && currGraphPos->nodeNo == i) { //match
      double currWeight = 1.0 / ((double)outDegree[i]);
      double finalWeight = (1.0 - escape) * currWeight + escape * escapeWeight;
      result += finalWeight * vector[i];
      currGraphPos = currGraphPos->next;
    }
    else if (currGraphPos == nullptr || currGraphPos->nodeNo > i) { //unmatch
      double finalWeight = 0.0;
      if (outDegree[i] == 0) { //out degree equals zero, then give it average weight towards every node
        finalWeight = escapeWeight;
      }
      else {
        finalWeight = escape * escapeWeight;
      }
      result += finalWeight * vector[i];
    }
    else {
      assert(0);
    }
  }
  return result;
}

Graph::~Graph() {
  for (int i = 0; i < this->numOfNode; ++i) {
    GraphNode::DeleteList(this->nodes[i].next);
  }
  delete [] this->nodes;
}
