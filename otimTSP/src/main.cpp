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

struct Solution{
    std::vector<int> sequence;
    double custoSolucao;
};

InsertionInfo calcularCustoInsercao (Solution& s, std::vector<int>& CL){

  custoInsercao= std::vector<InsertionInfo> custoInsercao ((s.size()-1) * CL.size());

  int l= 0;

  for(int a= 0; b= 1; i < s.size() - 1; a++, b++){
    int i= s.sequence[a];
    int j= s.sequence[b];

    for (auto k : CL){
      custoInsercao[l].custo= matrizAdj[i][k] + matrizAdj[j][k] - matrizAdj[i][j];
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

  s.sequence.push_back(1);      //adicionando a cidade 1 no inicio
  s.sequence.push_back(1);      //adicionando a c1 no final
  CL.erase(CL.begin());        //tirando a cidade 1 da lista de candidatos

  for(i= 1; i <= 3; i++){       //funcao para escolher 3 cidades aleatorias
    std::vector<int> CL;
    srand(time(0));
    quant= CL.size();
    numRandom= CL[0]+rand()% CL[quant-1];      //gera numeros aleatorios entre o primeiro elem de CL e o penultimo elem. de CL
    s.sequence.insert(s.sequence.end()-1, numRandom);

    j= 0;
    while(1){           //retirar cidades escolhidas aleatoriamente da lista de candidatos
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

    j= 0;
    while(1){           
      if(selecionado == CL[j]){
        CL.erase(CL.begin()+j);
        break;
      }
      j++;
    }

    s.sequence.insert(s.sequence.begin()+ custoInsercao.arestaRemovida + 1, selecionado);
  }

  for(i= 0; i < s.sequence.size()-1; i++){        //calcular o custo do tour solução

    s.custoSolucao += matrizAdj[i][i+1];        
  }
  
}

bool BestImprovementSwap (Solution *s){         //estrutura de vizinhança: SWAP 

  double bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double custoSwap;

  for(i= 1; i < s->sequence.size() - 1; i++) {
    for (j= i + 1; j < s->sequence.size() - 1; j++){
      
      custoSwap= s->custoSolucao - matrizAdj[i-1][i] - matrizAdj[j][j+1] + matrizAdj[i-1][j] + matrizAdj[i][j+1];

      double delta= s->custoSolucao - custoSwap;

      if(delta < bestDelta){
        bestDelta= delta;
        best_i= i;
        best_j= j;
      }
    }
  }

  if (bestDelta < 0){
    std::swap(s->sequence[best_i], s->sequence[best_j]);
    s->custoSolucao= s->custoSolucao - delta;
    return true;
  }

  return false;
}

bool BestImprovement2Opt (Solution *s){         //estrutura de vizinhança: 2opt 

  double bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double custoSwap;

  for(i= 1; i < s->sequence.size() - 1; i++) {
    for (j= i + 2; j < s->sequence.size() - 1; j++){
      
      custoSwap= s->custoSolucao - matrizAdj[i-1][i] - matrizAdj[j][j+1] + matrizAdj[i-1][j] + matrizAdj[i][j+1];

      double delta= s->custoSolucao - custoSwap;

      if(delta < bestDelta){
        bestDelta= delta;
        best_i= i;
        best_j= j;
      }
    }
  }
  if (bestDelta < 0){

    j= best_j;
    for(i= best_i; i < j; i++){               //for para inverter a subsequencia obtida anteriormente
      std::swap(s->sequence[i], s->sequence[j]);
      j--;
    }
    s->custoSolucao= s->custoSolucao - delta;
    
    return true;
  }

  return false; 
}

bool BestImprovementOrOpt (Solution *s, int quantity){         

  double bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double custoSwap;

  switch(quantity){
    
    case 1:             //método: REINSERTION

      for(i= 1; i < s->sequence.size() - 1; i++) {
        for (j= i + 1; j < s->sequence.size() - 1; j++){
          
          custoSwap= s->custoSolucao - matrizAdj[i][i-1] - matrizAdj[i][i+1] - matrizAdj[j][j+1] + matrizAdj[i-1][i+1] + matrizAdj[i][j] + matrizAdj[i][j+1];

          double delta= s->custoSolucao - custoSwap;

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
      }
      if (bestDelta < 0){
        s->sequence.erase(s->sequence.begin() + best_i);
        s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i]);
        s->custoSolucao= s->custoSolucao - delta;
        
        return true;
      }
      return false;

    case 2:          //método: OR-OPT-2

      for(i= 1; i < s->sequence.size() - 1; i++) {
        for (j= i + 2; j < s->sequence.size() - 1; j++){
          
          custoSwap= s->custoSolucao - matrizAdj[i][i-1] - matrizAdj[i+1][i+2] - matrizAdj[j+1][j+2] + matrizAdj[i-1][i+2] + matrizAdj[j+1][i] + matrizAdj[i+1][j+2];

          double delta= s->custoSolucao - custoSwap;

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
      }
      if (bestDelta < 0){
        s->sequence.erase(s->sequence.begin() + best_i);
        s->sequence.erase(s->sequence.begin() + (best_i + 1));
        s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i]);
        s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i+1]);
        s->custoSolucao= s->custoSolucao - delta;
        
        return true;
      }
      return false;

    case 3:      //método: OR-OPT-3

      for(i= 1; i < s->sequence.size() - 1; i++) {
        for (j= i + 3; j < s->sequence.size() - 1; j++){
          
          custoSwap= s->custoSolucao - matrizAdj[i][i-1] - matrizAdj[i+2][i+3] - matrizAdj[j+2][j+3] + matrizAdj[i-1][i+3] + matrizAdj[j+2][i] + matrizAdj[i+2][j+3];

          double delta= s->custoSolucao - custoSwap; 

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
      }
      if (bestDelta < 0){
        s->sequence.erase(s->sequence.begin() + best_i);
        s->sequence.erase(s->sequence.begin() + (best_i + 1));
        s->sequence.erase(s->sequence.begin() + (best_i + 2));
        s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i]);
        s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i+1]);
        s->sequence.insert(s->sequence.begin() + best_j + 2, s->sequence[best_i+2]);
        s->custoSolucao= s->custoSolucao - delta;
        
        return true;
      }

      return false;  
  }

}

void BuscaLocal (Solution *s){

  std::vector<int>NL= {1, 2, 3, 4, 5};

  bool improved= false;

  while(NL.empty() == false){

    int n= rand() % NL.size;

    switch (NL(n)) {
      case 1: 
        improved= BestImprovementSwap(s);
        break;
      case 2: 
        improved= BestImprovement2Opt(s);
        break;
      case 3:
        improved= BestImprovementOrOpt(s, 1);   //reinsertion
        break;
      case 4:
        improved= BestImprovementOrOpt(s, 2);   //Or-opt2
        break;
      case 5:
        improved= BestImprovementOrOpt(s, 3);   //Or-opt3
        break;
    }

    if(improved){
      NL= {1, 2, 3, 4, 5};

    }else{
      NL.erase(NL.begin() + n);
    }

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