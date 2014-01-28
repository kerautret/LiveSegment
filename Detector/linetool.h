#ifndef LINETOOL_H
#define LINETOOL_H

#include <vector>
#include "pixel.h"
using namespace std;


/** 
 * @class LineTool linetool.h
 * \brief Image straight line segment utility.
 * \author {P. Even and B. Kerautret}
 */
class LineTool
{

public:

  /** 
   * 
   * \brief  Return the vector of pixels on the straight line segment
   *    ended by @param{p1} and @param{p2}
   * \author {B. Kerautret}
   */  
  static vector<Pixel> draw (Pixel p1, Pixel p2);


  /** 
   * 
   * \brief  Return the vector of pixels on the @param{n}th parallel
   *    segment to the straight line segment ended by @param{p1} and @param{p2}.
   * \author {P. Even}
   */  
  static vector<Pixel> draw (Pixel p1, Pixel p2, int n);

  
  /** 
   * 
   * \brief  Adds vector @param{v2} to vector @param{v1}.
   * \author {B. Kerautret}
   */
  static void merge (vector<Pixel> &v1 , vector<Pixel> &v2);

};

#endif
