#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include "data.h"

using namespace std;

typedef struct Solution {
    vector<int> sequence;
    double cost = 0;
} Solution;

struct InsertionCost {
    int noInserido;
    int arestaRemovida;
    double cost;
};

double ILS(Data *data, int max_iter);