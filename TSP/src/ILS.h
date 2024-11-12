#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <vector>
#include <limits>
#include "Data.h"
#include "Structs.h"
#include "Construction.h"
#include "LocalSearch.h"
#include "Perturbation.h"

Solution ILS(Data *data, int maxIter);

#endif