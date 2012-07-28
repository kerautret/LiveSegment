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
#include "segment.h"
 
Segment::Segment()
{	
  convexhull = 0;
  isMaxRight=false;
}
Segment::Segment(long b, long e)
{
  isMaxRight=false;
  begin=b;
  end=e;
  convexhull = 0;
}
long Segment::getBegin() const
{
	return begin;
}
long Segment::getEnd() const
{
	return end;
}
void Segment::setBegin(long b)
{
	begin=b;
}
void Segment::setEnd(long e)
{
	end=e;
}


/*
bool Segment::getMaxRight()
{
	return isMaxRight;
}
void Segment::setMaxRight(bool b)
{
	isMaxRight=b;
}
*/
/*
ostream& operator<<(ostream& os, Segment& segment)
{
	os<<"Segment: "<<endl;
// 	os<<"Begin "<<bPoint<<" End "<<ePoint<<endl;
	return os;
}
*/

Segment::~Segment()
{
  if ( convexhull != 0 )
    delete convexhull;
}



void Segment::setConvexHull( ConvexHull* ch)
{
  convexhull=ch;
}


const ConvexHull & Segment::getConvexHull() const
{
  return *convexhull;
}


