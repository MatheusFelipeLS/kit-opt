#include "MaxBack.h"

int qtUsedNodes(const vector<vector<int>> &subtours) {
  int size = 0;
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      size++;
    }
  }
  return size;
}

int nextNode(vector<vector<int>> &subtours) {
  int noUsedNode = 0;

  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      if(subtours[i][j] == noUsedNode) {
        noUsedNode++;
        i = -1;
        break;
      }
    }
  }

  return noUsedNode;
}

vector<bool> createCandidateList(int n, int node) {
  vector<bool> CL(n, true);

  CL[node] = false;

  return CL;
}

double calculateCutMin(double** x, int n, int node) {
  double cutmin = 0;
  for(int i = 0; i < n; i++) {
    if(i == node) {
      cutmin += x[node][i] + x[i][node];
    }
  }

  return cutmin;
}

void calculateBv(double** x, int n, vector<double> &bv, vector<int> &S, vector<bool> &CL) {
  for(int i = 0; i < S[S.size()-1]; i++) {
    if(CL[i]) bv[i] += x[i][S[S.size()-1]]; 
  }

  for(int i = S[S.size()-1]; i < n; i++) {
    if(CL[i]) bv[i] += x[S[S.size()-1]][i];
  }
}

int getNodeWithBiggestMaxBack(const vector<double> &bv) {
  int idxBiggestCut = 0;
  for(int i = 1; i < bv.size(); i++) {
    if(bv[i] > bv[idxBiggestCut]) idxBiggestCut = i;
  }

  return idxBiggestCut;
}

vector<int> SingleMaxBack(double** x, int n, int initialNode, vector<vector<int>> &subtours) {
  vector<int> Smin = {initialNode};
  vector<int> S = {initialNode};
  vector<bool> CL = createCandidateList(n, initialNode);
  vector<double> bv(n); //max back de cada nó
  double cutmin = calculateCutMin(x, n, initialNode);
  double cutval;

  bv[initialNode] = -1;
  cutval = cutmin;

  while(S.size() < n) {
    calculateBv(x, n, bv, S, CL);

    int idxBiggestCut = getNodeWithBiggestMaxBack(bv);

    S.push_back(idxBiggestCut);
    cutval += 2 - (2 * bv[idxBiggestCut]);

    bv[idxBiggestCut] = -1;
    CL[idxBiggestCut] = false;

    if(cutval < cutmin) {
      cutmin = cutval;
      Smin = S;
    }
  }

  return Smin;
}
