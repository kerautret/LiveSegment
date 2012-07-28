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
#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>

class Point3D
{
protected:
	long x;
	long y;
	long z;
public:
	Point3D();
	Point3D(long x, long y, long z);
	~Point3D();	
	long getX(){return x;}
	long getY(){return y;}
	long getZ(){return z;}
	
};


#endif

