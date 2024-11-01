#ifndef MINCUT_H
#define MINCUT_H

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

int qtUsedNodes2(const vector<vector<int>> &subtours);

void calculateDegreeOfConnection(const double** x, vector<double> &degreeOfConnection, const vector<int> &newSet, const vector<vector<int>> &V, const vector<bool> &CL);

int mostTightlyConnectedSet(vector<double> &degreeOfConnection);

void forbiddenSets(vector<bool> &CL, vector<vector<int>> &A);

vector<vector<int>> MinimumCutPhase(double **x, int n, vector<vector<int>> &V, int idxInitialSet);

double calculateCutOfThePhase(double** x, int n, vector<vector<int>> &S);

void merge2LastSets(vector<vector<int>>& V, vector<vector<int>> &S);

vector<vector<int>> SingleMinCut(double** x, int n, int initialNode);


#endif