#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#include "data.h"
#include "hungarian.h"
#include "ILS.h"

struct Node {
	vector<pair<int, int>> forbidden_arcs; // lista de arcos proibidos do no
	vector<vector<int>> subtour; // conjunto de subtours da solucao
	double lower_bound; // cost total da solucao do algoritmo hungaro
	int chosen; // indice do menor subtour
	bool feasible; // indica se a solucao do AP_TSP e viavel
};

void showNode(Node n) {
	cout << "Arcos proibidos: \n{ "; 
	for(int i = 0; i < n.forbidden_arcs.size(); ++i) {
		cout << "{" << n.forbidden_arcs[i].first << ", " << n.forbidden_arcs[i].second << "} "; 
	}
	cout << "}\nSubtours:\n{";
	for(int i = 0; i < n.subtour.size(); ++i) {
		cout << "{";
		int j;
		for(j = 0; j < n.subtour[i].size()-1; ++j) {
			cout << n.subtour[i][j] << ", ";
		}
		cout << n.subtour[i][j] << "} ";
	}
	cout << "}\nLower bound: " << n.lower_bound 
	<< "\nChosen: " << n.chosen
	<< "\nFeasible: " << n.feasible << endl;
}

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

vector<vector<int>> getSubtour(hungarian_problem_t *p) {
	vector<vector<int>> sequencia; //guarda as subsequencias geradas pelo húngaro
	vector<int> linhas; //vetor que informa qual linha ainda não foi percorrida
	int count = 0; //contador que informa quantas linhas foram percorridas (condição de parada da função)
	int i = 0; //variável que itera entre linhas
	int a = 0; //índice do subtour atual
	int N = p->num_cols; //quantidade de linhas (e colunas) na matriz
	int **matriz = p->assignment; //matriz que será percorrida
		
	sequencia.push_back({0}); //adiciona o valor 0 pois a linha 0 será a primeira a ser percorrida

	//inicia o vetor de linhas, apenas com o 1º valor igual a -1 (valor = -1 => linha já foi percorrida)
	for(int x = 0; x < N; ++x) {
		linhas.push_back(x-1);
	}
		
	//condição para saber se deve parar
	while(++count <= N) {
		//for para percorrer as colunas
		for(int j = 0; j < N; ++j) {
			//verifica se o trabalhador "i está fazendo o trabalho j". Nesse caso, se a cidade i está ligado a j
			if(matriz[i][j]) {
				//adiciona a cidade na subsequencia
		        sequencia[a].push_back(j);
				//se o valor adicionado é igual ao primeiro, significa que o subtour foi fechado e deve-se começar outro
    			if((j) == sequencia[a][0]) {
    			    ++a;
					//procura uma linha disponível para ser percorrida
    			    for(auto s : linhas) {
    			        if(s >= 0) {
    			            linhas[s+1] = -1; //torna a linha que será lida em inválida
    			            i = s+1; //coloca o iterador das linhas na próxima linha percorrida
    			            break;
    			        }
    			    }
    			    if(count < N) sequencia.push_back({i}); //inicia a nova subsequencia
    			} else {
    			    i = j; //simplesmente vai para nova linha a ser percorrida
    			    linhas[j] = -1; //torna a linha que será percorrida em inválida, para evitar que ela seja percorrida de novo
    			}
    			break;
			}
		}
	}
	return sequencia;
}


vector<vector<int>> getSolutionHungarian(Node *node, Data *data, double **cost, double UB) {
	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

	node->lower_bound = hungarian_solve(&p);
	vector<vector<int>> sequence = getSubtour(&p);

	hungarian_free(&p);

	node->chosen = 0;
	if(sequence.size() == 1) {
		node->feasible = true;
	} else {
		node->feasible = false;
		int tam_min = sequence[0].size();
		for(int i = 1; i < sequence.size(); ++i) {
			if(sequence[i].size() < tam_min) {
				tam_min = sequence[i].size();
				node->chosen = i;
			}
		}
	}

	return sequence;
}


void forbidArcs(double **cost, Node *node) {
	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = 99999999;
	}
}


void restartCostMatrix(double **cost, Node *node, Data *data) {
	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = data->getDistance(arc.first, arc.second);
	}
}


struct CompareLB {
    bool operator()(Node const& n1, Node const& n2) {
        return n1.lower_bound > n2.lower_bound;
    }
};


double BnB(Data *data, double** cost, int strategy) {
	double upper_bound = ILS(data, 50);

	Node root;
	vector<Node> tree;
	priority_queue<Node, vector<Node>, CompareLB> treePq;
	
	if(strategy != 3) {
		tree.push_back(root);
	} else {
		treePq.push(root);
	}

	while(!tree.empty() || !treePq.empty()) {
		Node node;
		if(strategy != 3) {
			node = branchingStrategy(tree, strategy);
		} else {
			node = treePq.top();
			treePq.pop();
		}

		forbidArcs(cost, &node);

		node.subtour = getSolutionHungarian(&node, data, cost, upper_bound); //subtours, chosen, feasible, lower bound

		restartCostMatrix(cost, &node, data);

		if(node.lower_bound >= upper_bound) {
			continue;
		}

		if(node.feasible) {
			upper_bound = node.lower_bound;
		} else {
			for(int i = 0; i < node.subtour[node.chosen].size()-1; ++i) {
				Node n;
				n.lower_bound = node.lower_bound;
				n.forbidden_arcs = node.forbidden_arcs;
				pair<int, int> forbidden_arc = {
					node.subtour[node.chosen][i],
					node.subtour[node.chosen][i+1]
				};
				n.forbidden_arcs.push_back(forbidden_arc);
				if(strategy != 3) tree.push_back(n);
				else treePq.push(n);
			}
		}	
	}
	return upper_bound;
}


int main(int argc, char** argv) {
	Data * data = new Data(argc, argv[1]);
	data->readData();

	cout << "Choose the branching strategy:" << endl
	<< "[1] - BFS" << endl
	<< "[2] - DFS" << endl
	<< "[3] - Lower Bound" << endl;

	int strategy;
	cin >> strategy;

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){ 
			cost[i][j] = data->getDistance(i,j);
		}
	}
	
	vector<string> strategies = {"BFS", "DFS", "Lower Bound"};
	double upper_bound = BnB(data, cost, strategy);
	cout << strategies[strategy-1] << " - upper_bound: " << upper_bound << endl;

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}