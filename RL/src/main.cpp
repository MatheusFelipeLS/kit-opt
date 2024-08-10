#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#include "data.h"
#include "BNB.h"
#include "ILS.h"

double calcular_dist_total_rl(vector<pair<int, int>> s, Data *data) {
  double cost = 0;
  for(int i = 0; i < s.size(); i++) {
    cost += data->getDistance(s[i].first, s[i].second);
  }
  return cost;
}

int main(int argc, char** argv) {
	Data * data = new Data(argc, argv[1]);
	data->readData();

/*
// Para caso queira testar uma única estratégia 
	cout << "Choose the branching strategy:" << endl
	<< "[1] - BFS" << endl
	<< "[2] - DFS" << endl
	<< "[3] - Lower Bound" << endl;

	int strategy;
	cin >> strategy;
*/
	
	vector<vector<double>> cost_matrix;
	for (int i = 0; i < data->getDimension(); i++){
		cost_matrix.push_back({});
		for (int j = 0; j < data->getDimension(); j++){ 
			cost_matrix[i].push_back(data->getDistance(i, j));
		}
	}
	
	double ub = ILS(data, 50) * 1.5;
	double guardaUB = ub;

	for(int strategy = 1; strategy <= 3; strategy++) {
		ub = guardaUB;
		double cost = BnB(data, cost_matrix,  ub, strategy);

		if(ub < cost) cost = ub;

		vector<string> strategies = {"BFS", "DFS", "Lower Bound"};
		cout << strategies[strategy-1] << " - Cost: " << cost << endl;
	}

	delete data;

	return 0;
}
