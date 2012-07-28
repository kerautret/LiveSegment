#ifndef BLURREDSEGMENT_H
#define BLURREDSEGMENT_H





#include "convexhull.h"
#include "point2d.h"
#include <iostream>
#include <QPoint>



class BlurredSegment
{
protected:
  double maxThickness;
  ConvexHull *convexhull;
  vector<Point2D> vectorPixel; 
  
  
  
public:
  BlurredSegment();
  BlurredSegment(Point2D p1, Point2D p2, Point2D p3, int maxThickness);
  ~BlurredSegment() ;
  bool addPoint(Point2D p, Direction d);
  bool isSegmentPoint(Point2D point, Direction d );
  double getThickness();
  double getMaxThickness();
  QPoint getCenter();
  
  ConvexHull *getConvexHull();
  vector<QPoint> getListeQPoint();
  vector<Point2D> getListePixel();
  double getDirection();

  vector<QPoint>  getPrintedPoints();



protected:
  Point2D lastLeftPointAdded;
  Point2D lastRightPointAdded;
  
  
  Point2D point1;
  Point2D point2;
  Point2D point3;
  int nbPoint;
  


};
#endif


