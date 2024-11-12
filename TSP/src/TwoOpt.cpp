#include "TwoOpt.h"

bool bestImprovement2Opt(Solution *s, Data *data) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->sequence.size() - 3; i++) {
    int vi = s->sequence[i];
    int vi_prev = s->sequence[i - 1]; 
    double valoresVi = -data->getDistance(vi, vi_prev);
    for(int j = i + 2; j < s->sequence.size()-1; j++) {
      int vj = s->sequence[j];
      int vj_next = s->sequence[j + 1];
      double delta = valoresVi - data->getDistance(vj, vj_next) + data->getDistance(vi, vj_next) 
      + data->getDistance(vi_prev, vj); 
      if(delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    for(int i = 0; i < 1 + ((best_j - best_i) / 2); i++) {
      std::swap(s->sequence[best_i+i], s->sequence[best_j-i]);
    };
    s->cost += bestDelta;
    return true;
  }
  return false;
}