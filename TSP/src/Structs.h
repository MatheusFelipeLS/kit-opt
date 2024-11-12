#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>
#include "Data.h"
                  
typedef struct Solution {
    std::vector<int> sequence;
    double cost = 0;
} Solution;

struct InsertionCost {
    int noInserido;
    int arestaRemovida;
    double cost;
};

void showSolution(Solution s);
int calc_total_dist(Solution s, Data *data);

#endif