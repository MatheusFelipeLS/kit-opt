#include "ILS.h"

Solution ILS(Data *data, int maxIter) {
  size_t n = data->getDimension();
  Solution bestSolution;
  bestSolution.cost = std::numeric_limits<int>::max();
  
  int maxIterIls = n;
  if(n >= 150) maxIterIls /= 2;

  for(int i = 0; i < maxIter; i++) {
    Solution s = Construction(data);
    Solution best = s;

    int iterILS = 0;
    while(iterILS <= maxIterIls) {
      LocalSearch(&s, data);
      if(s.cost < best.cost) {
        best = s;
        iterILS = 0;
      }
      s = Perturbation(best, data);
      iterILS++;
    }
    if(best.cost < bestSolution.cost) 
      bestSolution = best;
  }

  return bestSolution;
}