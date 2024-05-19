#include <iostream>
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

//escolhe o próximo nó
list<Node>::iterator branchingStrategy(list<Node> &tree, int strategy) {
	if(strategy > 3 || strategy < 1) {
		printf("Invalid branch strategy\n");
		exit(0);
	}

	if(strategy == 1) { //bfs
		return tree.begin();
	} else if(strategy == 2) { //dfs
		return tree.end(); 
	}
	
	//min lower bound
	//ainda vendo
	return tree.end(); 
}


void createCostMatrix(Node node) {

}


vector<vector<int>> getSubtour(hungarian_problem_t *p) {
	vector<vector<int>> sequence; //guarda as subsequences geradas pelo húngaro
	vector<int> linhas; //vetor que informa qual linha ainda não foi percorrida
	int count = 0; //contador que informa quantas linhas foram percorridas (condição de parada da função)
	int i = 0; //variável que itera entre linhas
	int a = 0; //o valor dessa variável é sempre igual a quantidade de soubtours - 1
	int N = p->num_cols; //quantidade de linhas (e colunas) na matriz
	int **matriz = p->assignment; //matriz que será percorrida
		
	sequence.push_back({0}); //adiciona o valor 0 pois a linha 0 será a primeira a ser percorrida

	//inicia o vetor de linhas, apenas com o 1º valor igual a -1 (valor = -1 => linha já foi percorrida)
	for(int x = 0; x < N+1; ++x) {
		linhas.push_back(x-1);
	}
		
	//condição para saber se deve parar
	while(++count <= N) {
		//for para percorrer as colunas
		for(int j = 0; j < N; ++j) {
			//verifica se o trabalhador "i está fazendo o trabalho j". Nesse caso, se a cidade i está ligado a j
			if(matriz[i][j]) {
				//adiciona a cidade na subsequence
		        sequence[a].push_back(j);
				//se o valor adicionado é igual ao primeiro, significa que o subtour foi fechado e deve-se começar outro
    			if((j) == sequence[a][0]) {
    			    ++a;
					//procura uma linha disponível para ser percorrida
    			    for(auto s : linhas) {
    			        if(s >= 0) {
    			            linhas[s+1] = -1; //torna a linha que será lida em inválida
    			            i = s+1; //coloca o iterador das linhas na próxima linha percorrida
    			            break;
    			        }
    			    }
    			    if(count < N) sequence.push_back({i}); //inicia a nova subsequence
    			} else {
    			    i = j; //simplesmente vai para nova linha a ser percorrida
    			    linhas[j] = -1; //torna a linha que será percorrida em inválida, para evitar que ela seja percorrida de novo
    			}
    			break;
			}
		}
	}

	return sequence;
}


vector<vector<int>> getSolutionHungarian(list<Node>::iterator node, Data *data, double **cost) {
	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

	node->lower_bound = hungarian_solve(&p);
	
	vector<vector<int>> sequence = getSubtour(&p);

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


void forbidArcs(double **cost, list<Node>::iterator node) {
	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = 99999999;
	}
}


void restartCostMatrix(double **cost, list<Node>::iterator node, Data *data) {
	for(int i = 0; i < node->forbidden_arcs.size(); ++i) {
		pair<int, int> arc = node->forbidden_arcs[i];
		cost[arc.first][arc.second] = data->getDistance(arc.first, arc.second);
	}	
}


int main(int argc, char** argv) {

	Data * data = new Data(argc, argv[1]);
	data->readData();

	// cout << "Choose the branching strategy:" << endl
	// << "[1] - DFS" << endl
	// << "[2] - BFS" << endl
	// << "[3] - Minimum lower bound" << endl;

	int strategy = 1;
	// cin >> strategy;

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	//próximas 6 linhas podem ser deletadas 
	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

	double lower_bound = hungarian_solve(&p);
	cout << "Obj. value: " << lower_bound << endl;

	// cout << "Assignment" << endl;
	// hungarian_print_assignment(&p);

	double upper_bound = ILS(data, 50);

	cout << "upper_bound: " << upper_bound << endl;

	Node root;
	root.lower_bound = lower_bound; //linha desnecessária (dentro do for já consegue isso)
	
	Node solution;
	solution.lower_bound = 99999999;

	list<Node> tree;
	tree.push_back(root);

	cout << "Antes do while" << endl;
	while(!tree.empty()) {
		auto node = branchingStrategy(tree, strategy);
		forbidArcs(cost, node);

		node->subtour = getSolutionHungarian(node, data, cost); //subtours, chosen, feasible, lower bound

		if(node->lower_bound >= upper_bound) {
			tree.erase(node);
			continue;
		}

		if(node->feasible) {
			upper_bound = node->lower_bound;
		} else {
			for(int i = 0; i < node->subtour[node->chosen].size()-1; ++i) {
				Node n;
				n.forbidden_arcs = node->forbidden_arcs;
				
				pair<int, int> forbidden_arc = {
					node->subtour[node->chosen][i],
					node->subtour[node->chosen][i+1]
				};

				n.forbidden_arcs.push_back(forbidden_arc);
				tree.push_back(n);
			}
		}

		restartCostMatrix(cost, node, data);

		tree.erase(node);
	}

	cout << "Depois do while" << endl;
	cout << "upper_bound: " << upper_bound << endl;

	
///////////////////////////// NÃO MEXER ///////////////////////////////////////////
	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}