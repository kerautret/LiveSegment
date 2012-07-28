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
#include "line.h"



Line::Line()
{
}
Line::Line(Point2D b, Point2D e):begin(b),end(e)
{
}
Line::Line(Point2DinCV b, Point2DinCV e)
{
	begin=b;
	end=e;
}

Point2DinCV Line::getBegin() const
{
	return begin;
}
Point2DinCV Line::getEnd() const
{	
	return end;
}
void Line::setBegin(Point2DinCV b)
{
	this->begin=b;
}
void Line::setEnd(Point2DinCV e)
{
	this->end=e;
}
double Line::slope() const
{

  

/*
	Point2D point1, point2;
	point1=this->getBegin();
	point2=this->getEnd();
	long a = point2.getY()- point1.getY();
	long b = point2.getX()- point1.getX();
	double sl;
	if (b!=0) {sl=(double)a/(double)b; return sl;}
	else 
	{
		if(a!=0) return  10000000;//INFINI;
		else {
			//#ifdef DEBUG cout<<"Erreur lorsqu'on calcule le pente du droite: 2 points du droit englobant sont le meme";
		cout<<"Erreur: 2 point du droit englobant sont les memes"<<endl;
		exit(1);
		}
	}
*/
//	double angle,result;
//	double pi=3.1415926;
	Point2D point1, point2;
	point1=this->getBegin();
	point2=this->getEnd();
	
	if ((point1.getX()==point2.getX())&&(point1.getY()==point2.getY())) {cout<<"Erreur: 2 points d'entree sont la meme"<<endl;exit (0);}
	
//   	if (point1.getX()==point2.getX()) {if(point2.getY()>point1.getY())return pi/2; else return 1.5*pi;}
// 	if(point1.getY()==point2.getY()){if(point2.getX()>point1.getX())return 0;else return pi;}
// 	angle= atan(fabs(point2.getY()-point1.getY())/fabs(point2.getX()-point1.getX()));
// 	if((point1.getX()<point2.getX())&&(point1.getY()<point2.getY())) return angle;
// 	if((point1.getX()>point2.getX())&&(point1.getY()<point2.getY())) return (pi - angle);
// 	if((point1.getX()>point2.getX())&&(point1.getY()>point2.getY())) return (angle + pi);
// 	if((point1.getX()<point2.getX())&&(point1.getY()>point2.getY())) return (2*pi-angle);
	return atan2(point2.getY()-point1.getY(),point2.getX()-point1.getX());
	
}





long Line::remain(Point2D M) const
{
	Point2D point1, point2;
	point1=this->getBegin();
	point2=this->getEnd();
	long a = point2.getY()- point1.getY();
	long b = point2.getX()- point1.getX();
	if(a<0) {a=-a; b=-b;}
	if (a>0) return (a*M.getX()-b*M.getY()) ;
 	else {if (b<0) b=-b; return (a*M.getX()-b*M.getY()) ;}      // (a==0)
}



long Line::getA()  const
{
	Point2D point1, point2;
	point1=this->getBegin();
	point2=this->getEnd();
	long a = point2.getY()- point1.getY();
	long b = point2.getX()- point1.getX();	
	if(a<0) {a=-a; b=-b;}
	return a;
}

long Line::getB() const
{
	Point2D point1, point2;
	point1=this->getBegin();
	point2=this->getEnd();
	long a = point2.getY()- point1.getY();
	long b = point2.getX()- point1.getX();	
	if(a<0) {a=-a; b=-b;}
	if((a==0)&&(b<0)) b=-b;
	return b;
}

ostream& operator<<(ostream & o, Line& line)
{
	o<<"Begin poin: x="<<line.getBegin().getX()<<" y="<<line.getBegin().getY()<<endl;
	o<<"End point: x="<<line.getEnd().getX()<<" y="<<line.getEnd().getY()<<endl;
	return o;
}




double angle2Line(const Line & line1, const Line & line2)	
{
	Point2D point1=line1.getBegin();
	Point2D point2=line1.getEnd();
	Point2D point3=line2.getBegin();
	Point2D point4=line2.getEnd();
	long dentaX1=point2.getX()-point1.getX();
	long dentaY1=point2.getY()-point1.getY();
	long dentaX2= point4.getX()-point3.getX();
	long dentaY2=point4.getY()-point3.getY();
	
	return (dentaX1*dentaY2-dentaX2*dentaY1); 	
}

