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
#include "listpoint2d.h"

using namespace std;

ListePoint2D::ListePoint2D()
{
size=0;
}


void ListePoint2D::addPoint2D(Point2D point)
{
points.push_back(point);
size=size+1;
}
    
    
    
Point2D ListePoint2D::getPointI(int i) const
{
Point2D p;
p.setX(points[i].getX());
p.setY(points[i].getY());
return p;
};
    
   
int ListePoint2D::getSize() const
{return size;}

ostream& operator<<(ostream &os, const ListePoint2D &list)
{
	long size=list.getSize();
	for(int i=0;i<size;i++)
	{
		os<<list.getPointI(i);
	}
	return os;
}

ListePoint2D::~ListePoint2D()
{

}

void ListePoint2D::affichePixel(Graphic graphic, Color color)
{
  long size=getSize();
  Point2D point;
  for(int i=0;i<size;i++)
    {
      point=getPointI(i);
      point.affichePixel(graphic, color);
    }
}







void ListePoint2D::affichePixel(Graphic graphic, Color color, int position)
{
	long size=getSize();
	Point2D point;
	if((position<0)||(position>size-1)) {cout<<"Error: input index for affiche pixel is not valid"<<endl;exit(1);}
	point=getPointI(position);
	point.affichePixel(graphic,color);	
}



    
void ListePoint2D::affichePixel(Graphic graphic, Color color, int begin, int end)
{
  long size=getSize();
  Point2D point;
  //if((begin<0)||(end>size-1)) {cout<<"Error: input index for affiche pixel are not valid"<<endl;exit(1);}
  
    
  for(int i=begin;i!=(end+1)%size;i=(i+1)%size)
    {
      point=getPointI(i%size);
      point.affichePixel(graphic, color);
    }
  
  

}
