#include "separation.h"

// SUPPORT FUNCTION //
void filterX(double** x, int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(x[i][j] < EPSILON) x[i][j] = 0;
    }
  }
}

void printEntrada(double** x, int n);
void printS(vector<vector<int>> &S);


int qtUsedNodes(const vector<vector<int>> &subtours) {
  int size = 0;
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      size++;
    }
  }

  return size;
}

// int nextNode(vector<vector<int>> &subtours) {
//   int noUsedNode = 0;

//   for(int i = 0; i < subtours.size(); i++) {
//     for(int j = 0; j < subtours[i].size(); j++) {
//       if(subtours[i][j] == noUsedNode) {
//         noUsedNode++;
//         i = -1;
//         break;
//       }
//     }
//   }

//   if(noUsedNode == n) return -1;

//   return noUsedNode;
// }

int nextNode(vector<bool> &inSubtours) {
  for(int i = 0; i < inSubtours.size(); i++) {
    if(!inSubtours[i]) return i;
  }

  return -1;
}

void putInSubtours(vector<int> &Smin, vector<bool> &inSubtours) {
  for(int i = 0; i < Smin.size(); i++) {
    inSubtours[Smin[i]] = true;
  }
}

vector<vector<int>> MaxBack(double** x, int n) {
  vector<vector<int>> subtours;
  vector<bool> inSubtours(n, false);

  filterX(x, n);

  int node = 0;
  while(node != -1) {
    vector<int> Smin = SingleMaxBack(x, n, node, subtours);
    putInSubtours(Smin, inSubtours);

    if(!Smin.empty() && Smin.size() != n) subtours.push_back(Smin);
    
    node = nextNode(inSubtours);
  }

  if(subtours.size() == 1) return {};
  return subtours;

}


void showSubtours(vector<vector<int>> &subtours) {
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      cout << subtours[i][j] << " ";
    }
    cout << endl;
  }
}

int qtNodesOnSets(const vector<vector<int>> &subtours, int n) {
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

void calculateDegreeOfConnection(double** x, vector<double> &DOC, const vector<int> &newSet, const vector<vector<int>> &V, const vector<bool> &CL) {
  for(int i = 0; i < newSet.size(); i++) {
    for(int j = 0; j < V.size(); j++) {
      for(int k = 0; k < V[j].size(); k++) {
        if(CL[j]) {
          if(newSet[i] > V[j][k]) {
            DOC[j] += x[V[j][k]][newSet[i]];
          } else if(newSet[i] < V[j][k]) {
            DOC[j] += x[newSet[i]][V[j][k]];
          }
        } else {
          break;
        }
      }
    }
  }
}


int mostTightlyConnectedSet(vector<double> &degreeOfConnection, vector<bool> &CL) {
  int idx = 0;
  for(int i = 0; i < degreeOfConnection.size(); i++) {
    if(degreeOfConnection[i] > degreeOfConnection[idx]) {
      idx = i;
    }
  }

  if(!CL[idx]) {
    for(int i = idx+1; i < CL.size(); i++) {
      if(CL[i]) {
        return i;
      }
    }
  }

  return idx;
}

void forbiddenSets(vector<bool> &CL, int idx) {
  CL[idx] =  false;
}


vector<vector<int>> MinimumCutPhase(double **x, int n, vector<vector<int>> &V, int idxInitialSet) {
  vector<bool> CL(V.size(), true);
  vector<double> degreeOfConnection(V.size(), 0.0);
  vector<vector<int>> A = {};
  int idx = idxInitialSet;

  A.insert(A.end(), V.begin()+idxInitialSet, V.begin()+idxInitialSet+1);

  int sizeV = V.size();

  int t;
  while(A.size() < sizeV) {
    forbiddenSets(CL, idx);

    // cout << "\nCL\n";
    // for(int i = 0; i < CL.size(); i++) cout << i << ": "<< CL[i] << "; ";
    // cout << endl;
    // cin >> t;

    calculateDegreeOfConnection(x, degreeOfConnection, A[A.size()-1], V, CL);

    // cout << "\ndegree\n";
    // for(int i = 0; i < degreeOfConnection.size(); i++) cout << i << ": "<< degreeOfConnection[i] << "; ";
    // cout << endl;
    // cin >> t;

    idx = mostTightlyConnectedSet(degreeOfConnection, CL);

    // cout << "idx: " << idx << endl;
    // cin >> t;

    A.insert(A.end(), V.begin()+idx, V.begin()+idx+1);

    // cout << "A: ";
    // printS(A);
    // cin >> t;

    degreeOfConnection[idx] = 0;

    // if(!CL[idx]) {
    //   cout << "???";
    //   int t;
    //   cin >>t;
    // }
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

int qtNodesOnLastSet(vector<int> &lastSet, int n) {
  vector<bool> nodesOnSets(n, false);
  for(int i = 0; i < lastSet.size(); i++) {
    nodesOnSets[lastSet[i]] = true;
  }
  

  int total = 0;
  for(int i = 0; i < nodesOnSets.size(); i++) {
    if(nodesOnSets[i]) {
      total++;
    }
  }

  return total;
}

void printSets(vector<vector<int>> &S);

vector<vector<int>> MinCut(double** x, int n) {
  filterX(x, n);

  srand(time(NULL));
  int node = 0;
  int cutmin = 99999999;
  vector<vector<int>> sets;
  vector<vector<int>> V(n);
  vector<int> sla;
  int idx = 0;
  double cutOfThePhase;

  for(int i = 0; i < n; i++) V[i].push_back(i);

  // cout << "MinCut" << endl;
  // printEntrada(x, n);

  while(V.size() > 1) {
    // cout << "V\n";
    // printS(V); 
    vector<vector<int>> S = MinimumCutPhase(x, n, V, idx);

    // cout << "S\n";
    // printS(S);    

    cutOfThePhase = calculateCutOfThePhase(x, n, S);
    // cout << "cutOfThePhase: " << cutOfThePhase << endl; 

    if(cutOfThePhase < 2 - EPSILON) {
      if(S[S.size()-1].size() != n) {
        sets.push_back(S[S.size()-1]); 
      }
      // cout << "Sets 1\n";
      // printS(sets);    
    } 
    // else if(cutOfThePhase < cutmin) {
    //   cutmin = cutOfThePhase;
    //   sla = S[S.size()-1];
    // }

    idx = merge2LastSets(V, S, idx);
    // int t;
    // cin >> t;
    // cout << endl << endl;   
  }

  // cout << "Sets\n";
  // printSets(sets);    
  // int t;
  // cin >> t;

  return sets;
}


// SUPPORT FUNCTION //
void printEntrada(double** x, int n) {
  cout << "n: " << n << endl << "x: " << endl; 
  for(int i = 0; i < n; i++) {
    double soma = 0;
    cout << "linha " << i;
    if(i < 10) cout << " ";
    cout << " | ";
    for(int j = 0; j < n; j++) {
      if(x[i][j] > 0.000005 || x[i][j] < -0.000005) 
        cout << j << ": " << x[i][j] << "; ";
      // else 
        // cout << "0 ";
      soma += x[i][j];
    }
    cout << " soma por linha: " << soma;
    cout << endl;

  }
}

void printS(vector<vector<int>> &S) {
  for(int i = 0; i < S.size(); i++) {
    cout << "{ ";
    for(int j = 0; j < S[i].size(); j++) {
      cout << S[i][j] << " ";
    }
    cout << "}, ";
  }
  cout << endl;
}

void printSets(vector<vector<int>> &S) {
  for(int i = 0; i < S.size(); i++) {
    for(int j = 0; j < S[i].size(); j++) {
      cout << S[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}
