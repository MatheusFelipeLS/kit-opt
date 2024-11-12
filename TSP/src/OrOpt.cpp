#include "OrOpt.h"

bool bestImprovementOrOpt(Solution *s, int range, Data *data) {
  double bestDelta = 0;
  int best_i, best_j;
  
  //i = nó que será reinserido
  for(int i = 1; i < s->sequence.size()-range; i++) {
    int vi_prev = s->sequence[i-1];
    int vi = s->sequence[i];
    int i_tOpt = i+range;
    int noFinal = i_tOpt - 1;
    double valoresVi = data->getDistance(vi_prev, s->sequence[i_tOpt]) - data->getDistance(vi_prev, vi) 
    - data->getDistance(s->sequence[noFinal], s->sequence[i_tOpt]);
    
    //o nó i será reinserido entre os nós j e j+1
    //verifica se existe solução melhor na qual o nó é inserido algumas casas a frente na solução
    for(int j = i + range; j < s->sequence.size()-1; j++) {
      int prox_j = s->sequence[j+1];
      double delta = valoresVi + data->getDistance(vi, s->sequence[j]) + data->getDistance(s->sequence[noFinal], prox_j) 
      - data->getDistance(s->sequence[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
    //verifica se existe solução melhor na qual o nó é inserido algumas casas atrás na solução
    for(int j = 0; j < i-1; j++) {
      int prox_j = s->sequence[j+1];
      double delta = valoresVi + data->getDistance(vi, s->sequence[j]) + data->getDistance(s->sequence[noFinal], prox_j) 
      - data->getDistance(s->sequence[j], prox_j);
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  } 

  if(bestDelta < 0) {
    if(best_i < best_j) {
      s->sequence.insert(s->sequence.begin()+best_j+1, s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
      s->sequence.erase(s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
    } else {
      vector<int> vecAux;
      vecAux.insert(vecAux.begin(), s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
      s->sequence.erase(s->sequence.begin()+best_i, s->sequence.begin()+best_i+range);
      s->sequence.insert(s->sequence.begin()+best_j+1, vecAux.begin(), vecAux.end());
    }
    s->cost += bestDelta;
    return true;
  }
  return false;
}