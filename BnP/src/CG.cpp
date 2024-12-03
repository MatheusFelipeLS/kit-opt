#include "CG.h"

lambdaInfo CG(Data *data, Node &node) {
	lambdaInfo l;
	
	const double M = 1e6;

	int n = data->getNItems();    
	vector<int> weight(n);
	int capacity = data->getBinCapacity();

	for(int i = 0; i < n; i++) {
		weight[i] = data->getItemWeight(i);
	}

	IloEnv env;
	IloModel master_model(env);

	IloNumVarArray lambda(env, n, 0, IloInfinity);

	IloExpr sum_obj(env);
	IloRangeArray partition_constraint(env);

	for (int i = 0; i < n; i++)
	{
		char var_name[50];
		sprintf(var_name, "y%d", i);

		lambda[i].setName(var_name);
		sum_obj += M * lambda[i];
	}

	master_model.add(partition_constraint);

	IloObjective master_objective = IloMinimize(env, sum_obj);
	master_model.add(master_objective);

	IloCplex rmp(master_model);

	rmp.setOut(env.getNullStream()); // disables CPLEX log

	rmp.solve();

	// cout << "Initial lower bound: " << rmp.getObjValue() << endl;

	// cout << "Initial solution: ";
	// for (size_t j = 0; j < lambda.getSize(); j++)
	// {
	// 	cout << rmp.getValue(lambda[j]) << " ";
	// }
	// cout << endl;

	int lambda_counter = n;

	while(true) {
		// Get the dual variables
		IloNumArray pi(env, n);

		rmp.getDuals(pi, partition_constraint);

		// for (size_t i = 0; i < n; i++)
		// {
		// 	cout << "Dual variable of constraint " << i << " = " << pi[i] << endl;
		// }

		// Build and solve the pricing problem

		IloEnv pricing_env;
		IloModel pricing_model(pricing_env);

		IloNumVarArray x(pricing_env, n, 0, 1, ILOINT);

		IloExpr auxExprConstraint(pricing_env);
		IloExpr auxExprObjective(pricing_env);

		for(int i = 0; i < n; i++) {
			auxExprConstraint += (weight[i] * x[i]);
			auxExprObjective -= (x[i] * pi[i]);
		}

		for(int i = 0; i < node.separated.size(); i++) {
			pricing_model.add(x[node.separated[i].first] + x[node.separated[i].second] <= 1);
		}

		for(int i = 0; i < node.together.size(); i++) {
			pricing_model.add(x[node.together[i].first] + x[node.together[i].second] == 2);
		}

		auxExprObjective += 1;

		pricing_model.add(IloRange(pricing_env, 0, auxExprConstraint, capacity));
		pricing_model.add(IloMinimize(pricing_env, auxExprObjective));


		IloCplex pricing_problem(pricing_env);
		pricing_problem.extract(pricing_model);
		
		pricing_problem.setOut(pricing_env.getNullStream());
		pricing_problem.solve();


		// ...

		if (pricing_problem.getObjValue() < -1e-5) {

			// cout << "Reduced cost is equal to " << pricing_problem.getObjValue() << ", which is less than 0..." << endl;
			
			IloNumArray entering_col(env, n);

			pricing_problem.getValues(x, entering_col);

			// cout << endl << "Entering column:" << endl;
			l.lambdaItems.push_back({});
			for (size_t i = 0; i < n; i++)
			{
				if(entering_col[i] >= 0.5) l.lambdaItems[lambda_counter-n].push_back(i); 
				// cout << (entering_col[i] < 0.5 ? 0 : 1) << endl;
			}
			// cout << endl;

			// Add the column to the master problem
			// (the cost of the new variable is always 1)
			char var_name[50];
			sprintf(var_name, "y%d", lambda_counter++);
			IloNumVar new_lambda(master_objective(1) + partition_constraint(entering_col), 0, IloInfinity);
			new_lambda.setName(var_name);

			for(int i = 0; i < node.separated.size(); i++) {
				if(entering_col[node.separated[i].first] == 1 && entering_col[node.separated[i].second] == 1) {
					new_lambda.setUB(0.0);
					break;
				}
			}

			for(int i = 0; i < node.together.size(); i++) {
				if(entering_col[node.together[i].first] == 0 && entering_col[node.together[i].second] == 0) {
					new_lambda.setUB(0.0);
					break;
				}
			}

			lambda.add(new_lambda);

			// cout << "Solving the RMP again..." << endl;
			rmp.solve();
			// ...
		} else { 
			// cout << "No column with negative reduced costs found. The current basis is optimal" << endl;
			// cout << "Final master problem: " << endl;
			// system("cat model.lp");
			break;
		}

		auxExprConstraint.end();
		auxExprObjective.end();
		pricing_problem.clear();
		pricing_env.end();
	}

  double qtBins = 0;
	
	for (size_t j = 0; j < lambda.getSize(); j++) {
		qtBins += rmp.getValue(lambda[j]);
		l.lambdaValue.push_back(rmp.getValue(lambda[j]));
		// cout << rmp.getValue(lambda[j]) << " ";
	}
	// cout << endl;

	node.qtBins = qtBins;
    
	env.end();
	
	// std::cout << "Bins usados: " << qtBins << "\n";

	return l;
}