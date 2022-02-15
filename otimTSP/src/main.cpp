#include "readData.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices


void printData();

struct InsertionInfo{
  int noInserido;
  int arestaRemovida;
  double custo;
};

struct Solution{
  std::vector<int> sequence;
  double custoSolucao;
};

bool compares(InsertionInfo a, InsertionInfo b){  //funcao auxiliar na ordenacao dos custos
  return a.custo < b.custo;
}

std:: vector<InsertionInfo> calcularCustoInsercao (Solution& s, std::vector<int>& CL){

  std::vector<InsertionInfo> custoInsercao ((s.sequence.size()-1) * CL.size());

  

  int a, b, i, k, count;
  int l= 0;

  count= 0;
  for(int a= 0, b= 1; count < s.sequence.size() - 1; a++, b++){
    
    int i= s.sequence[a];
    int j= s.sequence[b];

    for (auto k : CL){
      custoInsercao[l].custo= matrizAdj[i][k] + matrizAdj[j][k] - matrizAdj[i][j];
      custoInsercao[l].noInserido= k;
      custoInsercao[l].arestaRemovida= a;
      l++;
      
    }
    count++;
  }
  
  return custoInsercao;
}

Solution Construcao(Solution& s){
  
  int numRandom, i, j, quant;
  bool flag;         
  std::vector<int> CL;

  for(i= 1; i <= dimension; i++){
    CL.push_back(i);
  }

  s.sequence.push_back(1);      //adicionando a cidade 1 no inicio
  s.sequence.push_back(1);      //adicionando a c1 no final
  CL.erase(CL.begin());        //tirando a cidade 1 da lista de candidatos

  srand(time(0));

  for(i= 1; i <= 3; i++){       //funcao para escolher 3 cidades aleatorias
    quant= CL.size();

    flag= false;
    while(1){       //gera numeros aleatorios entre o primeiro elem de CL e o penultimo elem. de CL
      
      numRandom= rand() % CL[quant];

      if(numRandom >= CL[0] && numRandom <= CL[quant-1]){
        for(j= 0; j < quant; j++){
          if(numRandom == CL[j]){
              flag= true;          //flag pra saber se entrou no if
              break;
          }
        }
      }
      if(flag){
        break;
      }
    }

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
    
    
    sort(custoInsercao.begin(), custoInsercao.end(), compares);   //ordena os custos

    double alpha= (double) rand() / RAND_MAX;
    int selecionado= rand() % ( (int) ceil(alpha * custoInsercao.size()) );

    s.sequence.insert(s.sequence.begin()+ custoInsercao[selecionado].arestaRemovida + 1, custoInsercao[selecionado].noInserido);

    j= 0;
    while(1){                     //retirando a cidade inserida da lista de candidatos  
      if(custoInsercao[selecionado].noInserido == CL[j]){
        CL.erase(CL.begin()+j);
        break;
      }
      j++;
    }
  }
  
  s.custoSolucao= 0;
  for(i= 0; i < s.sequence.size()-1; i++){        //calcular o custo do tour solução
    s.custoSolucao += matrizAdj[s.sequence[i]][s.sequence[i+1]];        
  }
  
  return s;
}

bool BestImprovementSwap (Solution& s){         //estrutura de vizinhança: SWAP 

  double delta, bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double changeCost;

  for(i= 1; i < s.sequence.size() - 2; i++) {
    for (j= i + 1; j < s.sequence.size() - 1; j++){
      
      changeCost= s.custoSolucao - matrizAdj[s.sequence[i-1]][s.sequence[i]] - matrizAdj[s.sequence[i]][s.sequence[i+1]] - matrizAdj[s.sequence[j]][s.sequence[j+1]] - matrizAdj[s.sequence[j-1]][s.sequence[j]] 
                                 + matrizAdj[s.sequence[i-1]][s.sequence[j]] + matrizAdj[s.sequence[j]][s.sequence[i+1]] + matrizAdj[s.sequence[i]][s.sequence[j+1]] + matrizAdj[s.sequence[i]][s.sequence[j-1]];
 
      delta= changeCost - s.custoSolucao;

      if(delta < bestDelta){
        bestDelta= delta;
        best_i= i;
        best_j= j;
      }
    }
  }
  cout << "best i" << best_i << endl;
  cout << "best j" << best_j << endl;

  if (bestDelta < 0){
    std::swap(s.sequence[best_i], s.sequence[best_j]);
    s.custoSolucao= s.custoSolucao + bestDelta;
    return true;
  }

  return false;
}

bool BestImprovement2Opt (Solution& s){         //estrutura de vizinhança: 2opt 

  double delta, bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double changeCost;

  for(i= 1; i < s.sequence.size() - 1; i++) {
    for (j= i + 2; j < s.sequence.size() - 1; j++){
      
      changeCost= s.custoSolucao - matrizAdj[s.sequence[i-1]][s.sequence[i]] - matrizAdj[s.sequence[j]][s.sequence[j+1]] 
                                 + matrizAdj[s.sequence[i-1]][s.sequence[j]] + matrizAdj[s.sequence[i]][s.sequence[j+1]];

      delta= changeCost - s.custoSolucao;

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
      std::swap(s.sequence[i], s.sequence[j]);
      j--;
    }
    s.custoSolucao= s.custoSolucao + bestDelta;
    
    return true;
  }

  return false; 
}

bool BestImprovementOrOpt (Solution& s, int quantity){   //as 3 outras estruturas de vizinhança 

  double delta, bestDelta= 0;
  int best_i, best_j;
  int i, j;
  double changeCost;

  switch(quantity){
    
    case 1:             //método: REINSERTION

      for(i= 1; i < s.sequence.size() - 1; i++) {
        for (j= i + 1; j < s.sequence.size() - 1; j++){
          
          changeCost= s.custoSolucao - matrizAdj[s.sequence[i]][s.sequence[i-1]] - matrizAdj[s.sequence[i]][s.sequence[i+1]] - matrizAdj[s.sequence[j]][s.sequence[j+1]] 
                                     + matrizAdj[s.sequence[i-1]][s.sequence[i+1]] + matrizAdj[s.sequence[i]][s.sequence[j]] + matrizAdj[s.sequence[i]][s.sequence[j+1]];

          delta= changeCost - s.custoSolucao;

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
      }
      if (bestDelta < 0){
        s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
        s.sequence.erase(s.sequence.begin() + best_i);
        s.custoSolucao= s.custoSolucao + bestDelta;
        
        return true;
      }
      return false;

    case 2:          //método: OR-OPT-2

      for(i= 1; i < s.sequence.size() - 1; i++) {
        for (j= i + 2; j < s.sequence.size() - 2; j++){
          
          changeCost= s.custoSolucao - matrizAdj[s.sequence[i]][s.sequence[i-1]] - matrizAdj[s.sequence[i+1]][s.sequence[i+2]] - matrizAdj[s.sequence[j+1]][s.sequence[j+2]]
                                     + matrizAdj[s.sequence[i-1]][s.sequence[i+2]] + matrizAdj[s.sequence[j+1]][s.sequence[i]] + matrizAdj[s.sequence[i+1]][s.sequence[j+2]];

          delta= changeCost - s.custoSolucao;

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
      }
      if (bestDelta < 0){
        s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i]);
        s.sequence.insert(s.sequence.begin() + best_j + 3, s.sequence[best_i+1]);
        s.sequence.erase(s.sequence.begin() + (best_i + 1));
        s.sequence.erase(s.sequence.begin() + best_i);
        s.custoSolucao= s.custoSolucao + bestDelta;
        
        return true;
      }
      return false;

    case 3:      //método: OR-OPT-3

      for(i= 1; i < s.sequence.size() - 1; i++) {
        for (j= i + 3; j < s.sequence.size() - 3; j++){

          changeCost= s.custoSolucao - matrizAdj[s.sequence[i]][s.sequence[i-1]] - matrizAdj[s.sequence[i+2]][s.sequence[i+3]] - matrizAdj[s.sequence[j+2]][s.sequence[j+3]]
                                     + matrizAdj[s.sequence[i-1]][s.sequence[i+3]] + matrizAdj[s.sequence[j+2]][s.sequence[i]] + matrizAdj[s.sequence[i+2]][s.sequence[j+3]];
          
          delta= changeCost - s.custoSolucao; 

          if(delta < bestDelta){
            bestDelta= delta;
            best_i= i;
            best_j= j;
          }
        }
        
      }
      if (bestDelta < 0){
        s.sequence.insert(s.sequence.begin() + best_j + 3, s.sequence[best_i]);
        s.sequence.insert(s.sequence.begin() + best_j + 4, s.sequence[best_i+1]);
        s.sequence.insert(s.sequence.begin() + best_j + 5, s.sequence[best_i+2]);
        s.sequence.erase(s.sequence.begin() + (best_i + 2));
        s.sequence.erase(s.sequence.begin() + (best_i + 1));
        s.sequence.erase(s.sequence.begin() + best_i);
        s.custoSolucao= s.custoSolucao + bestDelta;
        
        return true;
      }
      
      return false;  
  }
}

void BuscaLocal (Solution& s){

  std::vector<int>NL= {1};
  
  bool improved= false;
  int i;
  while(NL.empty() == false){
    
    int n= rand() % NL.size();
    
    switch (NL[n]) {
      case 1: 
        improved= BestImprovementSwap(s);
        break;
      case 2: 
        //improved= BestImprovement2Opt(s);
        break;
      case 3:
        //improved= BestImprovementOrOpt(s, 1);   //reinsertion
        break;
      case 4:
        //improved= BestImprovementOrOpt(s, 2);   //Or-opt2
        break;
      case 5:
        //improved= BestImprovementOrOpt(s, 3);   //Or-opt3
        break;
    }
    
    cout << endl << "custo " << s.custoSolucao << endl;

    if(improved){
      NL= {1};

    }else{
      NL.erase(NL.begin() + n);
    }
    
  } 
}

Solution Pertubacao (Solution& s){ 

  cout << "entrou na pert" << endl;
  int subseqMax, aux;
  int subseq1, subseq2;
  int index1, index2;
  bool changed= false;
  int i, times;

  srand(time(0));

  if (s.sequence.size()-1 <= 20){
    subseq1= 2;
    subseq2= 2;

  }else{
     subseqMax= (s.sequence.size()-1) / 10.0;
     
     while(1){
      subseq1= rand();

      if(subseq1 >= 2 && subseq1 <= subseqMax){
        break;
      }
     }

     while(1){
      subseq2= rand();
      
      if(subseq2 >= 2 && subseq2 <= subseqMax){
        break;
      }
     }

     if(subseq1 > subseq2){
      aux= subseq1;
      subseq1= subseq2;
      subseq2= aux;

      aux= index1;
      index1= index2;
      index2= aux;

      changed= true;
    }
  }

  while(1){
    index1= rand();

    if(index1 >= 1 && index1 <= s.sequence.size()-subseq2-subseq1){
        break;
    }
  }
  while(1){
    index2= rand();

    if(index2 >= index1+subseq2 && index2 <= s.sequence.size()-subseq2){
        break;    
    }
  }

  for(i= 0; i < subseq1; i++){
      std::swap(s.sequence[index1+i], s.sequence[index2+i]);
  }

  if(subseq1 != subseq2){
    times= subseq2- subseq1;

    i= 0;
    while(times != 0){
      s.sequence.insert(s.sequence.begin() + index1 + subseq1 + i, s.sequence[index2 + subseq1 + i] );

      if(changed){
        s.sequence.erase(s.sequence.begin() + index2 + subseq1 + i);
      }else{
        s.sequence.erase(s.sequence.begin() + index2 + subseq1 + i+1);
      }

      times--;
      i++;
    }
  }
  
}

int main(int argc, char** argv) {

    Solution s, bestS;
    int maxIter, maxIterIls;
    int i, count;

    readData(argc, argv, &dimension, &matrizAdj);


    if(dimension <= 150){
      maxIterIls= dimension/ 2.0;
    }else{
      maxIterIls= dimension;
    }
    srand(time(0));

   maxIter= 50;
   for(i= 0; i < maxIter; i++){
      Construcao(s);

    for(i= 0; i < s.sequence.size(); i++){
      cout << s.sequence[i] << " ";
    }

      if(i == 0){
        bestS= s;
      }
      cout << "custo inicial " << s.custoSolucao << endl;

      count= 0;
      while(count < maxIterIls){
        //BuscaLocal(s);

        cout << "custo final " << s.custoSolucao << endl;
        //if(s.custoSolucao < bestS.custoSolucao){
          bestS= s;
          count= 0;
        //}
        //s= Pertubacao(bestS);
        count++;
      }
      
     
   }

    //printData();
    
    return 0;  

}

void printData() {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}
