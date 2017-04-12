#ifndef __GRAPH_H__
#define __GRAPH_H__

class GraphNode;
class Graph;

typedef GraphNode *GraphPosition;

class GraphNode {
  friend class Graph;
public:
  GraphNode();
  GraphNode(int nodeNo);
  ~GraphNode();
  static void DeleteList(GraphNode *head);
private:
  int nodeNo;
  GraphNode *next;
};

class Graph {
public:
  Graph();
  Graph(int numOfNode);
  GraphPosition *Insert(int inIndex, int outIndex);
  GraphPosition *Insert(int outIndex, GraphPosition *pos);
  void PrintAll();
  double MultipleVector(int rowIndex, int pageNum, int *outDegree, double escape, double *vector);
  ~Graph();
private:
  int numOfNode;
  GraphNode *nodes;
};

#endif
