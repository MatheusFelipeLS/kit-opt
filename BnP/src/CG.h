#include <iostream>
#include <ilcplex/ilocplex.h>
#include <vector>

#include "Data.h"

using namespace std;

struct lambdaInfo {
  vector<double> lambdaValue;
  vector<vector<int>> lambdaItems; 
};

struct Node {
	vector<pair<int, int>> together;
	vector<pair<int, int>> separated; 
  double qtBins;
};

lambdaInfo CG(Data *data, Node &node);
