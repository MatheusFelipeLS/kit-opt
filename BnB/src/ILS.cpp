#include "ILS.h"

void showSolution(Solution s) {
  for(int i = 0; i < s.sequence.size()-1; ++i) {
    cout << s.sequence[i] << " - ";
  }
  cout << s.sequence[s.sequence.size()-1] << endl;
}

int calcular_dist_total(Solution s, Data *pData) {
  s.cost = 0;
  for(int i = 0; i < s.sequence.size()-1; i++) {
    s.cost += pData->getDistance(s.sequence[i], s.sequence[i+1]);
  }
  return s.cost;
}

// Escolhe 3 valores, formando uma solução do tipo 1 - x - y - z - 1, com 1 < x, y e z < dim(instancia) 
vector<int> escolher3NosAleatorios(Data *data) {   //, vector<int> &CL
	vector<int> nos;
	int valor;

	nos.push_back(0);

	for(int i = 0; i < 3; i++) {
	    valor = rand() % data->getDimension();
        for(int j = 0; j < nos.size(); j++) {
            if(valor == nos[j]) {
                j = 0;
                valor = rand() % data->getDimension();
            }
        }
		nos.push_back(valor);
	}
	nos.push_back(0);

	return nos;
}

//verifica quais nós não estão não são os 3 escolhidos antes, e os insere na solução 
vector<int> nosRestantes(Data *data, vector<int> nosEscolhidos) {
	vector<int> nos;
	bool verifica;
	for(int i = 1; i < data->getDimension(); i++) {
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

vector<InsertionCost> calcularInsertionCost(Solution s, vector<int> CL, Data *pData) {
  vector<InsertionCost> InsertionCost((s.sequence.size() - 1) * CL.size());
  int l = 0;
  int a;
  for(int a = 0; a < s.sequence.size() - 1; a++) {
    int i = s.sequence[a];
    int j = s.sequence[a + 1];
    for (auto k : CL) {
      InsertionCost[l].cost = pData->getDistance(i, k) + pData->getDistance(j, k) - pData->getDistance(i, j);
      InsertionCost[l].noInserido = k;
      InsertionCost[l].arestaRemovida = a;
      l++;
    }
  }
  return InsertionCost;
}

void inserirNaSolucao(Solution *s, InsertionCost novoNo, vector<int> &CL) {
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

Solution Construcao(Data *pData) {
  Solution s;

  s.sequence = escolher3NosAleatorios(pData);
  vector<int> CL = nosRestantes(pData, s.sequence);
  while(!CL.empty()) {
    vector<InsertionCost> InsertionCost = calcularInsertionCost(s, CL, pData);
    sort(InsertionCost.begin(), InsertionCost.end(), check);  
    double alpha = (double) rand() / RAND_MAX;
    int selecionado = rand() % ((int) ceil(alpha * InsertionCost.size()));
    inserirNaSolucao(&s, InsertionCost[selecionado], CL);
  } 
  s.cost = calcular_dist_total(s, pData);
  return s;
}

bool bestImprovementSwap(Solution *s, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->sequence.size() - 2; i++) {
    int vi = s->sequence[i];
    int vi_next = s->sequence[i + 1];
    int vi_prev = s->sequence[i - 1];
    double valoresVi = - pData->getDistance(vi_prev, vi) - pData->getDistance(vi, vi_next);

    int vj = s->sequence[i+1];
    int vj_next = s->sequence[i+2];
    int vj_prev = s->sequence[i];
    double delta = valoresVi + pData->getDistance(vi_prev, vj) - pData->getDistance(vj, vj_next)
    + pData->getDistance(vi, vj_next) + pData->getDistance(vi, vi_next);
    if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = i+1;
    }
    for(int j = i + 2; j < s->sequence.size() - 1; j++) {
      vj = s->sequence[j];
      vj_next = s->sequence[j+1];
      vj_prev = s->sequence[j-1];
      delta = valoresVi + pData->getDistance(vi_prev, vj) + pData->getDistance(vj, vi_next) - pData->getDistance(vj_prev, vj) 
      - pData->getDistance(vj, vj_next) + pData->getDistance(vj_prev, vi) + pData->getDistance(vi, vj_next);
      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    swap(s->sequence[best_i], s->sequence[best_j]);
    s->cost += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovement2Opt(Solution *s, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->sequence.size() - 3; i++) {
    int vi = s->sequence[i];
    int vi_prev = s->sequence[i - 1]; 
    double valoresVi = -pData->getDistance(vi, vi_prev);
    for(int j = i + 2; j < s->sequence.size()-1; j++) {
      int vj = s->sequence[j];
      int vj_next = s->sequence[j + 1];
      double delta = valoresVi - pData->getDistance(vj, vj_next) + pData->getDistance(vi, vj_next) 
      + pData->getDistance(vi_prev, vj); 
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    for(int i = 0; i < 1 + ((best_j - best_i) / 2); i++) {
      swap(s->sequence[best_i+i], s->sequence[best_j-i]);
    };
    s->cost += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovementOrOpt(Solution *s, int tipoOpt, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  
  //i = nó que será reinserido
  for(int i = 1; i < s->sequence.size()-tipoOpt; i++) {
    int vi_prev = s->sequence[i-1];
    int vi = s->sequence[i];
    int i_tOpt = i+tipoOpt;
    int noFinal = i_tOpt - 1;
    double valoresVi = pData->getDistance(vi_prev, s->sequence[i_tOpt]) - pData->getDistance(vi_prev, vi) 
    - pData->getDistance(s->sequence[noFinal], s->sequence[i_tOpt]);
    
    //j = nó onde o nó i será reinserido
    for(int j = i + tipoOpt; j < s->sequence.size()-1; j++) {
      int prox_j = s->sequence[j+1];
      double delta = valoresVi + pData->getDistance(vi, s->sequence[j]) + pData->getDistance(s->sequence[noFinal], prox_j) 
      - pData->getDistance(s->sequence[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
    for(int j = 0; j < i-1; j++) {
      int prox_j = s->sequence[j+1];
      double delta = valoresVi + pData->getDistance(vi, s->sequence[j]) + pData->getDistance(s->sequence[noFinal], prox_j) 
      - pData->getDistance(s->sequence[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  } 

  if(bestDelta < 0) {
    if(best_i < best_j) {
      s->sequence.insert(s->sequence.begin()+best_j+1, s->sequence.begin()+best_i, s->sequence.begin()+best_i+tipoOpt);
      s->sequence.erase(s->sequence.begin()+best_i, s->sequence.begin()+best_i+tipoOpt);
    } else {
      vector<int> vecAux;
      vecAux.insert(vecAux.begin(), s->sequence.begin()+best_i, s->sequence.begin()+best_i+tipoOpt);
      s->sequence.erase(s->sequence.begin()+best_i, s->sequence.begin()+best_i+tipoOpt);
      s->sequence.insert(s->sequence.begin()+best_j+1, vecAux.begin(), vecAux.end());
    }
    s->cost += bestDelta;
    return true;
  }
  return false;
}

void buscaLocal(Solution *s, Data *pData) {
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    while(NL.empty() == false) {
      int n = rand() % NL.size();
      switch(NL[n]) {
        case 1: 
          improved = bestImprovementSwap(s, pData);
          break;
        case 2:
          improved = bestImprovement2Opt(s, pData);
          break;
        case 3:
          improved = bestImprovementOrOpt(s, 1, pData); //Reinsertion
          break;
        case 4:
          improved = bestImprovementOrOpt(s, 2, pData); //Or-opt2
          break;
        case 5:
          improved = bestImprovementOrOpt(s, 3, pData); //Or-opt3
          break;
      }
      if(improved) {
        NL = {1, 2, 3, 4, 5};
      }
      else
        NL.erase(NL.begin() + n);
    }
}

Solution Perturbacao(Solution s, Data *pData) {
    int tamTrechEsq = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    int tamTrechDir = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    
    int initTrecEsq = 1+(rand()%(pData->getDimension()-tamTrechDir-tamTrechEsq));
    int initTrecDir = initTrecEsq+tamTrechEsq+(rand()%(pData->getDimension()+1-tamTrechDir-tamTrechEsq-initTrecEsq));

    int NITE = s.sequence[initTrecEsq]; //nó no início do trecho esquerdo
    int NFTE = s.sequence[initTrecEsq+tamTrechEsq-1]; //nó no final do trecho esquerdo
    int NITD = s.sequence[initTrecDir]; //nó no início do trecho direito
    int NFTD = s.sequence[initTrecDir+tamTrechDir-1]; //nó no final do trecho direito

    s.cost += - pData->getDistance(s.sequence[initTrecEsq-1], NITE) - pData->getDistance(NFTD, s.sequence[initTrecDir+tamTrechDir])
    + pData->getDistance(NFTE, s.sequence[initTrecDir+tamTrechDir]) + pData->getDistance(NITD, s.sequence[initTrecEsq-1]);
    
    if(initTrecEsq + tamTrechEsq == initTrecDir) {
      s.cost += - pData->getDistance(NFTE, NITD)  + pData->getDistance(NFTD, NITE);
    } else {
      s.cost += - pData->getDistance(NFTE, s.sequence[initTrecEsq+tamTrechEsq]) - pData->getDistance(NITD, s.sequence[initTrecDir-1])
      + pData->getDistance(NITE, s.sequence[initTrecDir-1]) + pData->getDistance(NFTD, s.sequence[initTrecEsq+tamTrechEsq]); 
    }

    for(int i = 0; i < tamTrechEsq; i++) {
        s.sequence.insert(s.sequence.begin()+initTrecDir, s.sequence[initTrecEsq]);
        s.sequence.erase(s.sequence.begin() + initTrecEsq);
    }
    for(int i = 0; i < tamTrechDir; i++) {
        s.sequence.insert(s.sequence.begin()+initTrecEsq+i, s.sequence[initTrecDir+i]);
        s.sequence.erase(s.sequence.begin()+initTrecDir+i+1);
    }
    
    return s;
}

double ILS(Data *data, int max_iter) {
    size_t n = data->getDimension();

    Solution bestSolution;
    bestSolution.cost = numeric_limits<int>::max();
    
    int maxIterIls = n;
    if(n >= 150) 
      maxIterIls /= 2;

    for(int i = 0; i < max_iter; i++) {
      Solution s = Construcao(data);
      Solution best = s;
      int iterILS = 0;
      while(iterILS <= maxIterIls) {
        buscaLocal(&s, data);
        if(s.cost < best.cost) {
          best = s;
          iterILS = 0;
        }
        s = Perturbacao(best, data);
        iterILS++;
      }
      if(best.cost < bestSolution.cost) {
        bestSolution = best;
      }
    }
	return bestSolution.cost;
}
