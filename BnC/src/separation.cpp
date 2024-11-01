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


void showSubtours(vector<vector<int>> &subtours) {
  for(int i = 0; i < subtours.size(); i++) {
    for(int j = 0; j < subtours[i].size(); j++) {
      cout << subtours[i][j] << " ";
    }
    cout << endl;
  }
}




vector<vector<int>> MinCut(double** x, int n) {
  srand(time(NULL));
  vector<vector<int>> S;
  int node = 0;

  filterX(x, n);

  S = SingleMinCut(x, n, node);

  cout << "eu\n";
  showSubtours(S);
  int t;
  cin >> t;

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
