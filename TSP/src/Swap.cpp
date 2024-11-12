#include "Swap.h"

bool bestImprovementSwap(Solution *s, Data *data) {
  double bestDelta = 0;
  int best_i, best_j;
  for(int i = 1; i < s->sequence.size() - 2; i++) {
    int vi = s->sequence[i];
    int vi_next = s->sequence[i + 1];
    int vi_prev = s->sequence[i - 1];
    double valoresVi = - data->getDistance(vi_prev, vi) - data->getDistance(vi, vi_next);

    int vj = s->sequence[i+1];
    int vj_next = s->sequence[i+2];
    int vj_prev = s->sequence[i];
    double delta = valoresVi + data->getDistance(vi_prev, vj) - data->getDistance(vj, vj_next)
    + data->getDistance(vi, vj_next) + data->getDistance(vi, vi_next);
    if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = i+1;
    }
    for(int j = i + 2; j < s->sequence.size() - 1; j++) {
      vj = s->sequence[j];
      vj_next = s->sequence[j+1];
      vj_prev = s->sequence[j-1];
      delta = valoresVi + data->getDistance(vi_prev, vj) + data->getDistance(vj, vi_next) - data->getDistance(vj_prev, vj) 
      - data->getDistance(vj, vj_next) + data->getDistance(vj_prev, vi) + data->getDistance(vi, vj_next);
      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }
  if(bestDelta < 0) {
    std::swap(s->sequence[best_i], s->sequence[best_j]);
    s->cost += bestDelta;
    return true;
  }
  return false;
}