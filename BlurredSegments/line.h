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
#ifndef LINE_H
#define LINE_H

#include <cstdlib>
#include <ostream>
#include <cmath>
#include "point2dincv.h"

class Line
{
protected:
	Point2DinCV begin;
	Point2DinCV end;
public:
	Line();
	Line(Point2D b, Point2D e);
	Line(Point2DinCV b, Point2DinCV e);
	double slope() const;
	Point2DinCV getBegin() const;
	Point2DinCV getEnd() const;
	void setBegin(Point2DinCV b);
	void setEnd(Point2DinCV e);
	long remain(Point2D M) const;
	long getA() const;
	long getB() const;
	friend ostream& operator<<(ostream & o, Line& line);
	friend double angle2Line(const Line & line1, const Line & line2);
};

#endif
