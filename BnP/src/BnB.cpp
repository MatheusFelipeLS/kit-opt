#include "BnB.h"

//escolhe o próximo nó
Node branchingStrategy(vector<Node> &tree, int strategy) {
	
	if(strategy < 1 || strategy > 2) {
		printf("Invalid branch strategy\n");
		exit(0);
	}

	Node n;

	if(strategy == 1) { //bfs
		n = tree[0];
		// swap(tree[0], tree[tree.size()-1]);
		// tree.pop_back();
		tree.erase(tree.begin());
		return n;
	} 

	n = tree[(tree.size()-1)];
	tree.pop_back();

	return n; 
}


void createZ(vector<vector<double>> &Z, lambdaInfo &L) {
	for(int i = 0; i < Z.size(); i++) {
		for(int j = 0; j < Z[i].size(); j++) {
			Z[i][j] = 0;
		}
	}

	int t;
	for(int i = 0; i < L.lambdaItems.size(); i++) {
		for(int j = 0; j < L.lambdaItems[i].size(); j++) {
			for(int k = j+1; k < L.lambdaItems[i].size(); k++) {
				Z[L.lambdaItems[i][j]][L.lambdaItems[i][k]] += L.lambdaValue[i];
				Z[L.lambdaItems[i][k]][L.lambdaItems[i][j]] += L.lambdaValue[i];
			}
		}
	}
}

pair<int, int> getMostFractional(vector<vector<double>> &Z) {
	pair<int, int> items;
	double value = 1;

	for(int i = 0; i < Z.size(); i++) {
		for(int j = i+1; j < Z[i].size(); j++) {
			double delta = abs(Z[i][j] - 0.5);
			if(delta < value) {
				items = {i, j};
				value = delta;
			}
		}
	}

	cout << "value: " << value << endl;
	// int t;
	// cin >> t;
	if(value >= 0.5 - 1e-5) {
		return make_pair(-1, -1);
	}

	return items;
}

double BnB(Data *data, int strategy) {
	Node root;
	vector<Node> tree;
	double qtBins = data->getNItems();

	vector<vector<double>> Z(data->getNItems(), vector<double>(data->getNItems()));
	
	tree.push_back(root);

	while(!tree.empty()) {

		Node node;

		node = branchingStrategy(tree, strategy);
	
		// restrições para juntar e separar os nós

		lambdaInfo L = CG(data, node); // chama o CG

		// tira as restrições

		// cria a matriz Z a partir dos lambdas e dos padrões
		createZ(Z, L);
		
		// encontra o mais fracionário
		pair<int, int> items = getMostFractional(Z);

		if(items.first == -1) {
			if(node.qtBins >= qtBins) continue;

			qtBins = node.qtBins;

		} else {
			// cria dois filhos juntando e separando
			Node n = node;
			
			n.separated.push_back(items);
			tree.push_back(n);	
			n.separated.pop_back();

			n.together.push_back(items);
			tree.push_back(n);
		}
	}

	return qtBins;
}
