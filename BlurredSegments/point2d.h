/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                                       *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/
 
#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>
#include "graphic.h"
#include "color.h"

using namespace std;

class Point2D{
protected:
    long x;
    long y;
    int color; 
 
public:
    Point2D();
    Point2D(long x, long y);
    ~Point2D();
    
    int getColor() const; 
    long getX() const;
    long getY() const;
  void setColor(int c){color=c;};
  void setXY(long X, long Y) {x=X; y=Y;};
  void setX(long X){x=X;};
  void setY(long Y){y=Y;};
  void affichePixel(Graphic graphic, int r, int g, int b);   
  void affichePixelPoint(Graphic graphic, int r, int g, int b);   
  void affichePixelPoint(Graphic graphic, Color color);   
  void affichePixel(Graphic graphic, Color color);   
  void afficheLine(Graphic graphic, Color color, Point2D p);
    friend ostream& operator<<(ostream& os, const Point2D & p);        
};

#endif
