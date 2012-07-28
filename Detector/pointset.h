#ifndef POINTSET_H
#define POINTSET_H

#include <QPoint>
#include <vector>
using namespace std;


/** 
 * @class PointSet pointset.h
 * \brief Manipulation of point sets.
 * \author {P. Even}
 */
class PointSet
{

public:

  /** 
   * 
   * \brief  Return the vector of points that belongs to the straight line segment
   *    ended by @param{p1} and @param{p2}
   * \author {B. Kerautret}
   */  
  static vector<QPoint> tracerSegment (QPoint p1, QPoint p2);


  /** 
   * 
   * \brief  Return the vector of points that belongs to the @param{n}th parallel
   *    segment to the straight line segment ended by @param{p1} and @param{p2}.
   * \author {P. Even}
   */  
  static vector<QPoint> tracerSegment (QPoint p1, QPoint p2, int n);

  
  /** 
   * 
   * \brief  Adds vector @param{v2} to vector @param{v1}.
   * \author {B. Kerautret}
   */
  static void fusionVector (vector<QPoint> &v1 , vector<QPoint> &v2);

};

#endif
