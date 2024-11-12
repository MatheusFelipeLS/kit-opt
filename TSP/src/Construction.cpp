#include "Construction.h"

// Escolhe 3 valores, formando uma solução do tipo 1 - x - y - z - 1, com 1 < x, y e z < dim(instancia) 
std::vector<int> escolher3NosAleatorios(Data *data) {  
	std::vector<int> nos;
	int valor;

	nos.push_back(1);

	for(int i = 0; i < 3; i++) {
	    valor = rand() % data->getDimension() + 1;
        for(int j = 0; j < nos.size(); j++) {
            if(valor == nos[j]) {
                j = 0;
                valor = rand() % data->getDimension() + 1;
            }
        }
		nos.push_back(valor);
	}
	nos.push_back(1);

	return nos;
}

//verifica quais nós não estão não são os 3 escolhidos antes, e os insere na solução 
std::vector<int> nosRestantes(Data *data, std::vector<int> nosEscolhidos) {
	std::vector<int> nos;
	bool verifica;
	for(int i = 2; i < data->getDimension()+1; i++) {
	  verifica = false;
		for(int j = 1; j < nosEscolhidos.size()-1; j++) {
			if(i == nosEscolhidos[j]) {
				verifica = true;
				break;
			}
		}
		if(!verifica) {
			nos.push_back(i);
		}
	}

	return nos;
}

std::vector<InsertionCost> calcularInsertioncost(Solution s, std::vector<int> CL, Data *data) {
  std::vector<InsertionCost> InsertionCost((s.sequence.size() - 1) * CL.size());
  int l = 0;
  for(int a = 0; a < s.sequence.size() - 1; a++) {
    int i = s.sequence[a];
    int j = s.sequence[a + 1];
    for (auto k : CL) {
      InsertionCost[l].cost = data->getDistance(i, k) + data->getDistance(j, k) - data->getDistance(i, j);
      InsertionCost[l].noInserido = k;
      InsertionCost[l].arestaRemovida = a;
      l++;
    }
  }
  return InsertionCost;
}

void inserirNaSolucao(Solution *s, InsertionCost novoNo, std::vector<int> &CL) {
  s->sequence.insert(s->sequence.begin() + novoNo.arestaRemovida + 1, novoNo.noInserido);
  for(int i = 0; i < CL.size(); i++) {
    if(CL[i] == novoNo.noInserido) {
      CL.erase(CL.begin()+i);
      break;
    }
  }
}

bool check(InsertionCost i, InsertionCost j){
  return i.cost < j.cost;
}

Solution Construction(Data *data) {
  Solution s;

  s.sequence = escolher3NosAleatorios(data);
  std::vector<int> CL = nosRestantes(data, s.sequence);

  while(!CL.empty()) {
    std::vector<InsertionCost> InsertionCost = calcularInsertioncost(s, CL, data);
    std::sort(InsertionCost.begin(), InsertionCost.end(), check);  
    double alpha = (double) rand() / RAND_MAX;
    int selecionado = rand() % ((int) ceil(alpha * InsertionCost.size()));
    inserirNaSolucao(&s, InsertionCost[selecionado], CL);
  } 

  s.cost = calc_total_dist(s, data);
  return s;
}