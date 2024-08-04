#include <iostream>
#include <vector>

#include "Kruskal.h"

#define epsilon_min 0.065
#define i_max 10

using namespace std;

vector<double> RL(vector<vector<double>> &cost, vector<double> &lambda, vector<pair<int, int>> &edges, double ub, double &lb, bool &feasible);
vector<int>degreeOfNodes(vector<pair<int, int>> &edges);
void addPenalty(vector<vector<double>> &cost, vector<double> &lambda);
void attLambda(vector<double> &subgradient, vector<double> &lambda, double UB, double LB, double epsilon);
vector<int>degreeOfNodes(vector<pair<int, int>> &edges);
vector<double> getSubgradient(vector<int> &degree);
double getSumOfTheSquareOfCoordinates(vector<double> &subgradient);
bool isValid(vector<int> &degree);
void removePenalty(vector<vector<double>> &cost, vector<double> &lambda);

void printLambda(vector<double> &lambda);
void printDegree(vector<int> &degree);
void printMST(vector<pair<int, int>> &edges);