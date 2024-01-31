#include "Data.h"
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

#define max_iter 50

using namespace std;

typedef struct Solucao {
    vector<int> ordem;
    double custo = 0;
} Solucao;

struct custoInsercao {
    int noInserido;
    int arestaRemovida;
    double custo;
};

void mostraSolucao(Solucao s) {
  for(int i = 0; i < s.ordem.size()-1; ++i) {
    cout << s.ordem[i] << " - ";
  }
  cout << s.ordem[s.ordem.size()-1] << endl;
}

int calcular_dist_total(Solucao s, Data *pData) {
  s.custo = 0;
  for(int i = 0; i < s.ordem.size()-1; i++) {
    s.custo += pData->getDistance(s.ordem[i], s.ordem[i+1]);
  }
  return s.custo;
}

// Escolhe 3 valores, formando uma solução do tipo 1 - x - y - z - 1, com 1 < x, y e z < dim(instancia) 
vector<int> escolher3NosAleatorios(Data *data) {   //, vector<int> &CL
	vector<int> nos;
	int valor;
	srand(time(NULL));

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
vector<int> nosRestantes(Data *data, vector<int> nosEscolhidos) {
	vector<int> nos;
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

vector<custoInsercao> calcularCustoInsercao(Solucao s, vector<int> CL, Data *pData) {
  vector<custoInsercao> custoInsercao((s.ordem.size() - 1) * CL.size());
  int l = 0;
  for(int a = 0; a < s.ordem.size() - 1; a++) {
    int i = s.ordem[a];
    int j = s.ordem[a + 1];
    for (auto k : CL) {
      custoInsercao[l].custo = pData->getDistance(i, k) + pData->getDistance(j, k) - pData->getDistance(i, j);
      custoInsercao[l].noInserido = k;
      custoInsercao[l].arestaRemovida = a;
      l++;
    }
  }
  return custoInsercao;
}

void ordenarEmOrdemCrescente(vector<custoInsercao> &CI) {
  int i, trocou;
  custoInsercao aux;

  trocou = 1;
  while(trocou){
    trocou = 0;
    for(i = 0; i < CI.size()-1; i++){
      if (CI[i].custo > CI[i+1].custo) {
        aux = CI[i];
        CI[i] = CI[i+1];
        CI[i+1] = aux;
        trocou = 1;
      }
    }
  }
}

void inserirNaSolucao(Solucao *s, custoInsercao novoNo, vector<int> &CL) {
  s->ordem.insert(s->ordem.begin() + novoNo.arestaRemovida + 1, novoNo.noInserido);
  for(int i = 0; i < CL.size(); i++) {
    if(CL[i] == novoNo.noInserido) {
      CL.erase(CL.begin()+i);
      break;
    }
  }
}

bool check(custoInsercao i, custoInsercao j){
  return i.custo < j.custo;
}

Solucao Construcao(Data *pData) {
  Solucao s;
  srand(time(NULL));

  s.ordem = escolher3NosAleatorios(pData);
  vector<int> CL = nosRestantes(pData, s.ordem);

  while(!CL.empty()) {
    vector<custoInsercao> custoInsercao = calcularCustoInsercao(s, CL, pData);
    // ordenarEmOrdemCrescente(custoInsercao);
    sort(custoInsercao.begin(), custoInsercao.end(), check);
    int alpha = rand() % 2;
    if(CL.size() == 1) 
      alpha = 0;
    // double alpha = (double) rand() / RAND_MAX;
    // int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
    inserirNaSolucao(&s, custoInsercao[alpha], CL);
  } 

  s.custo = calcular_dist_total(s, pData);
  return s;
}


bool bestImprovementSwap(Solucao *s, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->ordem.size() - 2; i++) {
    int vi = s->ordem[i];
    int vi_next = s->ordem[i + 1];
    int vi_prev = s->ordem[i - 1];
    double valoresVi = - pData->getDistance(vi_prev, vi) - pData->getDistance(vi, vi_next);

    int vj = s->ordem[i+1];
    int vj_next = s->ordem[i+2];
    int vj_prev = s->ordem[i];
    double delta = valoresVi + pData->getDistance(vi_prev, vj) - pData->getDistance(vj, vj_next) + pData->getDistance(vi, vj_next) + pData->getDistance(vi, vi_next);
    if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = i+1;
    }
    for(int j = i + 2; j < s->ordem.size() - 1; j++) {
      vj = s->ordem[j];
      vj_next = s->ordem[j+1];
      vj_prev = s->ordem[j-1];
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
    swap(s->ordem[best_i], s->ordem[best_j]);
    s->custo += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovement2Opt(Solucao *s, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->ordem.size() - 3; i++) {
    int vi = s->ordem[i];
    int vi_prev = s->ordem[i - 1]; 
    double valoresVi = -pData->getDistance(vi, vi_prev);
    for(int j = i + 2; j < s->ordem.size()-1; j++) {
      int vj = s->ordem[j];
      int vj_next = s->ordem[j + 1];
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
      swap(s->ordem[best_i+i], s->ordem[best_j-i]);
    };
    s->custo += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovementOrOpt(Solucao *s, int tipoOpt, Data *pData) {
  double bestDelta = 0;
  int best_i, best_j;
  
  //i = nó que será reinserido
  for(int i = 1; i < s->ordem.size()-tipoOpt; i++) {
    int vi_prev = s->ordem[i-1];
    int vi = s->ordem[i];
    int i_tOpt = i+tipoOpt;
    int noFinal = i_tOpt - 1;
    double valoresVi = pData->getDistance(vi_prev, s->ordem[i_tOpt]) - pData->getDistance(vi_prev, vi) 
    - pData->getDistance(s->ordem[noFinal], s->ordem[i_tOpt]);
    
    //j = nó onde o nó i será reinserido
    for(int j = i + tipoOpt; j < s->ordem.size()-1; j++) {
      int prox_j = s->ordem[j+1];
      double delta = valoresVi + pData->getDistance(vi, s->ordem[j]) + pData->getDistance(s->ordem[noFinal], prox_j) 
      - pData->getDistance(s->ordem[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
    for(int j = 0; j < i-1; j++) {
      int prox_j = s->ordem[j+1];
      double delta = valoresVi + pData->getDistance(vi, s->ordem[j]) + pData->getDistance(s->ordem[noFinal], prox_j) 
      - pData->getDistance(s->ordem[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  } 

  if(bestDelta < 0) {
    if(best_i < best_j) {
      for(int i = 0; i < tipoOpt; i++) {
        s->ordem.insert(s->ordem.begin()+best_j+1, s->ordem[best_i]);
        s->ordem.erase(s->ordem.begin() + best_i);
      }
    }else {
      for(int i = 0; i < tipoOpt; i++) {
        s->ordem.insert(s->ordem.begin()+best_j+i+1, s->ordem[best_i+i]);
        s->ordem.erase(s->ordem.begin() + best_i+i+1);
      }
    }
    s->custo += bestDelta;
    return true;
  }
  return false;
}

void buscaLocal(Solucao *s, Data *pData) {
    vector<int> NL = {1, 2, 3, 4, 5};
    srand(time(NULL));
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

Solucao Perturbacao(Solucao s, Data *pData) {
    srand(time(NULL));
    
    // int initTrecDir = 3+(rand() % (pData->getDimension()-5));
    // int initTrecEsq = 1+(rand()%(initTrecDir-2));
    
    // int tamTrechEsq = 2+(rand()%(initTrecDir-initTrecEsq-1));
    // int tamTrechDir = 2+(rand()%(pData->getDimension()-initTrecDir-2));

    int tamTrechEsq = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    int tamTrechDir = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    
    int initTrecEsq = 1+(rand()%(pData->getDimension()-tamTrechDir-tamTrechEsq));
    int initTrecDir = initTrecEsq+tamTrechEsq+(rand()%(pData->getDimension()+1-tamTrechDir-tamTrechEsq-initTrecEsq));

    int NITE = s.ordem[initTrecEsq]; //nó no início do trecho esquerdo
    int NFTE = s.ordem[initTrecEsq+tamTrechEsq-1]; //nó no final do trecho esquerdo
    int NITD = s.ordem[initTrecDir]; //nó no início do trecho direito
    int NFTD = s.ordem[initTrecDir+tamTrechDir-1]; //nó no final do trecho direito

    s.custo += - pData->getDistance(s.ordem[initTrecEsq-1], NITE) - pData->getDistance(NFTD, s.ordem[initTrecDir+tamTrechDir])
    + pData->getDistance(NFTE, s.ordem[initTrecDir+tamTrechDir]) + pData->getDistance(NITD, s.ordem[initTrecEsq-1]);
    
    if(initTrecEsq + tamTrechEsq == initTrecDir) {
      s.custo += - pData->getDistance(NFTE, NITD)  + pData->getDistance(NFTD, NITE);
    } else {
      s.custo += - pData->getDistance(NFTE, s.ordem[initTrecEsq+tamTrechEsq]) - pData->getDistance(NITD, s.ordem[initTrecDir-1])
      + pData->getDistance(NITE, s.ordem[initTrecDir-1]) + pData->getDistance(NFTD, s.ordem[initTrecEsq+tamTrechEsq]); 
    }
    
    // cout << "initTrecEsq: " << initTrecEsq << "; initTrecDir: " << initTrecDir << 
    // "; tamTrechEsq: " << tamTrechEsq << "; tamTrechDir: " << tamTrechDir << endl;
    
    for(int i = 0; i < tamTrechEsq; i++) {
        s.ordem.insert(s.ordem.begin()+initTrecDir, s.ordem[initTrecEsq]);
        s.ordem.erase(s.ordem.begin() + initTrecEsq);
    }
    for(int i = 0; i < tamTrechDir; i++) {
        s.ordem.insert(s.ordem.begin()+initTrecEsq+i, s.ordem[initTrecDir+i]);
        s.ordem.erase(s.ordem.begin()+initTrecDir+i+1);
    }
    
    return s;
}


int main(int argc, char** argv) {
  time_t start, end;
  int media = 0;
  start = clock();

  for(int cont = 0; cont < 10; cont++) {

    ios_base::sync_with_stdio(false);

    auto data = Data(argc, argv[1]);
    data.read();
    Data *pData = &data;
    size_t n = data.getDimension();

    Solucao melhorSolucao;
    melhorSolucao.custo = numeric_limits<int>::max();
    
    int maxIterIls = n;
    if(n >= 150) 
      maxIterIls /= 2;

    for(int i = 0; i < max_iter; i++) {
      Solucao s = Construcao(pData);
      Solucao melhorTemporaria = s;
      int iterILS = 0;
      while(iterILS <= maxIterIls) {
        buscaLocal(&s, pData);
        if(s.custo < melhorTemporaria.custo) {
          melhorTemporaria = s;
          iterILS = 0;
        }
        s = Perturbacao(melhorTemporaria, pData);
        iterILS++;
      }
      if(melhorTemporaria.custo < melhorSolucao.custo) 
        melhorSolucao = melhorTemporaria;
    }

    mostraSolucao(melhorSolucao);
    cout << "Custo: " << melhorSolucao.custo << endl;
    media += melhorSolucao.custo;
  }

  cout << "Custo médio: " << media/10;
  end = clock();
      double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
      cout << "; Tempo médio: " << fixed 
          << time_taken/10 << setprecision(5);
      cout << " sec " << endl;

  return 0;
}