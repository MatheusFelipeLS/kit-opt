#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#include "data.h"
#include "RL.h"

struct Node {
	int chosen; // indice do nó com maior grau
	double lower_bound; 
	bool feasible; // indica se a solucao do dual é viável
	vector<double> lambda; // penalizador lambda
	vector<pair<int, int>> edges; //nós da árvore MS1TP
	vector<pair<int, int>> forbidden_arcs; // lista de arcos proibidos do nó
};

struct CompareLB {
    bool operator()(Node const& n1, Node const& n2) {
        return n1.lower_bound > n2.lower_bound;
    }
};

double BnB(Data *data, vector<vector<double>> &cost_matrix, double ub, int strategy);

void showNode(Node n);
Node branchingStrategy(vector<Node> &tree, int strategy);
vector<pair<int, int>> getSolutionKruskal(Node *node, Data *data, vector<vector<double>> &cost_matrix, double UB);
void forbidArcs(vector<vector<double>> &cost_matrix, Node *node);
void restartCostMatrix(vector<vector<double>> &cost_matrix, Node *node, Data *data);

void printCostMatrix(vector<vector<double>> &cost);
double calcular_dist_total_rl(vector<pair<int, int>> s, Data *data);