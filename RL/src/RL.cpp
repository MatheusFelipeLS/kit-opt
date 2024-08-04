#include "RL.h"

void addPenalty(vector<vector<double>> &cost, vector<double> &lambda) {
	for(int i = 1; i < cost.size(); i++) {
		for(int j = 1; j < cost[i].size(); j++) {
			cost[i][j] -= (lambda[i] + lambda[j]); 
		}
	}
}

void attLambda(vector<double> &subgradient, vector<double> &lambda, double UB, double LB, double epsilon) {
  int denominador = getSumOfTheSquareOfCoordinates(subgradient);
	for(int i = 0; i < lambda.size(); i++) {
		lambda[i] = lambda[i] + (epsilon * (UB - LB) * subgradient[i] / denominador);
	}
}

vector<int>degreeOfNodes(vector<pair<int, int>> &edges) {
  vector<int> degree(edges.size());
  for(int i = 0; i < edges.size(); i++) {
    degree[edges[i].first]++;
    degree[edges[i].second]++;
  }

  return degree;
}

vector<double> getSubgradient(vector<int> &degree) {
  int n = degree.size();
  vector<double> subgradient(n);

  for(int i = 1; i < n; i++) {
    subgradient[i] = 2 - degree[i];
  }

  return subgradient;
}

double getSumOfTheSquareOfCoordinates(vector<double> &subgradient) {
  double sum = 0;
  for(int i = 1; i < subgradient.size(); i++) {
    sum += (subgradient[i]*subgradient[i]);
  }
  return sum;
}

bool isValid(vector<int> &degree) {
  for(int i = 0; i < degree.size(); i++) {
    if(degree[i] != 2) return false;
  }

  return true;
}

void removePenalty(vector<vector<double>> &cost, vector<double> &lambda) {
	for(int i = 1; i < cost.size(); i++) {
		for(int j = 1; j < cost[i].size(); j++) {
			cost[i][j] += (lambda[i] + lambda[j]); 
		}
	}
}

vector<double> RL(vector<vector<double>> &cost, vector<double> &lambda, vector<pair<int, int>> &edges, double ub, double &lb, bool &feasible) {
  double epsilon = 1;
  int i = 0;
  double best_obj_value = 0;
  vector<pair<int, int>> best_edges;
  vector<double> best_lambda;
  bool stop = false;

  while((epsilon > epsilon_min) && !stop) {
    addPenalty(cost, lambda);

    Kruskal k = Kruskal((vvi&) cost);
    double obj_value = k.MST(lambda.size());
    vector<pair<int, int>> new_edges = k.getEdges();

    removePenalty(cost, lambda);

    vector<int> degree = degreeOfNodes(new_edges);

    stop = isValid(degree);
    if(stop) {
      best_obj_value = obj_value;
      best_lambda = lambda;
      best_edges = new_edges;
      continue;
    }

    vector<double> subgradient = getSubgradient(degree);
    attLambda(subgradient, lambda, ub, obj_value, epsilon);

    if(obj_value > best_obj_value) {
      best_obj_value = obj_value;
      best_lambda = lambda;
      best_edges = new_edges;
      i = 0;
    } else {
      i++;
      if(i >= i_max) {
        i = 0;
        epsilon /= 2;
      }
    }
  }

  feasible = stop;
  lb = best_obj_value;
  edges = best_edges;
  cin >> g;
  return best_lambda;
}

void printLambda(vector<double> &lambda) {
  cout << "Lambda: ";
  for(int i = 0; i < lambda.size(); i++) {
    cout << lambda[i] << " ";
  }
  cout << endl;
}

void printDegree(vector<int> &degree) {
  cout << "degree: ";
  for(int i = 0; i < degree.size(); i++) {
    cout << degree[i] << " ";
  }
  cout << endl;
}

void printMST(vector<pair<int, int>> &edges) {
  for(int i = 0; i < edges.size(); ++i) {
		cout << "f: " << edges[i].first << " s: " << edges[i].second << endl; 
	}
}