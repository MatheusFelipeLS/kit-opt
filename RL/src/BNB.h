// #include <iostream>
// #include <vector>
// #include <queue>
// using namespace std;

// #include "data.h"
// #include "ILS.h"
// #include "Kruskal.h"

// struct Node {
// 	vector<pair<int, int>> forbidden_arcs; // lista de arcos proibidos do nó
// 	vector<pair<int, int>> edges; //arcos dados pelo MST
// 	vector<int> lambda; // vetor de penalidades
// 	vector<int> degree; // grau de cada nó
// 	vector<int> next_forbidden;
// 	double lower_bound; // custo da solução do dual
// 	int chosen; // indice do nó com maior grau
// 	bool feasible; // indica se a solucao do dual é viável
// };

// struct CompareLB {
//     bool operator()(Node const& n1, Node const& n2) {
//         return n1.lower_bound > n2.lower_bound;
//     }
// };

// void showNode(Node n);
// Node branchingStrategy(vector<Node> &tree, int strategy);
// vector<pair<int, int>> getSolutionKruskal(Node *node, Data *data, vector<vector<double>> &cost_matrix, double UB);
// void forbidArcs(vector<vector<double>> &cost_matrix, Node *node);
// void restartCostMatrix(vector<vector<double>> &cost_matrix, Node *node, Data *data);
// double BnB(Data *data, vector<vector<double>> &cost_matrix, int strategy);
