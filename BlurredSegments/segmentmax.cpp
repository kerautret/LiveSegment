#include "segmentmax.h"
#include "Mathutils.h" 






SegmentMax::SegmentMax()
{
  debut=-1;
  fin=-1;
  epaisseur=0;
  largeur=0;
  penteMin=0.0;
  penteMax=0.0;
  numero=-1;
  

}


SegmentMax::SegmentMax(int debut, int fin)
{
  this->debut=debut;
  this->fin=fin;
  epaisseur=0;
  largeur=0;
  penteMin=0.0;
  penteMax=0.0;
  numero=-1;
}

SegmentMax::SegmentMax(int debut, int fin, int numero)
{
  this->debut=debut;
  this->fin=fin;
  epaisseur=0;
  largeur=0;
  penteMin=0.0;
  penteMax=0.0;
  this->numero=numero;
}




bool  SegmentMax::isUp( const SegmentMax & s, int tailleContour){
  ImaGene::Mathutils::ModuloComputer mc(tailleContour);
  int b = s.debut;
  return mc.less(b, this->fin) && mc.less(this->debut,b); 
  //return true;
}













