#ifndef SEGMENTMAX_H
#define SEGMENTMAX_H

#include <iostream>

#include "listpoint2d.h"
#include "segment.h"
#include "segmentmax.h"
#include "convexhull.h"
#include "graphic.h"
#include "Mathutils.h"
#include "boiterecouvrementcst.h"

class SegmentMax
{

protected:
  
  
  

public:
  SegmentMax();
  SegmentMax(int debut, int fin);
  SegmentMax(int debut, int fin, int numero);
  bool  isUp(const SegmentMax & s, int tailleContour);
  const Segment* segment;
  




  int debut;
  int fin;
  int numero;
  
  double pente;
  double penteMin;
  double penteMax;
  double epaisseur;
  double largeur;
  
  

};
#endif
