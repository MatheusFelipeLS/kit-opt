#include <iostream>
#include <vector>
#include <ilcplex/ilocplex.h>
#include <malloc.h>
#include <algorithm>

#include "Data.h"


#define EPSILON 1e-4

struct Node {
	std::vector<std::pair<int, int>> together;
	std::vector<std::pair<int, int>> separated; 
};

int BnB(Data *data, int strategy);
Node branchingStrategy(std::vector<Node> &tree, int strategy);
