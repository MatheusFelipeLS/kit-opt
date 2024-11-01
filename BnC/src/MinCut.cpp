#include "MinCut.h"

int qtNodesOnSet(const vector<vector<int>> &subtours, int n) {
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

void calculateDegreeOfConnection(double** x, vector<double> &degreeOfConnection, const vector<int> &newSet, const vector<vector<int>> &V, const vector<bool> &CL) {
  for(int i = 0; i < newSet.size(); i++) {
    for(int j = 0; j < V.size(); j++) {
      for(int k = 0; k < V[j].size(); k++) {
        if(CL[V[j][k]]) {
          if(newSet[i] > V[j][k]) {
            degreeOfConnection[j] += x[V[j][k]][newSet[i]];
          } else if(newSet[i] < V[j][k]) {
            degreeOfConnection[j] += x[newSet[i]][V[j][k]];
          }
        }
      }
    }
  }
}


int mostTightlyConnectedSet(vector<double> &degreeOfConnection) {
  int idx = 0;
  for(int i = 1; i < degreeOfConnection.size(); i++) {
    if(degreeOfConnection[i] > degreeOfConnection[idx]) idx = i;
  }

  return idx;
}

void forbiddenSets(vector<bool> &CL, const vector<int> &newSet) {
  for(int i = 0; i < newSet.size(); i++) {
    CL[newSet[i]] = false;
  }
}


vector<vector<int>> MinimumCutPhase(double **x, int n, vector<vector<int>> &V, int idxInitialSet) {
  vector<bool> CL(n, true);
  vector<double> degreeOfConnection(n, 0.0);
  vector<vector<int>> A = {};

  A.insert(A.end(), V.begin()+idxInitialSet, V.begin()+idxInitialSet+1);

  int sizeV = V.size();

  while(A.size() < sizeV) {
    forbiddenSets(CL, A[A.size()-1]);

    // for(int i = 0; i < A[A.size()-1].size(); i++) cout << A[A.size()-1][i] << " ";
    // cout << endl;
    // char t;
    // cin >> t;

    calculateDegreeOfConnection(x, degreeOfConnection, A[A.size()-1], V, CL);

    int idx = mostTightlyConnectedSet(degreeOfConnection);

    degreeOfConnection[idx] = 0;
    A.insert(A.end(), V.begin()+idx, V.begin()+idx+1);
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


int merge2LastSets(vector<vector<int>>& V, vector<vector<int>> &S, int node) {
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

  if(idx2 <= node) node--;

  V[idx1].insert(V[idx1].end(), V[idx2].begin(), V[idx2].end());
  V.erase(V.begin()+idx2);

  return node;
}


vector<vector<int>> SingleMinCut(double** x, int n, int idxInitialNode) {
  vector<vector<int>> V(n);
  vector<vector<int>> sets;
  int idx = idxInitialNode;
  double cutOfThePhase;

  for(int i = 0; i < n; i++) V[i].push_back(i);

  while(V.size() > 1) {
    vector<vector<int>> S = MinimumCutPhase(x, n, V, idx);

    cutOfThePhase = calculateCutOfThePhase(x, n, S);

    if(cutOfThePhase < 2) {
      sets.push_back(S[S.size()-1]);
    }

    idx = merge2LastSets(V, S, idx);
  }


  return sets;
}
