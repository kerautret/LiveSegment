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
#include "antipodalv.h"

AntiPodalV::AntiPodalV(Point2DinCV v, Line e)
{
	vertex = v;
	edge = e;
}

AntiPodalV::AntiPodalV()
{
}

AntiPodalV::AntiPodalV(Point2DinCV p1, Point2DinCV p2, Point2DinCV p3)
{
	if(p1.getX()<p2.getX())
	   {
	   if(p2.getX()<p3.getX()) {vertex=p2; edge=Line(p1,p3);}
	   else 
	   	{
		if (p1.getX()<p3.getX())
	   	{vertex=p3; edge=Line(p1,p2);}	   
		else
		{vertex=p1; edge=Line(p2,p3);}
		}
	   }
	   else
	   {
	   if(p1.getX()<p3.getX()) {vertex=p1; edge=Line(p2,p3);}
	   else 
	   	{
		if(p2.getX()>p3.getX())
			{vertex=p2; edge=Line(p1,p3);}
		else 
			{vertex=p3; edge=Line(p1,p2);}
		}
	   }
}

const Point2DinCV & AntiPodalV::getVertex() const
{
	return vertex;
}
const Line & AntiPodalV::getEdge() const
{
	return edge;
}
double AntiPodalV::slope() const
{
	return edge.slope();
}

/*
Determine height of antipodal
*/
double AntiPodalV::height() const
{
	Point2DinCV point1,point2,point3;
	point1 = edge.getBegin();
	point2 = edge.getEnd();
	point3 = vertex;
	double ts= (double) (point2.getY()-point1.getY())*(point3.getX()-point1.getX());
	double ms=point2.getX()-point1.getX();
	if(ms==0)
	{
		#ifdef DEBUG
		cout<<"Line is parallel with Oy"<<endl;
		#endif
		return 1000000;
	}
	double yInsec= (double)ts/double(ms);		
	double height=point3.getY()-point1.getY()-yInsec;
	return fabs(height);
}

bool AntiPodalV::isCorrect() const
{	
	long xBegin=getEdge().getBegin().getX();
	long xEnd=getEdge().getEnd().getX();
	long yBegin=getEdge().getBegin().getY();
	long yEnd=getEdge().getEnd().getY();
	long xVertex=getVertex().getX();
	long yVertex=getVertex().getY();
	if((xVertex==xBegin)&&(yVertex==yBegin)) return false;
	if((xVertex==xEnd)&&(yVertex==yEnd)) return false;	
	if((xBegin==xEnd)&&(yBegin=yEnd))return false;
	if((xVertex-xBegin)*(xVertex-xEnd)>0) return false;
	return true;
}

/*
Affiche l'information de cette antipodal
*/
ostream& operator<<(ostream& os, AntiPodalV &apv)
{
	os<<"Vertex: x="<<apv.getVertex().getX()<<" y="<<apv.getVertex().getY()<<endl;	
// 	os<<"Linked vertex: x="<<getContent(*apv.getVertex().getPos()).getX()<<" y="<<getContent(*apv.getVertex().getPos()).getY()<<endl;
	os<<"Line: "<<endl;
	os<<"Begin point: x="<<apv.getEdge().getBegin().getX()<<" y="<<apv.getEdge().getBegin().getY()<<endl;
// 	os<<"Linked begin point: x="<<getContent(*apv.getEdge().getBegin().getPos()).getX()<<" y="<<getContent(*apv.getEdge().getBegin().getPos()).getY()<<endl;
	os<<"End point x="<<apv.getEdge().getEnd().getX()<<" y="<<apv.getEdge().getEnd().getY()<<endl;
// 	os<<"Linked end point: x="<<getContent(*apv.getEdge().getEnd().getPos()).getX()<<" y= "<<getContent(*apv.getEdge().getEnd().getPos()).getY()<<endl;
	return os;
}
