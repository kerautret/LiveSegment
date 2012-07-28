
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

 #include "point3d.h"
 
 
 Point3D::Point3D()
 {
 }
 
 Point3D::Point3D(long x, long y, long z)
 {
 	this->x=x;
	this->y=y;
	this->z=z;
 }
 
 Point3D::~Point3D()
 {
 }
