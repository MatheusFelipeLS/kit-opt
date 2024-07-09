#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#include "data.h"
// #include "BNB.h"
#include "RL.h"
#include "Kruskal.h"

int main(int argc, char** argv) {
	Data * data = new Data(argc, argv[1]);
	data->readData();

	// cout << "Choose the branching strategy:" << endl
	// << "[1] - BFS" << endl
	// << "[2] - DFS" << endl
	// << "[3] - Lower Bound" << endl;

	int strategy = 2;
	// cin >> strategy;
	
	vvi cost_matrix;
	for (int i = 0; i < data->getDimension(); i++){
		cost_matrix.push_back({});
		for (int j = 0; j < data->getDimension(); j++){ 
			cost_matrix[i].push_back(data->getDistance(i, j));
		}
	}

	// double cost = BnB(data, cost_matrix, strategy);
	vector<string> strategies = {"BFS", "DFS", "Lower Bound"};
	cout << strategies[strategy-1] << " - Cost: " << cost << endl;

	delete data;

	return 0;
}