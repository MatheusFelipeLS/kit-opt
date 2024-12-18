#ifndef MAXBACK_H
#define MAXBACK_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <pthread.h>

#define EPSILON 0.00000001

using namespace std;

vector<int> SingleMaxBack(double** x, int n, int initialNode, vector<vector<int>> &subtours);


#endif
