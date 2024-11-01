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


int nodeOutsideSubtour(vector<vector<int>> &subtours) {
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

void showSubtours(vector<vector<int>> &subtours) {
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      cout << subtours[i][j] << " ";
    }
    cout << endl;
  }
}


vector<vector<int>> MinCut(double** x, int n) {
  vector<vector<int>> subtours;
  vector<int> S;
  int node = 0;
  bool check = true;

  filterX(x, n);

  while(check) {
    check = !allNodesOnSubtours(n, subtours);
    S = SingleMinCut(x, n, node);
    subtours.push_back(S);
    node = nodeOutsideSubtour(subtours);
  }

  return subtours;
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
