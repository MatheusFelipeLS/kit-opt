#include "Structs.h"

void showSolution(Solution s) {
  for(int i = 0; i < s.sequence.size()-1; ++i) {
    cout << s.sequence[i] << " - ";
  }
  cout << s.sequence[s.sequence.size()-1] << endl;
}

int calc_total_dist(Solution s, Data *data) {
  s.cost = 0;
  for(int i = 0; i < s.sequence.size()-1; i++) {
    s.cost += data->getDistance(s.sequence[i], s.sequence[i+1]);
  }
  return s.cost;
}