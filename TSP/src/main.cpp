#include "Data.h"
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

#include "ILS.h"

#define max_iter 50

int main(int argc, char** argv) {
  time_t start, end;
  double media = 0;
  start = clock();
  srand(time(NULL));
  ios_base::sync_with_stdio(false);

  auto data = Data(argc, argv[1]);
  data.read();
  Data *pData = &data;

  Solution s;

  for(int cont = 0; cont < 10; cont++) {
    s = ILS(pData, max_iter);

    media += s.cost;
  }

  cout << "cost médio: " << media/10;
  end = clock();
      double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
      cout << "; Tempo médio: " << fixed 
          << time_taken/10 << setprecision(5);
      cout << " sec " << endl;

  return 0;
}