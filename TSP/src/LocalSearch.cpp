#include "LocalSearch.h"

void LocalSearch(Solution *s, Data *data) {
    std::vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    while(NL.empty() == false) {
      int n = rand() % NL.size();
      switch(NL[n]) {
        case 1: 
          improved = bestImprovementSwap(s, data);
          break;
        case 2:
          improved = bestImprovement2Opt(s, data);
          break;
        case 3:
          improved = bestImprovementOrOpt(s, 1, data); //Reinsertion
          break;
        case 4:
          improved = bestImprovementOrOpt(s, 2, data); //Or-opt2
          break;
        case 5:
          improved = bestImprovementOrOpt(s, 3, data); //Or-opt3
          break;
      }
      if(improved) {
        NL = {1, 2, 3, 4, 5};
      }
      else
        NL.erase(NL.begin() + n);
    }
}