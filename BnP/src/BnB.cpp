#include "BnB.h"

#include "combo.c"

//escolhe o próximo nó
Node branchingStrategy(std::vector<Node> &tree, int strategy) {
	
	if(strategy < 1 || strategy > 2) {
		printf("Invalid branch strategy\n");
		exit(0);
	}

	Node n;

	if(strategy == 1) { //bfs

		n = tree[0];
		std::swap(tree[0], tree[tree.size()-1]);
		tree.pop_back();
		return n;

	} 

	n = tree[(tree.size()-1)];
	tree.pop_back();

	return n; 
}


int BnB(Data *data, int strategy) {

	int optimalSolution = data->getNItems(); // returned value


// instance data

	int n = data->getNItems(); // instance size
	int capacity = data->getBinCapacity();

	const double M = 1e6;


// auxiliare variables 

	std::vector<std::vector<double>> z(n, std::vector<double>(n, 0.0)); 
	double value = 1e6;
	std::pair<int, int> items;
	int qtBins;


// variables related to branch and bound

	Node root;
	std::vector<Node> tree; 

	std::vector<std::vector<bool>> booleanColumns;
	std::vector<std::vector<int>> columnValues;


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

		booleanColumns.push_back(std::vector<bool>(n, false));
		booleanColumns[i][i] = true;
	}

	master_model.add(partition_constraint);

	IloObjective master_objective = IloMinimize(env, sum_obj);
	master_model.add(master_objective);

	int lambda_counter = n;

	// solving pricing problem
	IloCplex rmp(master_model);
	rmp.setOut(env.getNullStream()); // disables CPLEX log


// solving first node with combo algorithm

	stype ub_combo = optimalSolution * M * M; // combo upper bound

	while(true) {

		rmp.solve();

		if(rmp.getCplexStatus() == IloCplex::Infeasible) {
			break;
		}

		// Get the dual variables
		IloNumArray pi(env, n);

		rmp.getDuals(pi, partition_constraint);

		item *combo_items = (item *) malloc(sizeof(item) * n);

		for(int i = 0; i < n; i++) {
			combo_items[i].w = data->getItemWeight(i);
			combo_items[i].p = (stype) pi[i] * M;
			combo_items[i].x = false;
			combo_items[i].index = i;
		}


		long objValue = combo(combo_items, (combo_items + n - 1), (stype) capacity, 0, (stype) ub_combo, 1, 0);

		if(1 - ((double) objValue / M) < -EPSILON) {

			IloNumArray entering_col(env, n);

			columnValues.push_back({});
			booleanColumns.push_back(std::vector<bool>(n, false));

			for(int i = 0; i < n; i++) {
			
				if(combo_items[i].x < 0.5) {

					entering_col[ combo_items[i].index ] = 0;

				} 
				else {
					entering_col[ combo_items[i].index ] = 1;
					columnValues[lambda_counter].push_back( combo_items[i].index );
					booleanColumns[lambda_counter][ combo_items[i].index ] = true;
				}

			}

			std::sort(columnValues[lambda_counter].begin(), columnValues[lambda_counter].end());


			// Add the column to the master problem
			// (the cost of the new variable is always 1)
			char var_name[50];
			sprintf(var_name, "y%d", lambda_counter++);
			IloNumVar new_lambda(master_objective(1) + partition_constraint(entering_col), 0, IloInfinity);
			new_lambda.setName(var_name);

			lambda.add(new_lambda);

		} else {

			break;
		
		}

		free(combo_items);

	}


/////////////////// creating Z matrix

	for(size_t i = n; i < lambda.getSize(); i++) {
		for(int j = 0; j < columnValues[i].size(); j++) {

			for(int k = j+1; k < columnValues[i].size(); k++) {
				z[ columnValues[i][j] ][ columnValues[i][k] ] += rmp.getValue(lambda[i]);
			}

		}
	}	


//////// taking the pair of items with most fractional value

	for(int i = 0; i < n; i++) {
		for(int j = i+1; j < n; j++) {

			double absDelta = abs(z[i][j] - 0.5);
			if(absDelta < value) {
				value = absDelta;
				items = std::make_pair(i, j);
			}

		}
	}

	
	qtBins = std::ceil(rmp.getObjValue() - EPSILON); // getting the amount of used bins in the root 


	// checking if the solution is feasible
	if(z[items.first][items.second] <= EPSILON || z[items.first][items.second] >= 1 - EPSILON) {

		return qtBins;

	} else {

		root.separated.push_back(items);
		tree.push_back(root);	

		root.separated.pop_back();
		
		root.together.push_back(items);
		tree.push_back(root);

	}


// start the branch and bound method

	while(!tree.empty()) {

		Node node;

		node = branchingStrategy(tree, strategy); 
	

// applying constraints on master model

		std::vector<int> forbiddenLambdas;

		for(int i = 0; i < booleanColumns.size(); i++) {
			
			for(int j = 0; j < node.together.size(); j++) {
				
				if(booleanColumns[i][ node.together[j].first ] != booleanColumns[i][ node.together[j].second ]) {
					forbiddenLambdas.push_back(i);
					lambda[i].setUB(0.0);
					j = -1;
					i++;
					if(i == booleanColumns.size()) break;
				}

			}

			if(i == booleanColumns.size()) break;

			for(int j = 0; j < node.separated.size(); j++) {
				
				if(booleanColumns[i][ node.separated[j].first ] && booleanColumns[i][ node.separated[j].second ]) {
					forbiddenLambdas.push_back(i);
					lambda[i].setUB(0.0);
					break;
				}

			}

		}
		
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


			// pricing constraints

			IloExpr constraint(pricing_env);
			IloExpr objective(pricing_env);

			for(int i = 0; i < n; i++) {
				constraint += (data->getItemWeight(i) * x[i]);
				objective -= (x[i] * pi[i]);
			}

			pricing_model.add( constraint <= capacity );
			pricing_model.add(IloMinimize(pricing_env, 1 + objective));


			// node-related constraints

			for(int i = 0; i < node.together.size(); i++) {
				pricing_model.add( x[node.together[i].first] == x[node.together[i].second] );
			}

			for(int i = 0; i < node.separated.size(); i++) {
				pricing_model.add( x[node.separated[i].first] + x[node.separated[i].second] <= 1 );
			}


			// solving pricing

			IloCplex pricing_problem(pricing_env);

			pricing_problem.setParam(IloCplex::Param::Threads, 1);
			pricing_problem.setOut(pricing_env.getNullStream());

			pricing_problem.extract(pricing_model);
			
			pricing_problem.solve();

			if (pricing_problem.getObjValue() < -EPSILON) {

				IloNumArray entering_col(env, n);

				pricing_problem.getValues(x, entering_col);

				columnValues.push_back({});
				booleanColumns.push_back(std::vector<bool>(n, false));

				for(int i = 0; i < entering_col.getSize(); i++) {
				
					if(entering_col[i] >= 0.5) {
						columnValues[lambda_counter].push_back(i);
						booleanColumns[lambda_counter][i] = true;

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

		qtBins = std::ceil(rmp.getObjValue() - EPSILON);

		if(qtBins >= optimalSolution) {
			for(int i = 0; i < forbiddenLambdas.size(); i++) {
				lambda[ forbiddenLambdas[i] ].setUB(IloInfinity);
			}
			continue;
		}


// create Z		

		for(int i = 0; i < n; i++) {
			for(int j = i+1; j < n; j++) {
				z[i][j] = 0.0;
			}
		}

		for(size_t i = n; i < lambda.getSize(); i++) {
			for(int j = 0; j < columnValues[i].size(); j++) {

				for(int k = j+1; k < columnValues[i].size(); k++) {
					z[ columnValues[i][j] ][ columnValues[i][k] ] += rmp.getValue(lambda[i]);
				}

			}
		}


// find most fractional

		value = 1e6;
		items = {0, 0};

		for(int i = 0; i < n; i++) {
			for(int j = i+1; j < n; j++) {

				double absDelta = abs(z[i][j] - 0.5);
				if(absDelta < value) {
					value = absDelta;
					items = std::make_pair(i, j);
				}

			}
		}


// removing constraints

		for(int i = 0; i < forbiddenLambdas.size(); i++) {
			lambda[ forbiddenLambdas[i] ].setUB(IloInfinity);
		}


// checking if the tree needs to be pruned

		if(z[items.first][items.second] <= EPSILON || z[items.first][items.second] >= 1 - EPSILON) {

			optimalSolution = qtBins;

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

	return optimalSolution;
}
