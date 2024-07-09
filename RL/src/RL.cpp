#include "RL.h"

void addPenalty(vector<vector<double>> &cost, vector<double> lambda) {
	for(int i = 1; i < cost.size(); i++) {
		for(int j = 1; j < cost[i].size(); j++) {
			cost[i][j] += lambda[i] + lambda[j]; 
		}
	}
}

void removePenalty(vector<vector<double>> &cost, vector<double> lambda) {
	for(int i = 1; i < cost.size(); i++) {
		for(int j = 1; j < cost[i].size(); j++) {
			cost[i][j] -= lambda[i] + lambda[j]; 
		}
	}
}

vector<int> degreeOfNodes(int n, vector<pair<int, int>> &edges) {
  vector<int> degree(n);

	//definindo o grau de cada nó
	for(int i = 0; i < edges.size(); i++) {
		degree[edges[i].first]++;
		degree[edges[i].second]++;
	}

	return degree;
}

vector<double> getSubgradient(int n, vector<pair<int, int>> &edges) {
  vector<double> subgradient(n);
  vector<int> degree = degreeOfNodes(n, edges);

  for(int i = 1; i < subgradient.size(); i++) {
    subgradient[i] = 2 - degree[i];
  }

  return subgradient;
}

int getSumOfTheSquareOfCoordinates(vector<double> subgradient) {
  double sum = 0;
  for(int i = 1; i < subgradient.size(); i++) {
    sum += (subgradient[i]*subgradient[i]);
  }
  return sum;
}

void attLambda(vector<double> &subgradient, vector<double> &lambda, double UB, double LB, double epsilon) {
  int sum = getSumOfTheSquareOfCoordinates(subgradient);
	for(int i = 0; i < lambda.size(); i++) {
		lambda[i] = lambda[i] + (epsilon * (UB - LB) * subgradient[i] / sum);
	}
}

bool isTour(vector<pair<int, int>> &edges) {
	for(int i = 0; i < degree.size(); i++) {
		if(degree[i] != 2) return false; 
	}
	return true;
}

double MS1TP(vector<vector<double>> &cost, vector<pair<int, int>> &edges) {
	pair<int, int> leftEdge = {1, 99999999};
	pair<int, int> rightEdge = {2, 99999999};
	for(int i = 1; i < cost[0].size(); i++) {
		if(cost[0][i] < leftEdge.second) {
			rightEdge.first = leftEdge.first;
			rightEdge.second = leftEdge.second;
			leftEdge.first = i;
			leftEdge.second = cost[0][i];
		} else if(cost[0][i] < rightEdge.second) {
			rightEdge.first = i;
			rightEdge.second = cost[0][i];
		}
	}

	pair<int, int> e = {leftEdge.first, 0};
	edges.push_back(e);
	e = {0, rightEdge.first};
	edges.push_back(e);

  return (leftEdge.second + rightEdge.second);
}

bool verifyTheorem2(vector<double> &subgradient, vector<double> lambda) {
  for(int i = 0; i < subgradient.size(); i++) {
    if(subgradient[i] || subgradient[i]*lambda[i]) {
      return false;
    }
  }
  return true;
}

vector<double> RL(data *data, vector<vector<double>> &cost) {
  double ub = ILS(data, 50);
  vector<double> lambda(data->getDimension());
  double epsilon = 1;
  double epsilon_min = 0.10;
  int k = 0;
  int k_max = 10;
  double best_obj_value = 0;
  double theorem2 = false;

  while(epsilon > epsilon_min && !theorem2) {
    addPenalty(cost, lambda);

    Kruskal k = Kruskal((vvi&) cost);
    double obj_value = k.MST(data->getDimension());
    vector<pair<int, int>> edges = k.getEdges();
    obj_value += MS1TP(cost, edges);

    removePenalty(cost, lambda);

    vector<double> subgradient = getSubgradient(data->getDimension(), edges);
    attLambda(subgradient, lambda, ub, obj_value, epsilon);
    theorem2 = verifyTheorem2();
    if(obj_value > best_obj_value) {
      best_obj_value = obj_value;
    } else {
      k++;
      if(k >= k_max) {
        k = 0;
        epsilon /= 2;
      }
    }
  }

  return lambda;
}