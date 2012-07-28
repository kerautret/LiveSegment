
#ifndef BOITERECOUVREMENTCST_H
#define BOITERECOUVREMENTCST_H

#include <iostream>
#include "graphic.h"



using namespace std;

class BoiteRecouvrementCst{


protected:
  



public:
  int debut;
  int fin;
  
  
  
  double angleMin;
  double angleMax;
  double  angleTheta1;
  double  angleTheta2;
  double longueur;
  

  
  BoiteRecouvrementCst(int debut, int fin);
  BoiteRecouvrementCst();
  
  

};  
#endif

