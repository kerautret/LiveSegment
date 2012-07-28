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
#include "point2dincv.h"

Node* Point2DinCV::getPos() const
{
	return pos;
}
void Point2DinCV::setPos(Node* pos)
{
	this->pos=pos;
}

bool Point2DinCV::isNext(const Point2DinCV & pCV) const
{
	Node* next=getNext(getPos());
	if(next==pCV.getPos())return true;
	else return false;
}
bool Point2DinCV::isLast(const Point2DinCV & pCV) const
{
	Node* last=getLast(getPos());
	if(last==pCV.getPos())return true;
	else return false;
}

ostream& operator<<(ostream & o, const Point2DinCV &pCV)
{

	o<<"Real point: x="<<pCV.getX()<<" y="<<pCV.getY()<<endl;
	o<<"Target point: x="<<getContent( (*pCV.getPos())).getX()<<" y="<<getContent((*pCV.getPos())).getY()<<endl;
	return o;
}
