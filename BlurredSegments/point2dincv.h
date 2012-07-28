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
#ifndef POINT2DINCV_H
#define POINT2DINCV_H

#include <ostream>
#include "circulator.h"

using namespace std;

class Point2DinCV: public Point2D
{
protected:
	Node* pos;
public:
	Point2DinCV(){};
	Point2DinCV(Node* pos):Point2D(getContent(*pos)){this->pos=pos;}
	Point2DinCV(Point2D p):Point2D(p){};
	Point2DinCV(long x, long y, Node* pos):Point2D(x,y){this->pos=pos;}
	~Point2DinCV(){};
			
	bool isNext(const Point2DinCV & pCV) const;
	bool isLast(const Point2DinCV & pCV) const;
	Node* getPos() const;
	void setPos(Node* pos);

};

ostream& operator<<(ostream & o, const Point2DinCV &pCV);
#endif
