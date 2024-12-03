#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#include "Data.h"
#include "CG.h"

double BnB(Data *data, int strategy);

void showNode(Node n);
Node branchingStrategy(vector<Node> &tree, int strategy);
vector<pair<int, int>> getSolutionKruskal(Node *node, Data *data, vector<vector<double>> &cost_matrix, double UB);
void forbidArcs(vector<vector<double>> &cost_matrix, Node *node);
void restartCostMatrix(vector<vector<double>> &cost_matrix, Node *node, Data *data);

void printCostMatrix(vector<vector<double>> &cost);