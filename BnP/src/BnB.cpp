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
		tree.erase(tree.begin());
		return n;
	} 

	n = tree[(tree.size()-1)];
	tree.pop_back();

	return n; 
}


int BnB(Data *data, int strategy) {

// instance data

	int qtBins = data->getNItems(); // returned value

	int n = data->getNItems(); // instance size
	int capacity = data->getBinCapacity();

	vector<int> weight(n);
	for(int i = 0; i < n; i++) {
		weight[i] = data->getItemWeight(i);
	}


	const double M = 1e6;

// variables related to branch and bound

	Node root;
	vector<Node> tree; 

	vector<vector<bool>> realColumn;
	vector<vector<int>> columnValues;


// initing master problem

	IloEnv env;
	IloModel master_model(env);
	IloNumVarArray lambda(env, n, 0, IloInfinity);
	IloRangeArray partition_constraint(env);
	IloExpr sum_obj(env);

	for (int i = 0; i < n; i++) {
		char var_name[50];
		sprintf(var_name, "y%d", i);

		lambda[i].setName(var_name);
		sum_obj += M * lambda[i];
		partition_constraint.add(lambda[i] == 1);

		columnValues.push_back({i});

		realColumn.push_back(vector<bool>(n, false));
		realColumn[i][i] = true;
	}

	master_model.add(partition_constraint);

	IloObjective master_objective = IloMinimize(env, sum_obj);
	master_model.add(master_objective);


// start the branch and bound method

	tree.push_back(root);
	
	int lambda_counter = n;
	while(!tree.empty()) {

		Node node;

		node = branchingStrategy(tree, strategy); 
	
// applying constraints on master model

		vector<int> forbiddenLambdas;

		for(int i = 0; i < realColumn.size(); i++) {
			
			for(int j = 0; j < node.together.size(); j++) {
				
				if(realColumn[i][ node.together[j].first ] != realColumn[i][ node.together[j].second ]) {
					forbiddenLambdas.push_back(i);
					lambda[i].setUB(0.0);
					j = -1;
					i++;
					if(i == realColumn.size()) break;
				}

			}

			if(i == realColumn.size()) break;

			for(int j = 0; j < node.separated.size(); j++) {
				
				if(realColumn[i][ node.separated[j].first ] && realColumn[i][ node.separated[j].second ]) {
					forbiddenLambdas.push_back(i);
					lambda[i].setUB(0.0);
					break;
				}

			}

		}

		// solving pricing problem
		IloCplex rmp(master_model);
		rmp.setOut(env.getNullStream()); // disables CPLEX log

		while(true) {
			
			rmp.solve();

			if(rmp.getCplexStatus() == IloCplex::Infeasible) {
				break;
			}
			
			// Get the dual variables
			IloNumArray pi(env, n);

			rmp.getDuals(pi, partition_constraint);

			// Build and solve the pricing problem

			IloEnv pricing_env;
			IloModel pricing_model(pricing_env);

			IloNumVarArray x(pricing_env, n, 0, 1, ILOINT);

			IloExpr constraint(pricing_env);
			IloExpr objective(pricing_env);

			for(int i = 0; i < n; i++) {
				constraint += (weight[i] * x[i]);
				objective -= (x[i] * pi[i]);
			}

			pricing_model.add( constraint <= capacity );
			pricing_model.add(IloMinimize(pricing_env, 1 + objective));

			// pricing constraints (IMPORTANT PART OF ALGORITHM)

			for(int i = 0; i < node.together.size(); i++) {
				pricing_model.add( x[node.together[i].first] == x[node.together[i].second] );
			}

			for(int i = 0; i < node.separated.size(); i++) {
				pricing_model.add( x[node.separated[i].first] + x[node.separated[i].second] <= 1 );
			}

			///////////////////////////////////////////////////

			IloCplex pricing_problem(pricing_env);
			pricing_problem.extract(pricing_model);
			
			pricing_problem.setOut(pricing_env.getNullStream());
			pricing_problem.solve();

			if (pricing_problem.getObjValue() < -EPSILON) {

				IloNumArray entering_col(env, n);

				pricing_problem.getValues(x, entering_col);

				columnValues.push_back({});
				realColumn.push_back(vector<bool>(n, false));

				for(int i = 0; i < entering_col.getSize(); i++) {
				
					if(entering_col[i] >= 0.5) {
						columnValues[lambda_counter].push_back(i);
						realColumn[lambda_counter][i] = true;

					}

				}

				// Add the column to the master problem
				// (the cost of the new variable is always 1)
				char var_name[50];
				sprintf(var_name, "y%d", lambda_counter++);
				IloNumVar new_lambda(master_objective(1) + partition_constraint(entering_col), 0, IloInfinity);
				new_lambda.setName(var_name);

				lambda.add(new_lambda);

			} else { 

				x.clear();
				constraint.end();
				objective.end();
				pricing_problem.clear();
				pricing_env.end();

				break;
			
			}


			x.clear();
			constraint.end();
			objective.end();
			pricing_problem.clear();
			pricing_env.end();

		}


// verifying if the algorithm found an integer solution
		if(rmp.getCplexStatus() == IloCplex::Infeasible) {
			for(int i = 0; i < forbiddenLambdas.size(); i++) {
				lambda[ forbiddenLambdas[i] ].setUB(IloInfinity);
			}
			continue;
		}

		int x = std::ceil(rmp.getObjValue() - EPSILON);

		if(x >= qtBins) {
			for(int i = 0; i < forbiddenLambdas.size(); i++) {
				lambda[ forbiddenLambdas[i] ].setUB(IloInfinity);
			}
			continue;
		}


// create Z		

		vector<vector<double>> z(n, vector<double>(n, 0.0)); 

		for(size_t i = n; i < lambda.getSize(); i++) {
			for(int j = 0; j < columnValues[i].size(); j++) {

				for(int k = j+1; k < columnValues[i].size(); k++) {
					z[ columnValues[i][j] ][ columnValues[i][k] ] += rmp.getValue(lambda[i]);
				}

			}
		}


// find most fractional

		double value = 1e6;
		pair<int, int> items;

		for(int i = 0; i < n; i++) {
			for(int j = i+1; j < n; j++) {

				double absDelta = abs(z[i][j] - 0.5);
				if(absDelta < value) {
					value = absDelta;
					items = make_pair(i, j);
				}

			}
		}


// removing constraints

		for(int i = 0; i < forbiddenLambdas.size(); i++) {
			lambda[ forbiddenLambdas[i] ].setUB(IloInfinity);
		}


// checking if the tree needs to be pruned
		node.qtBins = x;

		if(z[items.first][items.second] <= EPSILON || z[items.first][items.second] >= 1 - EPSILON) {

			qtBins = x;

		} else {

			Node n = node;
			n.separated.push_back(items);
			tree.push_back(n);	

			n.separated.pop_back();
			
			n.together.push_back(items);
			tree.push_back(n);

		}
		
	}

	env.end();

	return qtBins;
}
