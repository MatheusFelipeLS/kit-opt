#include "BNB.h"

//escolhe o próximo nó
Node branchingStrategy(vector<Node> &tree, int strategy) {
	
	if(strategy < 1 || strategy > 2) {
		printf("Invalid branch strategy\n");
		exit(0);
	}

	Node n;

	if(strategy == 1) { //bfs
		n = tree[0];
		tree.erase(tree.begin());
		return n;
	} 

	n = tree[(tree.size()-1)];
	tree.pop_back();

	return n; 
}

void forbidArcs(vector<vector<double>> &cost, Node *node) {

	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = 99990000;
	}

}

void restartCostMatrix(vector<vector<double>> &cost, Node *node, Data *data) {

	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = data->getDistance(arc.first, arc.second);
	}

}

int highestDegreeNode(vector<int> &degree) {
	int highest = 0;

	for(int i = 1; i < degree.size(); i++) {
		
		if(degree[i] > degree[highest]) highest = i;
	
	}

	return highest;
}

vector<int> nextForbidden(int highest, Node *node) {
	vector<int> arcs;
	
	for(int i = 0; i < node->edges.size(); i++) {

		if(node->edges[i].first == highest) arcs.push_back(node->edges[i].second);
		
		else if(node->edges[i].second == highest) arcs.push_back(node->edges[i].first);
		
	}

	return arcs;
}

double BnB(Data *data, vector<vector<double>> &cost, double ub, int strategy) {
	Node root;
	double lower_bound = 0;
	root.lambda = vector<double>(data->getDimension());

	vector<Node> tree;
	priority_queue<Node, vector<Node>, CompareLB> treePq;
	
	if(strategy != 3) {
		tree.push_back(root);
	} else {
		treePq.push(root);
	}

	while(!tree.empty() || !treePq.empty()) {

		Node node;

		if(strategy != 3) node = branchingStrategy(tree, strategy);
		
		else {

			node = treePq.top();
			treePq.pop();

		}

		forbidArcs(cost, &node);

		node.lambda = RL(cost, node.lambda, node.edges, ub, node.lower_bound, node.feasible);

		restartCostMatrix(cost, &node, data);

		if(node.lower_bound >= ub) continue;

		else if(node.lower_bound > lower_bound) lower_bound = node.lower_bound;

		if(node.feasible) {

			if(node.lower_bound < ub) {
				ub = node.lower_bound;
			}
		
		} else {

			vector<int> degree = degreeOfNodes(node.edges);
			node.chosen = highestDegreeNode(degree);
			vector<int> arcs = nextForbidden(node.chosen, &node);

			for(int i = 0; i < arcs.size(); ++i) {
				Node n = node;

				pair<int, int> forbidden_arc = {
					node.chosen,
					arcs[i]
				};

				n.forbidden_arcs.push_back(forbidden_arc);

				forbidden_arc = {
					arcs[i],
					node.chosen
				};

				n.forbidden_arcs.push_back(forbidden_arc);

				if(strategy != 3) tree.push_back(n);

				else treePq.push(n);

			}
		}	
	}

	return ub;
}

void showNode(Node n) {
	cout << "Arcos proibidos: \n{ "; 
	for(int i = 0; i < n.forbidden_arcs.size(); ++i) {
		cout << "{" << n.forbidden_arcs[i].first << ", " << n.forbidden_arcs[i].second << "} "; 
	}
	cout << "}\nLower bound: " << n.lower_bound 
	<< "\nChosen: " << n.chosen
	<< "\nFeasible: " << n.feasible << endl;
}

void printCostMatrix(vector<vector<double>> &cost) {
	cout << "Cost: " << endl;
	for(int i = 0; i < cost.size(); i++) {
		for(int j = 0; j < cost[i].size(); j++) {
			if(cost[i][j] < 1000) cout << " ";
			if(cost[i][j] < 100) cout << " ";
			cout << cost[i][j] << " ";
		}
		cout << endl;
	}
}