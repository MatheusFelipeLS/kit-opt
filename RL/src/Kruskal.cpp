#include "Kruskal.h"

Kruskal::Kruskal(vvi dist){
	for(int i = 0; i < dist.size(); ++i){
		for(int j = 0; j < dist[i].size(); ++j){
			graph.push( make_pair(-dist[i][j], make_pair(i, j)) );
		}	
	}
}

void Kruskal::initDisjoint(int n){
	pset.resize(n);
	for (int i = 1; i < n; ++i){
		pset[i] = i;
	}
}

int Kruskal::findSet(int i){
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j){
	pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j){
	return (findSet(i) == findSet(j))? true:false;
}

vii Kruskal::getEdges(){
	return edges;
}

double Kruskal::MST(int nodes){
	initDisjoint(nodes);

	double cost = 0;
	vector<pair<int, double>> cost2zero;

	while(!graph.empty()){
		pair<double, ii> p = graph.top();
		graph.pop();

		if(p.second.first == 0) {
			cost2zero.push_back({p.second.second, p.first});
		} 
		else if(p.second.second == 0) {
			continue;
		} else {
			if(!isSameSet(p.second.first, p.second.second)){
				edges.push_back(make_pair(p.second.first, p.second.second));
				cost += (-p.first);
				unionSet(p.second.first, p.second.second);
			}
		}
	}

	pair<int, double> leftEdge = {1, 99999999};
	pair<int, double> rightEdge = {2, 99999999};

	for(int i = 0; i < cost2zero.size(); i++) {
    pair<int, double> c = cost2zero[i];
		if((-c.second) < leftEdge.second) {
			rightEdge.first = leftEdge.first;
			rightEdge.second = leftEdge.second;

			leftEdge.first = c.first;
			leftEdge.second = (-c.second);

		} else if((-c.second) < rightEdge.second) {
			rightEdge.first = c.first;
			rightEdge.second = (-c.second);
		}

	}

	pair<int, int> e = {leftEdge.first, 0};
	edges.push_back(e);

	e = {0, rightEdge.first};
	edges.push_back(e);
	cost += (leftEdge.second + rightEdge.second);

	return cost;
}