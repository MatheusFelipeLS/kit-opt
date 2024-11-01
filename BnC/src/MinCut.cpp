#include "MinCut.h"

void showS(vector<vector<int>> &S) {
  for(int i = 0; i < S.size(); i++) {
    for(int j = 0; j < S[i].size(); j++) {
      cout << S[i][j] << " ";
    }
    cout << endl;
  }
}

int qtUsedNodes2(const vector<vector<int>> &subtours, int n) {
  int size = 0;
  vector<bool> V(n, false);

  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      V[subtours[i][j]] = true;
    }
  }

  for(int i = 0; i < V.size(); i++) {
    if(V[i]) size++;
  }

  return size;
}

void calculateDegreeOfConnection(double** x, vector<double> &degreeOfConnection, vector<vector<int>> &A, vector<vector<int>> &V, vector<bool> &CL) {
  for(int i = 0; i < A[A.size()-1].size(); i++) {
    for(int j = 0; j < V.size(); j++) {
      for(int k = 0; k < V[j].size(); k++) {
        if(A[A.size()-1][i] > V[j][k]) {
          if(CL[V[j][k]]) degreeOfConnection[j] += x[V[j][k]][A[A.size()-1][i]];
        } else if(A[A.size()-1][i] < V[j][k]) {
          if(CL[V[j][k]]) degreeOfConnection[j] += x[A[A.size()-1][i]][V[j][k]];
        }
      }
    }
  }
}


int mostTightlyConnectedVertex(vector<double> &degreeOfConnection) {
  int idx = 0;
  for(int i = 1; i < degreeOfConnection.size(); i++) {
    if(degreeOfConnection[i] > degreeOfConnection[idx]) idx = i;
  }

  return idx;
}

void forbiddenNodes(vector<bool> &CL, vector<vector<int>> &A) {
  for(int i = 0; i < A[A.size()-1].size(); i++) {
    CL[A[A.size()-1][i]] = false;
  }
}


vector<vector<int>> MinimumCutPhase(double **x, int n, vector<vector<int>> &V, int idxInitialSet) {
  vector<bool> CL(n, true);
  vector<double> degreeOfConnection(n);
  vector<vector<int>> A = {{}};

  A[0].insert(A[0].end(), V[idxInitialSet].begin(), V[idxInitialSet].end());

  int sizeV = qtUsedNodes2(V, n);
  int sizeA = 1;
  while(sizeA < sizeV) {
    forbiddenNodes(CL, A);

    calculateDegreeOfConnection(x, degreeOfConnection, A, V, CL);

    int idx = mostTightlyConnectedVertex(degreeOfConnection);

    degreeOfConnection[idx] = 0;
    A.insert(A.end(), V.begin()+idx, V.begin()+idx+1);

    sizeA = qtUsedNodes2(A, n);
  }

  return A;
}

double calculateCutOfThePhase(double** x, int n, vector<vector<int>> &S) {
  double cutOfThePhase = 0;
  vector<bool> CL(n, true);

  for(int i = 0; i < S[S.size()-1].size(); i++) CL[S[S.size()-1][i]] = false;

  for(int i = 0; i < S[S.size()-1].size(); i++) {
    for(int j = 0; j < S[S.size()-1][i]; j++) {
      if(CL[j]) {
        cutOfThePhase += x[j][S[S.size()-1][i]];
      }
    }

    for(int j = S[S.size()-1][i]+1; j < n; j++) {
      if(CL[j]) {
        cutOfThePhase += x[S[S.size()-1][i]][j];
      }
    }
  }
  return cutOfThePhase;
}

void merge2LastSets(vector<vector<int>>& V, vector<vector<int>> &S) {
  int idx1 = 0;
  int idx2 = 0;
  for(int i = 0; i < V.size(); i++) {
    if(V[i][0] == S[S.size()-1][0] || V[i][0] == S[S.size()-2][0]) {
      idx1 = i;
      break;
    }
  }

  for(int i = V.size()-1; i >= 0; i--) {
    if(V[i][0] == S[S.size()-1][0] || V[i][0] == S[S.size()-2][0]) {
      idx2 = i;
      break;
    }
  }

  V[idx1].insert(V[idx1].end(), V[idx2].begin(), V[idx2].end());
  V.erase(V.begin()+idx2);  
}


vector<vector<int>> SingleMinCut(double** x, int n, int initialNode) {
  vector<vector<int>> V(n);
  vector<vector<int>> sets;
  double cutOfThePhase;
  double cutmin = 2;

  for(int i = 0; i < n; i++) V[i].push_back(i);

  while(V.size() > 1) {
    vector<vector<int>> S = MinimumCutPhase(x, n, V, initialNode);

    cutOfThePhase = calculateCutOfThePhase(x, n, S);

    if(cutOfThePhase < 2) {
      sets.push_back(S[S.size()-1]);
      if(qtUsedNodes2(sets, n) >= n/2) return sets;
    }

    merge2LastSets(V, S);
  }

  return sets;
}
