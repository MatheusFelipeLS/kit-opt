#include "separation.h"

void printEntrada(double** x, int n);
void showS(vector<vector<int>> &S);
void showVector(vector<int> &v);
void showBv(vector<double> &v);
void filterX(double** x, int n);

int usedNodes(const vector<vector<int>> &subtours) {
  int size = 0;
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      size++;
    }
  }
  return size;
}

int nextNode(vector<vector<int>> &subtours) {
  int noUsedNode = 1;

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

double calculateMinCut(double** x, int n, int node) {
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

vector<int> mb(double** x, int n, int initialNode, vector<vector<int>> &subtours) {
  vector<int> Smin = {initialNode};
  vector<int> S = {initialNode};
  vector<bool> CL = createCandidateList(n, initialNode);
  vector<double> bv(n); //max back de cada nó
  double cutmin = calculateMinCut(x, n, initialNode);
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

vector<vector<int>> MaxBack(double** x, int n) {
  vector<vector<int>> subtours;
  int t;

  filterX(x, n);

  int node = 0;

  while(true) {
    int size = usedNodes(subtours);
    if(size == n) break;
    vector<int> Smin = mb(x, n, node, subtours);
    subtours.push_back(Smin);
    node = nextNode(subtours);
  }

  if(subtours.size() == 1) return {};

  return subtours;
}

vector<vector<int>> MinCut(double** x, int n) {
  // printEntrada(x, n);
  vector<vector<int>> m;
  return m;
}

void filterX(double** x, int n) {
  //filtrando NaN e valores menores que EPSILON
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(isnan(x[i][j]) || x[i][j] < EPSILON) x[i][j] = 0;
    }
  }
}

void printEntrada(double** x, int n) {
  cout << "n: " << n << endl << "x: " << endl; 
  for(int i = 0; i < n; i++) {
    double sla = 0;
    cout << "linha " << i;
    if(i < 10) cout << " ";
    cout << " | ";
    for(int j = 0; j < n; j++) {
      if(x[i][j] > 0.000005 || x[i][j] < -0.000005) 
        cout << x[i][j] << " ";
      else 
        cout << "0 ";
      sla += x[i][j];
    }
    cout << " soma por linha: " << sla;
    cout << endl;

  }
}

void showS(vector<vector<int>> &S) {
  for(int i = 0; i < S.size(); i++) {
    for(int j = 0; j < S[i].size(); j++) {
      cout << S[i][j] << " ";
    }
    cout << endl;
  }
}

void showVector(vector<int> &v) {
  for(int i = 0; i < v.size(); i++) {
      cout << v[i] << " ";
  }
  cout << endl;
}

void showBv(vector<double> &v) {
  for(int i = 0; i < v.size(); i++) {
      cout << v[i] << " ";
  }
  cout << endl;
}
