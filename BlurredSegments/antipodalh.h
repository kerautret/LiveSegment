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
#ifndef ANTIPODALH_H
#define ANTIPODALH_H

#include "point2d.h"
#include "line.h"
#include <ostream>
#include <math.h>

using namespace std;

class AntiPodalH
{
protected:
	Point2DinCV vertex;
	Line edge;
public:
	AntiPodalH();
	AntiPodalH(Point2DinCV p1, Point2DinCV p2, Point2DinCV p3);
	AntiPodalH(Point2DinCV v, Line e);
	const Point2DinCV & getVertex() const;
	const Line & getEdge() const;
	double slope() const;
	double large() const;
	void setEdge(Line line){edge=line;}
	void setVertex(Point2DinCV v){vertex=v;}
	bool isCorrect() const;
};

ostream& operator<<(ostream &os, const AntiPodalH & aph);

#endif
