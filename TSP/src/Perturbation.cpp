#include "Perturbation.h"

Solution Perturbation(Solution s, Data *data) {
    int tamTrechEsq = 2+(rand()%((int) ceil((double) data->getDimension()/10)-1));
    int tamTrechDir = 2+(rand()%((int) ceil((double) data->getDimension()/10)-1));
    
    int initTrecEsq = 1+(rand()%(data->getDimension()-tamTrechDir-tamTrechEsq));
    int initTrecDir = initTrecEsq+tamTrechEsq+(rand()%(data->getDimension()+1-tamTrechDir-tamTrechEsq-initTrecEsq));

    int NITE = s.sequence[initTrecEsq]; //nó no início do trecho esquerdo
    int NFTE = s.sequence[initTrecEsq+tamTrechEsq-1]; //nó no final do trecho esquerdo
    int NITD = s.sequence[initTrecDir]; //nó no início do trecho direito
    int NFTD = s.sequence[initTrecDir+tamTrechDir-1]; //nó no final do trecho direito

    s.cost += - data->getDistance(s.sequence[initTrecEsq-1], NITE) - data->getDistance(NFTD, s.sequence[initTrecDir+tamTrechDir])
    + data->getDistance(NFTE, s.sequence[initTrecDir+tamTrechDir]) + data->getDistance(NITD, s.sequence[initTrecEsq-1]);
    
    if(initTrecEsq + tamTrechEsq == initTrecDir) {
      s.cost += - data->getDistance(NFTE, NITD)  + data->getDistance(NFTD, NITE);
    } else {
      s.cost += - data->getDistance(NFTE, s.sequence[initTrecEsq+tamTrechEsq]) - data->getDistance(NITD, s.sequence[initTrecDir-1])
      + data->getDistance(NITE, s.sequence[initTrecDir-1]) + data->getDistance(NFTD, s.sequence[initTrecEsq+tamTrechEsq]); 
    }

    for(int i = 0; i < tamTrechEsq; i++) {
        s.sequence.insert(s.sequence.begin()+initTrecDir, s.sequence[initTrecEsq]);
        s.sequence.erase(s.sequence.begin() + initTrecEsq);
    }
    for(int i = 0; i < tamTrechDir; i++) {
        s.sequence.insert(s.sequence.begin()+initTrecEsq+i, s.sequence[initTrecDir+i]);
        s.sequence.erase(s.sequence.begin()+initTrecDir+i+1);
    }
    
    return s;
}