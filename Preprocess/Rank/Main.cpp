#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "Graph.h"

using namespace std;

static const int s_pageNum = 16980; //num of pages(i.e. articles)
static const double s_escape = 0.2; //probability of heading towards a random node
static const double s_min = 1e-5; //minimum value that vector can be seen as static
static int *s_outDegree = nullptr;
static Graph s_graph(s_pageNum + 1);
static double *s_vector = nullptr;

void InitOutDegree(string rawLinkPath) {
  s_outDegree = new int[s_pageNum + 1];
  for (int i = 0; i <= s_pageNum; ++i) {
    s_outDegree[i] = 0;
  }
  assert(s_outDegree != nullptr);
  ifstream rawLinkFile;
  rawLinkFile.open(rawLinkPath);
  assert(rawLinkFile.is_open());
  char c_dontcare = 0; //these are punctuations that won't be used
  int i_dontcare = 0; //so don't care, they won't be used
  int numOfInDegree = 0, outIndex = 0;
  while (rawLinkFile >> c_dontcare) {
    assert(c_dontcare == '#');
    rawLinkFile >> c_dontcare;
    assert(c_dontcare == '#');
    rawLinkFile >> i_dontcare;
    rawLinkFile >> c_dontcare;
    assert(c_dontcare == ':');
    rawLinkFile >> numOfInDegree;
    rawLinkFile >> c_dontcare;
    for (int i = 0; i < numOfInDegree; ++i) {
      rawLinkFile >> outIndex;
      ++s_outDegree[outIndex];
    }
  }
  rawLinkFile.close();
}

void InitGraph(string rawLinkPath) {
  ifstream rawLinkFile;
  rawLinkFile.open(rawLinkPath);
  assert(rawLinkFile.is_open());
  char c_dontcare = 0;
  int inIndex = 0, outIndex = 0, numOfInDegree = 0;
  while (rawLinkFile >> c_dontcare) {
    assert(c_dontcare == '#');
    rawLinkFile >> c_dontcare;
    assert(c_dontcare == '#');
    rawLinkFile >> inIndex;
    rawLinkFile >> c_dontcare;
    assert(c_dontcare == ':');
    rawLinkFile >> numOfInDegree;
    rawLinkFile >> c_dontcare;
    GraphPosition *buf = nullptr;
    for (int i = 0; i < numOfInDegree; ++i) {
      rawLinkFile >> outIndex;
      if (i == 0) {
        //ATTENTION:
        //following piece of code is based on fact that
        //all out index are initially in ascending order
        //else it will go WRONG
        buf = s_graph.Insert(inIndex, outIndex);
      }
      else {
        buf = s_graph.Insert(outIndex, buf);
      }
    }
  }
}

void InitVector() {
  s_vector = new double[s_pageNum + 1];
  assert(s_vector != nullptr);
  for (int i = 0; i <= s_pageNum; ++i) {
    s_vector[i] = 1.0 / ((double)s_pageNum); //initalize with average weight
  }
}

double abs(double value) {
  return (value >= 0)? value : -value;
}

double Distance(double *newVector) {
  double distance = 0.0;
  for (int i = 1; i <= s_pageNum; ++i) {
    distance += abs(s_vector[i] - newVector[i]); //calculate similarity
  }
  return distance;
}

double Total(double *newVector, int size) {
  double total = 0.0;
  for (int i = 1; i < size; ++i) {
    total += newVector[i];
  }
  return total;
}

void PageRank() {
  double *newVector = new double[s_pageNum + 1];
  assert(newVector != nullptr);
  for (int i = 1; i <= s_pageNum; ++i) {
    newVector[i] = s_graph.MultipleVector(i, s_pageNum, s_outDegree, s_escape, s_vector);
  }
  double dist = Distance(newVector);
  while (dist > s_min) { //while vector not static, keep iterating until then
    for (int i = 1; i <= s_pageNum; ++i) {
      s_vector[i] = newVector[i];
    }
    for (int i = 1; i <= s_pageNum; ++i) {
      //each time vector multiplis a row and gets a number
      //all of which form new vector
      newVector[i] = s_graph.MultipleVector(i, s_pageNum, s_outDegree, s_escape, s_vector);
    }
    dist = Distance(newVector);
  }
  for (int i = 1; i <= s_pageNum; ++i) {
    s_vector[i] = newVector[i];
  }
  delete [] newVector;
}

void WriteFile(string rankPath) {
  ofstream rankFile;
  rankFile.open(rankPath);
  assert(rankFile.is_open());
  rankFile << fixed;
  for (int i = 1; i <= s_pageNum; ++i) {
    rankFile << i << "     " << s_vector[i] << '\n';
  }
  rankFile.close();
}

int main(int argc, char *argv[]) {
  InitOutDegree(argv[1]);
  InitGraph(argv[1]);
  InitVector();
  PageRank();
  WriteFile(argv[2]);
  delete [] s_outDegree;
  delete [] s_vector;
  return 0;
}
