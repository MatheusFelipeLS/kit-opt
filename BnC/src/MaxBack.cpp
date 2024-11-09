#include "MaxBack.h"

vector<bool> createCandidateList(int n, int node) {
  vector<bool> CL(n, true);

  CL[node] = false;

  return CL;
}

double calculateCutMin(double** x, int n, int node) {
  double cutmin = 0;
  for(int i = 0; i < n; i++) {
    if(i == node) {
      continue;
    }
    
    if(i < node) cutmin += x[i][node];

    else cutmin += x[node][i];
  
  }

  return cutmin;
}

void calculateBv(double** x, int n, vector<double> &bv, int lastNode, vector<bool> &CL) {
  for(int i = 0; i < n; i++) {
    
    if(!CL[i]) continue;

    if(i < lastNode) bv[i] += x[i][lastNode]; 

    else bv[i] += x[lastNode][i];
  }
}

int getNodeWithBiggestMaxBack(const vector<double> &bv) {
  int idxBiggestCut = 0;
  double bestMaxBack = -1;
  for(int i = 0; i < bv.size(); i++) {
    if(bv[i] > bestMaxBack) {
      idxBiggestCut = i;
      bestMaxBack = bv[i];
    }
  }

  return idxBiggestCut;
}

vector<int> SingleMaxBack(double** x, int n, int initialNode, vector<vector<int>> &subtours) {
  vector<int> S = {initialNode};
  vector<int> Smin = {initialNode};
  vector<double> bv(n, 0.0); //max back de cada nó
  double cutmin = calculateCutMin(x, n, initialNode);
  double cutval = cutmin;

  vector<bool> CL(n, true);
  CL[initialNode] = false;

  bv[initialNode] = -1;

  while(S.size() < n) {
    calculateBv(x, n, bv, S[S.size()-1], CL);

    int idxBiggestCut = getNodeWithBiggestMaxBack(bv);

    S.push_back(idxBiggestCut);

    cutval = cutval + 2 - (2 * bv[idxBiggestCut]);

    bv[idxBiggestCut] = -1;

    CL[idxBiggestCut] = false;

    if(cutval < cutmin) {

      cutmin = cutval;

      Smin = S;

    }
  }

  return Smin;
}
