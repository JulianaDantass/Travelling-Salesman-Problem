#include "readData.h"
#include <fstream>
#include <iostream>

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
  s.sequence= escolher3NosAleatorios();

  std::vector<int> CL= nosRestantes();

  while(!CL.empty()){
    std::vector<InsertionInfo> custoInsercao= calcularCustoInsercao(s, CL);
    ordenarEmOrdemCrescente(custoInsercao);
    double alpha= (double) rand() / RAND_MAX;
    int selecionado= rand() % ((int) ceil(alpha * custoInsercao.size()));
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