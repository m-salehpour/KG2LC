#include <stdio.h>
#include <stdlib.h>
#include "sddapi.h"

int main(int argc, char** argv) {

  SddLiteral var_count = 4;
  int auto_gc_and_minimize = 0;
  SddManager* m = sdd_manager_create(var_count,auto_gc_and_minimize);

  
  
  SddLiteral L = 1, K = 2, P = 3, A = 4;  
  
  
  
  
  
  
  printf("constructing SDD ... ");

  SddNode* alpha = sdd_manager_false(m);
  SddNode* beta;

  //TL****************************************
  beta  = sdd_conjoin(sdd_manager_literal(-L,m)        , sdd_manager_literal(-K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(-L,m)        , sdd_manager_literal(-K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(-L,m)        , sdd_manager_literal(K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);



  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(-K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(-K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(-K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);


  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(L,m)        , sdd_manager_literal(K,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(P,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(A,m)  ,m);

  alpha = sdd_disjoin(alpha,beta,m);









  /*

  //T1****************************************
  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(-2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-12,m) ,m);

  alpha = sdd_disjoin(alpha,beta,m);

  //T2****************************************

  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(-2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);

  //T3**************************************
  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(-2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-12,m) ,m);

  

  alpha = sdd_disjoin(alpha,beta,m);


  //T4**************************************
  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);


  //T5**************************************
  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);


  //T6**************************************
  beta  = sdd_conjoin(sdd_manager_literal(-1,m)        , sdd_manager_literal(2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);

  //T7**************************************
  beta  = sdd_conjoin(sdd_manager_literal(1,m)        , sdd_manager_literal(-2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal( 8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);

  //T8**************************************
  beta  = sdd_conjoin(sdd_manager_literal(1,m)        , sdd_manager_literal(-2,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(3,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-4,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(5,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(6,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(7,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(-8,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(9,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(10,m) ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(11,m)  ,m);
  beta  = sdd_conjoin(beta, sdd_manager_literal(12,m) ,m);


  alpha = sdd_disjoin(alpha,beta,m);
  */


  printf("saving sdd and vtree ... ");
  sdd_save_as_dot("output/sdd.dot",alpha);
  //sdd_vtree_save_as_dot("output/vtree.dot",vtree);
  printf("done\n");

  //sdd_vtree_free(vtree);
  sdd_manager_free(m);

  return 0;
}
