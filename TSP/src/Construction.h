#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Data.h"
#include "Structs.h"

std::vector<int> escolher3NosAleatorios(Data *data);
std::vector<int> nosRestantes(Data *data, std::vector<int> nosEscolhidos);
std::vector<InsertionCost> calcularInsertioncost(Solution s, std::vector<int> CL, Data *data);
void inserirNaSolucao(Solution *s, InsertionCost novoNo, std::vector<int> &CL);
Solution Construction(Data *data);

#endif