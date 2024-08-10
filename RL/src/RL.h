#include <iostream>
#include <vector>

#include "Kruskal.h"

#define epsilon_min 0.0005
#define i_max 30

using namespace std;

vector<double> RL(vector<vector<double>> &cost, vector<double> &lambda, vector<pair<int, int>> &edges, double ub, double &lb, bool &feasible);

void addPenalty(vector<vector<double>> &cost, vector<double> &lambda);
void attLambda(vector<int> &subgradient, vector<double> &lambda, double UB, double LB, double epsilon);
vector<int> degreeOfNodes(vector<pair<int, int>> &edges);
double extraPenalty(vector<double> &lambda);
vector<int> getSubgradient(vector<int> &degree);
int getSumOfTheSquareOfCoordinates(vector<int> &subgradient);
bool isTour(vector<int> &degree);
void removePenalty(vector<vector<double>> &cost, vector<double> &lambda);

// Essas funções só foram usadas para teste
void printSubgradient(vector<int> &subgradient);
void printLambda(vector<double> &lambda);
void printDegree(vector<int> &degree);
void printMST(vector<pair<int, int>> &edges);
void printCostMatrix(vector<vector<double>> &cost);