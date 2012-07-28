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
#include "antipodalh.h"


AntiPodalH::AntiPodalH()
{
}

AntiPodalH::AntiPodalH(Point2DinCV v, Line e)
{
	vertex=v;
	edge=e;
}

AntiPodalH::AntiPodalH(Point2DinCV p1, Point2DinCV p2, Point2DinCV p3)
{
	if(p1.getY()<p2.getY())
	   {
	   if(p2.getY()<p3.getY()) {vertex=p2; edge=Line(p1,p3);}
	   else 
	   	{
		if (p1.getY()<p3.getY())
	   	{vertex=p3; edge=Line(p1,p2);}	   
		else
		{vertex=p1; edge=Line(p2,p3);}
		}
	   }
	   else
	   {
	   if(p1.getY()<p3.getY()) {vertex=p1; edge=Line(p2,p3);}
	   else 
	   	{
		if(p2.getY()>p3.getY())
			{vertex=p2; edge=Line(p1,p3);}
		else 
			{vertex=p3; edge=Line(p1,p2);}
		}
	   }
}


const Point2DinCV & AntiPodalH::getVertex() const
{
	return vertex;	
}
const Line & AntiPodalH::getEdge() const
{
	return edge;
}
double AntiPodalH::slope() const
{
	return edge.slope();
}
double AntiPodalH::large() const
{
	Point2DinCV P1,P2,P3;
	P1=edge.getBegin();P2=edge.getEnd();P3=vertex;
	double tg=(double) (P3.getY()-P1.getY())*(P2.getX()-P1.getX());
	if(P2.getY()-P1.getY()==0)
	{
		#ifdef DEBUG
		cout<<"Line is parallel with Ox"<<endl; 
		#endif 
		return 1000000;
	}
	tg=double(tg)/double(P2.getY()-P1.getY());
	double xInsec= P1.getX() + tg;
	return fabs(P3.getX()-xInsec);
}
bool AntiPodalH::isCorrect() const
{	
  long yBegin=getEdge().getBegin().getY();
  long yEnd=getEdge().getEnd().getY();
  long xBegin=getEdge().getBegin().getX();
  long xEnd=getEdge().getEnd().getX();
  long xVertex=getVertex().getX();
  long yVertex=getVertex().getY();
  if((xVertex==xBegin)&&(yVertex==yBegin)) return false;
  if((xVertex==xEnd)&&(yVertex==yEnd)) return false;
  if((xBegin==xEnd)&&(yBegin==yEnd)) return false;
  if((yVertex-yBegin)*(yVertex-yEnd)>0) return false;
  return true;
}

ostream& operator<<(ostream &os, const AntiPodalH & aph)
{
	os<<"AntipodalH: "<<endl;
// 	os<<"Vertex x="<<aph.getVertex().getX()<<" y="<<aph.getVertex().getY()<<endl;
	os<<"Target vertex: x="<<getContent(*aph.getVertex().getPos()).getX()<<" y="<<getContent(*aph.getVertex().getPos()).getY()<<endl;
	os<<"Line: "<<endl;
// 	os<<"Begin: x="<<aph.getEdge().getBegin().getX()<<" y="<<aph.getEdge().getBegin().getY()<<endl;
	os<<"Target begin point: x="<<getContent(*aph.getEdge().getBegin().getPos()).getX()<<" y="<<getContent(*aph.getEdge().getBegin().getPos()).getY()<<endl;
// 	os<<"End: y="<<aph.getEdge().getEnd().getX()<<" y="<<aph.getEdge().getEnd().getY()<<endl;
	os<<"Target end point: x="<<getContent(*aph.getEdge().getEnd().getPos()).getX()<<" y= "<<getContent(*aph.getEdge().getEnd().getPos()).getY()<<endl;
	return os;
}
