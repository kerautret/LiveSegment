#ifndef BLURREDSEGMENT_H
#define BLURREDSEGMENT_H


#include "convexhull.h"
#include "point2d.h"
#include <iostream>
#include "pixel.h"

using namespace std;



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
  Pixel getCenter();
  
  ConvexHull *getConvexHull();
  vector<Pixel> getListePixel();
  vector<Point2D> getListePoint2D();
  double getDirection();

  vector<Pixel>  getPrintedPoints();


protected:
  Point2D lastLeftPointAdded;
  Point2D lastRightPointAdded;
  
  
  Point2D point1;
  Point2D point2;
  Point2D point3;
  int nbPoint;
  
};
#endif
