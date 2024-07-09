#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include "data.h"

using namespace std;

typedef struct Solution {
    vector<int> sequence;
    double cost = 0;
} Solution;

struct InsertionCost {
    int noInserido;
    int arestaRemovida;
    double cost;
};

double ILS(Data *data, int max_iter);
void showSolution(Solution s);
int calcular_dist_total(Solution s, Data *pData);
vector<int> escolher3NosAleatorios(Data *data);
vector<int> nosRestantes(Data *data, vector<int> nosEscolhidos);
vector<InsertionCost> calcularInsertionCost(Solution s, vector<int> CL, Data *pData);
void inserirNaSolucao(Solution *s, InsertionCost novoNo, vector<int> &CL);
bool check(InsertionCost i, InsertionCost j);
Solution Construcao(Data *pData);
bool bestImprovementSwap(Solution *s, Data *pData);
bool bestImprovement2Opt(Solution *s, Data *pData);
bool bestImprovementOrOpt(Solution *s, int tipoOpt, Data *pData);
void buscaLocal(Solution *s, Data *pData);
Solution Perturbacao(Solution s, Data *pData);
double ILS(Data *data, int max_iter);
