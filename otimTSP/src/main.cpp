#include "readData.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices


//void printData();
struct InsertionInfo{
    int noInserido;
    int arestaRemovida;
    double custo;
};

InsertionInfo calcularCustoInsercao (Solution& s, std::vector<int>& CL){

  custoInsercao= std::vector<InsertionInfo> custoInsercao ((s.size()-1) * CL.size());

  int l= 0;

  for(int a= 0; b= 1; i < s.size() - 1; a++, b++){
    int i= s.sequence[a];
    int j= s.sequence[b];

    for (auto k : CL){
      custoInsercao[l].custo= c[i][k] + c[j][k] - c[i][j];
      custoInsercao[l].noInserido= k;
      custoInsercao[l].arestaRemovida= a;
      l++;
    }

  }
  return custoInsercao;
}

Solution Construcao(){
  
  Solution s;
  int numRandom, i, j, quant, aux;

  s.sequence.push_back(1);
  s.sequence.push_back(1);      //adicionando a cidade 1
  CL.erase(CL.begin());        //tirando a cidade 1 da lista de candidatos

  for(i= 1; i <= 3; i++){       //funcao para escolher 3 cidades aleatorias
    std::vector<int> CL;
    srand(time(0));
    quant= CL.size();
    numRandom= 1+rand()% CL[quant-1];
    s.sequence.insert(s.sequence.end()-1, numRandom);

    j= 0;
    while(1){           //retirar cidades aleatorias da lista de candidatos
      if(numRandom == CL[j]){
        CL.erase(CL.begin()+j);
        break;
      }
      j++;
    }
  }
  
  while(!CL.empty()){
    std::vector<InsertionInfo> custoInsercao= calcularCustoInsercao(s, CL);

    for(i= 0; i < custoInsercao.size(); i++){               //ordernar os custos
      for(j= 0; j < custoInsercao.size()-1; j++){

        if(custoInsercao[j] > custoInsercao[j+1]){
          aux= custoInsercao[j];
          custoInsercao[j]= custoInsercao[j+1]; 
          custoInsercao[j+1]= aux; 
        }
      }
    }

    double alpha= (double) rand() / RAND_MAX;
    int selecionado= rand() % ( (int) ceil(alpha * custoInsercao.size()) );
    inserirNaSolucao(s, selecionado);
  }
}

int main(int argc, char** argv) {

    readData(argc, argv, &dimension, &matrizAdj);
    //printData();
    return 0;  

}
/*
void printData() {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}
*/