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

int qtUsedNodes(const vector<vector<int>> &subtours);

int nextNode(vector<vector<int>> &subtours);

vector<bool> createCandidateList(int n, int node);

double calculateCutMin(double** x, int n, int node);

void calculateBv(double** x, int n, vector<double> &bv, vector<int> &S, vector<bool> &CL);

int getNodeWithBiggestMaxBack(const vector<double> &bv);

vector<int> SingleMaxBack(double** x, int n, int initialNode, vector<vector<int>> &subtours);


#endif
