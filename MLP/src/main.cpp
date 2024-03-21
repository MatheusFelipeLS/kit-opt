//Código para resolução do MLP

//includes
#include "Data.h"
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

//definições
#define max_iter 50

using namespace std;
  
//structs
typedef struct Solucao {
    vector<int> ordem;
    double custo = 0;
} Solucao;

struct custoInsercao {
    int noInserido;
    double custo;
};

typedef struct Subsequence {
	double T, C;
	int W;
	int first, last; // primeiro e ultimo nos da subsequencia
	inline static Subsequence Concatenate(Subsequence &sigma_1, Subsequence &sigma_2, Data *pData) {
		Subsequence sigma;
		double temp = pData->getDistance(sigma_1.last, sigma_2.first);  
		sigma.W = sigma_1.W + sigma_2.W;
		sigma.T = sigma_1.T + temp + sigma_2.T;
		sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
		sigma.first = sigma_1.first;
		sigma.last = sigma_2.last;
		return sigma;
	}
} Subsequence;

//--------------------------------FUNÇÕES------------------------------------//
//funções gerais
void mostrarSolucao(Solucao s);
void UpdateAllSubseq(Solucao s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);
void atualiza(vector<vector<Subsequence>> &subseq_matrix, int start, int end, Data *pData);

//funções da construção em ordem de aparição no método Construção
vector<int> escolher3NosAleatorios(Data *data);
vector<int> nosRestantes(Data *data, vector<int> nosEscolhidos);
vector<custoInsercao> calcularCustoInsercao(Solucao s, vector<int> CL, Data *pData);
void inserirNaSolucao(Solucao *s, custoInsercao novoNo, vector<int> &CL);
Solucao Construcao(Data *pData);

//UpdateAllSubseq
void UpdateAllSubseq(Solucao s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);

//funções para busca local em ordem de aparição no método buscaLocal
bool bestImprovementSwap_mlp(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);
bool bestImprovement2Opt_mlp(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);
bool bestImprovementOrOpt_mlp(Solucao *s, int tipoOpt, Data *pData, vector<vector<Subsequence>> &subseq_matrix);
void buscaLocal(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);

//Perturbação
Solucao Perturbacao_mlp(Solucao s, Data *pData, vector<vector<Subsequence>> &subseq_matrix);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateAllSubseq(Solucao s, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
	int n = subseq_matrix.size();
	// subsequencias de um unico no
	for(int i = 0; i < n; i++) {
		int v = s.ordem[i];
		subseq_matrix[i][i].W = (i > 0);
		subseq_matrix[i][i].C = 0;
		subseq_matrix[i][i].T = 0;
		subseq_matrix[i][i].first = v;
		subseq_matrix[i][i].last = v;
	}
  
	for(int i = 0; i < n; i++)
		for(int j = i + 1; j < n; j++)
			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], pData);

	// subsequencias invertidas (necessarias para o 2-opt)
	for(int i = n - 1; i >= 0; i--)
		for(int j = i - 1; j >= 0; j--)
			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], pData);
}

//função para modificar as subsequencias que foram modificadas
void atualiza(vector<vector<Subsequence>> &subseq_matrix, int start, int end, Data *pData) {
  int n = subseq_matrix.size();
  //start é a posição da primeira modificação e end é a posição da última
  //refaz o retangulo superior esquerdo
  for(int i = 0; i < start; ++i) 
    for(int j = start; j < n; ++j) {
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], pData);
    }
  //refaz o lado direito da região modificada
  for(int i = start; i <= end; ++i) 
    for(int j = i+1; j < n; ++j) 
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], pData);

  //refaz o retangulo inferior esquerdo
  for(int i = n - 1; i > end; --i)
		for(int j = end; j >= 0; --j)
			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], pData);
    
  //refaz o lado esquerdo da região modificada
  for(int i = end; i >= start; --i)
		for(int j = i-1; j >= 0; --j)
			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], pData);
}

void mostrarSolucao(Solucao s) {
  for(int i = 0; i < s.ordem.size()-1; ++i) {
    cout << s.ordem[i] << " - ";
  }
  cout << s.ordem[s.ordem.size()-1] << endl;
}

vector<custoInsercao> calcularCustoInsercao_mlp(Solucao s, vector<int> CL, Data *pData) {
  vector<custoInsercao> custoInsercao(CL.size());
  int l = s.ordem.size();
  int a = 0;
  for(auto k : CL) {
    custoInsercao[a].custo = pData->getDistance(s.ordem[l-1], k);
    custoInsercao[a].noInserido = k;
    ++a;
  }
  return custoInsercao;
}

void inserirNaSolucao_mlp(Solucao *s, custoInsercao novoNo, vector<int> &CL) {
  s->ordem.push_back(novoNo.noInserido);
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
  vector<int> CL;

  s.ordem.push_back(1);

  for(int i = 0; i < pData->getDimension()-1; ++i) {
    CL.push_back(i+2);
  }

  while(!CL.empty()) {
    vector<custoInsercao> custoInsercao = calcularCustoInsercao_mlp(s, CL, pData);
    sort(custoInsercao.begin(), custoInsercao.end(), check);
    double alpha = (double) rand() / RAND_MAX;
    int porcent = ceil(( alpha * CL.size()) / 4);
    int selecionado = (rand() % porcent);
    inserirNaSolucao_mlp(&s, custoInsercao[selecionado], CL);
  }

  s.ordem.push_back(1);

  return s;
}

bool bestImprovementSwap_mlp(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
  double bestDelta = 0;
  int best_i, best_j;

  int n = s->ordem.size();
  for(int i = 1; i < n-3; ++i) {
    for(int j = i+2; j < n-1; ++j) {
      Subsequence sigma_1, sigma_2;
      sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j][j], pData);
      sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[i+1][j-1], pData);
      sigma_1 = Subsequence::Concatenate(sigma_2, subseq_matrix[i][i], pData);
      sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[j+1][n-1], pData);
      if(sigma_2.C - s->custo < bestDelta) {
        bestDelta = sigma_2.C - s->custo;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    swap(s->ordem[best_i], s->ordem[best_j]);
    Subsequence aux = subseq_matrix[best_i][best_i];
    subseq_matrix[best_i][best_i] = subseq_matrix[best_j][best_j];
    subseq_matrix[best_j][best_j] = aux;
    atualiza(subseq_matrix, best_i, best_j, pData);
    s->custo += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovement2Opt_mlp(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
  double bestDelta = 0;
  int best_i, best_j;
  int n = s->ordem.size();
  for(int i = 1; i < n-3; ++i) {
    for(int j = i+2; j < n-1; ++j) {
      Subsequence sigma_1, sigma_2;
      sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j][i], pData);
      sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[j+1][n-1], pData);
      if(sigma_2.C - s->custo < bestDelta) {
        bestDelta = sigma_2.C - s->custo;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    for(int i = 0; i < 1 + ((best_j - best_i) / 2); i++) {
      swap(s->ordem[best_i+i], s->ordem[best_j-i]);
      swap(subseq_matrix[best_i+i][best_i+i], subseq_matrix[best_j-i][best_j-i]);
    }
    atualiza(subseq_matrix, best_i, best_j, pData);
    s->custo += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovementOrOpt_mlp(Solucao *s, int tipoOpt, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
  double bestDelta = 0;
  int best_i, best_j;

  int n = s->ordem.size();
  for(int i = 1; i < n-tipoOpt; ++i) {
    Subsequence sigma_1, sigma_2;
    //reinserindo os nós em posições a frente
    for(int j = i+tipoOpt; j < n-1; ++j) {
      sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[i+tipoOpt][j], pData);
      sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[i][i+tipoOpt-1], pData);
      sigma_1 = Subsequence::Concatenate(sigma_2, subseq_matrix[j+1][n-1], pData);
      if(sigma_1.C - s->custo < bestDelta) {
        bestDelta = sigma_1.C - s->custo;
        best_i = i;
        best_j = j;
      }
    }

    //reinserindo os nós nas posições finais
    for(int j = 0; j < i-1; ++j) {
      sigma_1 = Subsequence::Concatenate(subseq_matrix[0][j], subseq_matrix[i][i+tipoOpt-1], pData);
      sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[j+1][i-1], pData);
      sigma_1 = Subsequence::Concatenate(sigma_2, subseq_matrix[i+tipoOpt][n-1], pData);
      if(sigma_1.C - s->custo < bestDelta) {
        bestDelta = sigma_1.C - s->custo;
        best_i = i;
        best_j = j;
      }
    }
  }

  if(bestDelta < 0) {
    if(best_i < best_j) {
      //reinserindo os nós 
      for(int i = 0; i < tipoOpt; i++) {
        s->ordem.insert(s->ordem.begin()+best_j+1, s->ordem[best_i]);
        s->ordem.erase(s->ordem.begin() + best_i);
        subseq_matrix.insert(subseq_matrix.begin()+best_j+1, subseq_matrix[best_i]);
        subseq_matrix.erase(subseq_matrix.begin() + best_i);
      }

      //desloca os nós que estão sendo reinseridos para a direita
      for(int i = 0; i < tipoOpt; ++i) {
        subseq_matrix[best_j-i][best_j-i] = subseq_matrix[best_j-i][best_i+tipoOpt-i-1];
      }
      
      //ajeita a diagonal principal dos nós que estão entre os nós reinseridos e a posição da reinserção
      for(int i = best_i; i < best_j-tipoOpt+1; ++i) {
        subseq_matrix[i][i] = subseq_matrix[i][i+tipoOpt];
      }
      
      atualiza(subseq_matrix, best_i, best_j, pData);
    }else {
      //reinserindo os nós
      for(int i = 0; i < tipoOpt; i++) {
        s->ordem.insert(s->ordem.begin()+best_j+i+1, s->ordem[best_i+i]);
        s->ordem.erase(s->ordem.begin() + best_i+i+1);
        subseq_matrix.insert(subseq_matrix.begin()+best_j+i+1, subseq_matrix[best_i+i]);
        subseq_matrix.erase(subseq_matrix.begin() + best_i+i+1);
      }
      
      //ajeita a diagonal principal dos nós reinseridos
      for(int i = 0; i < tipoOpt; ++i) {
        subseq_matrix[best_j+i+1][best_j+i+1] = subseq_matrix[best_j+i+1][best_i+i];
      }     
      
      //ajeita a diagonal principal dos nós que trocaram de posição, mas não porque foram reinseridos
      for(int i = best_j+tipoOpt+1; i <= best_i+tipoOpt-1; ++i) {
        subseq_matrix[i][i] = subseq_matrix[i][i-tipoOpt];
      }
      
      atualiza(subseq_matrix, best_j+1, best_i+tipoOpt-1, pData);
    }
    s->custo += bestDelta;
    return true;
  }
  return false;
}

void buscaLocal(Solucao *s, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    while(NL.empty() == false) {
      int n = rand() % NL.size();
      switch(NL[n]) {
        case 1: 
          improved = bestImprovementSwap_mlp(s, pData, subseq_matrix);
          break;
        case 2:
          improved = bestImprovement2Opt_mlp(s, pData, subseq_matrix);
          break;
        case 3:
          improved = bestImprovementOrOpt_mlp(s, 1, pData, subseq_matrix); //Reinsertion
          break;
        case 4:
          improved = bestImprovementOrOpt_mlp(s, 2, pData, subseq_matrix); //Or-opt2
          break;
        case 5:
          improved = bestImprovementOrOpt_mlp(s, 3, pData, subseq_matrix); //Or-opt3
          break;
      }
      if(improved) {
        NL = {1, 2, 3, 4, 5};
      }
      else
        NL.erase(NL.begin() + n);
    }
}

Solucao Perturbacao_mlp(Solucao s, Data *pData, vector<vector<Subsequence>> &subseq_matrix) {
    int n = pData->getDimension();

    int tamTrecEsq = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    int tamTrecDir = 2+(rand()%((int) ceil((double) pData->getDimension()/10)-1));
    
    int initTrecEsq = 1+(rand()%(pData->getDimension()-tamTrecDir-tamTrecEsq));
    int initTrecDir = initTrecEsq+tamTrecEsq+(rand()%(pData->getDimension()+1-tamTrecDir-tamTrecEsq-initTrecEsq));

    //colocando o trecho da esquerda para direita
    for(int i = 0; i < tamTrecEsq; i++) {
        s.ordem.insert(s.ordem.begin()+initTrecDir, s.ordem[initTrecEsq]);
        s.ordem.erase(s.ordem.begin() + initTrecEsq);
        subseq_matrix.insert(subseq_matrix.begin()+initTrecDir, subseq_matrix[initTrecEsq]);
        subseq_matrix.erase(subseq_matrix.begin() + initTrecEsq);
    }

    //colocando o trecho da direita para esquerda
    for(int i = 0; i < tamTrecDir; i++) {
        s.ordem.insert(s.ordem.begin()+initTrecEsq+i, s.ordem[initTrecDir+i]);
        s.ordem.erase(s.ordem.begin()+initTrecDir+i+1);
        subseq_matrix.insert(subseq_matrix.begin()+initTrecEsq+i, subseq_matrix[initTrecDir+i]);
        subseq_matrix.erase(subseq_matrix.begin()+initTrecDir+i+1);
    }

    //ajeita a diagonal principal do trecho que foi movido para "direita"
    for(int i = 0; i < tamTrecEsq; ++i) {
        int x = initTrecDir+tamTrecDir-tamTrecEsq+i;
        subseq_matrix[x][x] = subseq_matrix[x][initTrecEsq+i];
    }
    
    //ajeita a diagonal principal do trecho que foi movido para "esquerda"
    for(int i = 0; i < tamTrecDir; ++i) {
        subseq_matrix[initTrecEsq+i][initTrecEsq+i] = subseq_matrix[initTrecEsq+i][initTrecDir+i];
    }

    //ajeita a diagonal principal das linhas entre os trechos trocados
    for(int i = initTrecEsq + tamTrecDir; i < initTrecDir + tamTrecDir - tamTrecEsq; ++i) {
        subseq_matrix[i][i] = subseq_matrix[i][i+tamTrecEsq-tamTrecDir];
    }   
    
    atualiza(subseq_matrix, initTrecEsq, initTrecDir+tamTrecDir-1, pData);
    
    s.custo = subseq_matrix[0][s.ordem.size()-1].C;

    return s;
}

int main(int argc, char** argv) {
  time_t start, end;
  double media = 0;
  start = clock();
  srand(time(NULL));

  ios_base::sync_with_stdio(false);

  for(int cont = 0; cont < 10; cont++) {
    auto data = Data(argc, argv[1]);
    data.read();
    Data *pData = &data;
    size_t n = data.getDimension();

    Solucao melhorSolucao;
    vector<vector<Subsequence>> subseq_matrix(n+1, vector<Subsequence>(n+1));
    melhorSolucao.custo = numeric_limits<int>::max();
    
    int maxIterIls = n;
    if(n >= 150) 
      maxIterIls /= 2;

    for(int i = 0; i < max_iter; i++) {
      Solucao s = Construcao(pData);
      UpdateAllSubseq(s, pData, subseq_matrix);
      s.custo = subseq_matrix[0][n].C;
      Solucao melhorTemporaria = s;
      int iterILS = 0;
      while(iterILS <= maxIterIls) {
        buscaLocal(&s, pData, subseq_matrix);
        if(s.custo < melhorTemporaria.custo) {
          melhorTemporaria = s;
          iterILS = 0;
        }
        s = Perturbacao_mlp(melhorTemporaria, pData, subseq_matrix);
        iterILS++;
      }
      if(melhorTemporaria.custo < melhorSolucao.custo) 
        melhorSolucao = melhorTemporaria;
    }

    mostrarSolucao(melhorSolucao);
    cout << "Custo: " << melhorSolucao.custo << endl;
    media += melhorSolucao.custo;
  }

  cout << "Custo médio: " << media/10;
  end = clock();
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "; Tempo médio: " << fixed << time_taken/10 << setprecision(5);
  cout << " sec " << endl << endl;

  return 0;
}