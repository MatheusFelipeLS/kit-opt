#include <iostream>
#include <vector>
#include <ilcplex/ilocplex.h>

using namespace std;

#include "Data.h"
#define EPSILON 1e-4

struct Node {
	vector<pair<int, int>> together;
	vector<pair<int, int>> separated; 
  double qtBins;
};

int BnB(Data *data, int strategy);
Node branchingStrategy(vector<Node> &tree, int strategy);
