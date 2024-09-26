#include "separation.h"

void printEntrada(double** x, int n);
void showS(vector<vector<int>> &S);
void showVector(vector<int> &v);
void showBv(vector<double> &v);
void nanFilter(double** x, int n);

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

vector<int> createCandidateList(vector<vector<int>> &subtours, int n) {
  vector<int> CL(n);

  for(int i = 0; i < n; i++) {
    CL[i] = i;
  }

  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      CL[subtours[i][j]] = 0;
    }
  }

  return CL;
}

vector<int> mb(double** x, int n, int initialNode, vector<vector<int>> &subtours) {
  int t;

  vector<int> Smin = {initialNode};
  vector<int> S = Smin;
  vector<int> CL = createCandidateList(subtours, n);
  double cutmin = 0;
  double cutval;
  vector<double> bv(n); //max back de cada nó

  bv[0] = -1;
  CL[initialNode] = 0;

  //somando o grau dos nós para achar o corte mínimo
  for(int i = 1; i < n; i++) {
    if(CL[i]) {
      cutmin += x[initialNode][i] + x[i][initialNode];
      bv[i] += x[initialNode][i] + x[i][initialNode];
    }
  }

  cout << "bv ";
  showBv(bv);
  cutval = cutmin;

  while(S.size() < n) {
    cout << "S ";
    showVector(S);
    cout << "bv ";
    showBv(bv);

    int idxBiggestCut = 0;
    for(int i = 1; i < CL.size(); i++) {
      if(bv[CL[i]] > bv[CL[idxBiggestCut]]) idxBiggestCut = i;
    }

    S.push_back(CL[idxBiggestCut]);
    cutval += 2 - (2 * bv[CL[idxBiggestCut]]);

    bv[CL[idxBiggestCut]] = -1;
    CL[idxBiggestCut] = 0;

    if(cutval < cutmin) {
      cutmin = cutval;
      Smin = S;
      int idxNoUsedNode = 1;
      double degree = -999999999;
      for(int i = 1; i < n; i++) {
        if(CL[i] && bv[i] > degree) {
          idxNoUsedNode = i;
          degree = bv[i];
        }
      }

      S.push_back(CL[idxNoUsedNode]);
      CL[idxNoUsedNode] = 0;

      for(int i = 1; i < n; i++) {
        if(CL[i]) {
          cutval += x[S[S.size()-1]][i] + x[i][S[S.size()-1]];
        }
      }
    }

    for(int i = 0; i < n; i++) {
      if(CL[i]) {
        bv[i] += x[S[S.size()-1]][i] + x[i][S[S.size()-1]];
      }
    }
  }

  cout << "Smin ";
  showVector(Smin);
  // cin >> t;

  return Smin;
}

vector<vector<int>> MaxBack(double** x, int n) {
  vector<vector<int>> subtours;
  int t;

  nanFilter(x, n);

  cout << "Entrada maxback\n";
  printEntrada(x, n);
  // cin >> t;


  int node = 0;

  while(true) {
    int size = usedNodes(subtours);
    if(size == n) break;
    vector<int> Smin = mb(x, n, node, subtours);
    subtours.push_back(Smin);
    node = nextNode(subtours);
  }

  cout << "subtours\n";
  showS(subtours); 
  cin >> t; 
  return subtours;
}

vector<vector<int>> MinCut(double** x, int n) {
  // printEntrada(x, n);
  vector<vector<int>> m;
  return m;
}

void nanFilter(double** x, int n) {
  //filtrando NaN
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(isnan(x[i][j])) x[i][j] = 0;
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