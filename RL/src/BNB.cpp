// #include "BNB.h"

// void showNode(Node n) {
// 	cout << "Arcos proibidos: \n{ "; 
// 	for(int i = 0; i < n.forbidden_arcs.size(); ++i) {
// 		cout << "{" << n.forbidden_arcs[i].first << ", " << n.forbidden_arcs[i].second << "} "; 
// 	}
// 	cout << "}\nLower bound: " << n.lower_bound 
// 	<< "\nChosen: " << n.chosen
// 	<< "\nFeasible: " << n.feasible << endl;
// }

// //escolhe o próximo nó
// Node branchingStrategy(vector<Node> &tree, int strategy) {
// 	if(strategy < 1 || strategy > 2) {
// 		printf("Invalid branch strategy\n");
// 		exit(0);
// 	}

// 	Node n;
// 	if(strategy == 1) { //bfs
// 		n = tree[0];
// 		tree.erase(tree.begin());
// 		return n;
// 	} 

// 	n = tree[(tree.size()-1)];
// 	tree.pop_back();
// 	return n; 
// }

// vector<int> degreeOfNodes(vector<pair<int, int>> &edges, int n) {
// 	vector<int> degree(n);

// 	//definindo o grau de cada nó
// 	for(int i = 0; i < edges.size(); i++) {
// 		degree[edges[i].first]++;
// 		degree[edges[i].second]++;
// 	}

// 	return degree
// }

// vector<pair<int, int>> getSolutionKruskal(Node *node, Data *data, vector<vector<double>> &cost, double UB) {
// 	Kruskal k = Kruskal((vvi&) cost);

// 	node->lower_bound = k.MST(data->getDimension());
// 	vector<pair<int, int>> edges = k.getEdges();
// 	node->degree = degreeOfNodes(edges, data.getDimension());

// 	node->chosen = 1;

// 	//definindo o nó de maior grau
// 	for(int i = 2; i < node->degree.size(); i++) {
// 		if(node->degree[i] > node->degree[node->chosen]) node->chosen = i;
// 	}

// 	//salvando os próximos nós a serem proibidos caso essa solução não seja um tour
// 	for(int i = 0; i < edges.size(); ++i) {
// 		if(edges[i].first == node->chosen) node->next_forbidden.push_back(edges[i].second);
// 		else if(edges[i].second == node->chosen) node->next_forbidden.push_back(edges[i].first);
// 	}
// }

// void forbidArcs(vector<vector<double>> &cost, Node *node) {
// 	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
// 		pair<int, int> arc = node->forbidden_arcs[i];
// 		cost[arc.first][arc.second] = 99999999;
// 	}
// }

// void restartCostMatrix(vector<vector<double>> &cost, Node *node, Data *data) {
// 	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
// 		pair<int, int> arc = node->forbidden_arcs[i];
// 		cost[arc.first][arc.second] = data->getDistance(arc.first, arc.second);
// 	}
// }

// void MS1TP(vector<vector<double>> &cost, vector<pair<int, int>> &edges) {
// 	pair<int, int> leftEdge = {1, 99999999};
// 	pair<int, int> rightEdge = {2, 99999999};
// 	for(int i = 1; i < cost[0].size(); i++) {
// 		if(cost[0][i] < leftEdge.second) {
// 			rightEdge.first = leftEdge.first;
// 			rightEdge.second = leftEdge.second;
// 			leftEdge.first = i;
// 			leftEdge.second = cost[0][i];
// 		} else if(cost[0][i] < rightEdge.second) {
// 			rightEdge.first = i;
// 			rightEdge.second = cost[0][i];
// 		}
// 	}

// 	pair<int, int> e = {leftEdge.first, 0};
// 	edges.push_back(e);
// 	e = {0, rightEdge.first};
// 	edges.push_back(e);
// }

// bool isTour(vector<pair<int, int>> &edges) {
// 	for(int i = 0; i < degree.size(); i++) {
// 		if(degree[i] != 2) return false; 
// 	}
// 	return true;
// }

// vector<int> startLambda(n) {
// 	vector<int> v(n);
// 	return v;
// }

// void addPenalty(vector<vector<double>> &cost, Node *node) {
// 	for(int i = 1; i < cost.size(); i++) {
// 		for(int j = 1; j < cost[i].size(); j++) {
// 			cost[i][j] += node->lambda[i] + node->lambda[j]; 
// 		}
// 	}
// }

// void removePenalty(vector<vector<double>> &cost, Node *node) {
// 	for(int i = 1; i < cost.size(); i++) {
// 		for(int j = 1; j < cost[i].size(); j++) {
// 			cost[i][j] -= node->lambda[i] + node->lambda[j]; 
// 		}
// 	}
// }

// void attLambda(Node *node, double UB, double epsilon) {
// 	vector<int> subgradiente = getSubgradiente();
// 	for(int i = 0; i < node->lambda.size(); i++) {
// 		node->lambda[i] = node->lambda[i] + epsilon * (UB - node.lower_bound) / ;
// 	}
// }

// double BnB(Data *data, vector<vector<double>> &cost, int strategy) {
// 	double upper_bound = ILS(data, 50);
// 	Node root;
// 	vector<Node> tree;
// 	priority_queue<Node, vector<Node>, CompareLB> treePq;

// 	root.lambda = startLambda(data->getDimension());
	
// 	if(strategy != 3) {
// 		tree.push_back(root);
// 	} else {
// 		treePq.push(root);
// 	}

// 	while(!tree.empty() || !treePq.empty()) {
// 		Node node;
// 		if(strategy != 3) {
// 			node = branchingStrategy(tree, strategy);
// 		} else {
// 			node = treePq.top();
// 			treePq.pop();
// 		}

// 		addPenalty(cost, &node);
// 		forbidArcs(cost, &node);

// 		vector<pair<int, int>> edges = getSolutionKruskal(&node, data, cost, upper_bound); //next_forbidden, chosen, feasible, lower_bound

// 		MS1TP(cost, edges);

// 		node.feasible = isTour(edges); 

// 		restartCostMatrix(cost, &node, data);

// 		if(node.lower_bound >= upper_bound) {
// 			continue;
// 		}

// 		if(node.feasible) {
// 			upper_bound = node.lower_bound;
// 		} else {
// 			for(int i = 0; i < node.next_forbidden.size(); ++i) {
// 				Node n;
// 				n.lower_bound = node.lower_bound;
// 				n.forbidden_arcs = node.forbidden_arcs;
// 				pair<int, int> forbidden_arc = {
// 					node.chosen,
// 					node.next_forbidden[i]
// 				};
// 				n.forbidden_arcs.push_back(forbidden_arc);
// 				if(strategy != 3) tree.push_back(n);
// 				else treePq.push(n);
// 			}
// 		}	
// 	}

// 	return upper_bound;
// }
