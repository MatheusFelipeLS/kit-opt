#include "separation.h"

// SUPPORT FUNCTION //
void filterX(double** x, int n);
void printEntrada(double** x, int n);
// void showS(vector<vector<int>> &S);
void showVector(vector<int> &v);
void showBv(vector<double> &v);


vector<vector<int>> MaxBack(double** x, int n) {
  vector<vector<int>> subtours;

  filterX(x, n);

  int node = 0;

  while(true) {
    int size = qtUsedNodes(subtours);
    if(size == n) break;
    vector<int> Smin = SingleMaxBack(x, n, node, subtours);
    subtours.push_back(Smin);
    node = nextNode(subtours);
  }

  if(subtours.size() == 1) return {};

  return subtours;
}

bool allNodesOnSubtours(int n, vector<vector<int>> &subtours) {
  vector<int> t(n);

  for(int i = 0; i < n; i++) t[i] = i;

  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      for(int k = 0; k < t.size(); k++) {
        if(subtours[i][j] == t[k]) {
          t.erase(t.begin()+k);
          
          if(t.size() == 0) return true;

          break;
        }
      }
    }
  }

  return false;
}


int nodeOutsideSubtour(vector<vector<int>> &subtours, vector<bool> &possibleNodes) {
  int noUsedNode = 0;

  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      if(possibleNodes[j] == false) {
        noUsedNode++;
        i = -1;
        break;
      }
      if(subtours[i][j] == noUsedNode) {
        noUsedNode++;
        i = -1;
        break;
      }
    }
  }

  possibleNodes[noUsedNode] = false;
  return noUsedNode;
}

int nodeInSubtour(vector<vector<int>> &subtours, vector<bool> &possibleNodes) {

  possibleNodes[subtours[subtours.size()-1][0]] = false;
  return subtours[subtours.size()-1][0];
}

void showSubtours(vector<vector<int>> &subtours) {
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      cout << subtours[i][j] << " ";
    }
    cout << endl;
  }
}

bool sInSubtours(vector<vector<int>> &subtours, vector<int> &S) {
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      if(S.size() != subtours[i].size()) break;

      if(S[j] != subtours[i][j]) break;
      if(j == subtours[i].size()-1) return false;
    }
  }

  return true;
}

void getComplementary(vector<vector<int>> &S, int n) {
  vector<bool> barra(n, false);
  for(int i = 0; i < S.size(); i++) {
    for(int j = 0; j < S[i].size(); j++) {
      barra[S[i][j]] = true;
    }
  }

  vector<int> sla;
  for(int i = 0; i < n; i++) {
    if(!barra[i]) sla.push_back(i);
  }

  S.push_back(sla);
}


vector<vector<int>> MinCut(double** x, int n) {
  srand(time(NULL));
  vector<vector<int>> S;
  int node = 0;

  filterX(x, n);

  S = SingleMinCut(x, n, node);

  return S;
}


// SUPPORT FUNCTION //
void filterX(double** x, int n) {
  //filtrando NaN e valores menores que EPSILON
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(isnan(x[i][j]) || x[i][j] < EPSILON || i >= j) x[i][j] = 0;
    }
  }
}

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
