// 
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

#ifndef UTILE_H
#define UTILE_H

#include "point2d.h"
// #include "line.h"
#include <list>
#include <vector>
#include <cmath>



// #define DEBUG 1

using namespace std;

//typedef list <Point2D> List2D;

enum Direction {LEFT,RIGHT};

struct Node
{
	Point2D content;
	Node* left;
	Node* right;
};

struct NodePtr
{
	Node* leftPtr;
	Node* rightPtr;
};

double curvatureAt(Point2D point1, Point2D point2, Point2D point3);
double algebraCurvatureAt(Point2D point1, Point2D point2, Point2D point3);
double distance(Point2D point1, Point2D point2);




// Fonction centré en P0, isLeft>0 => P2 a gauche de (P0, P1) 

inline long isLeft( Point2D P0, Point2D P1, Point2D P2 )
{
  long formule1=(P1.getX() - P0.getX())*(P2.getY() - P0.getY());
  long formule2=(P2.getX() - P0.getX())*(P1.getY() - P0.getY());
  return formule1 - formule2;
}






inline double curvatureAt(Point2D point1, Point2D point2, Point2D point3)
{
	long dentaX, dentaY;
	double a,b,c,p,pa,pb,pc,S,radius;

	dentaX=point2.getX()-point1.getX();
	dentaY=point2.getY()-point1.getY();
	a=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));
	
	dentaX=point2.getX()-point3.getX();
	dentaY=point2.getY()-point3.getY();
	b=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));

	dentaX=point3.getX()-point1.getX();
	dentaY=point3.getY()-point1.getY();
	c=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));
	
	p=a+b+c; pa=b+c-a;
	pb=a+c-b; pc=a+b-c;
	//if (p*pa*pb*pc<0)
	//  cout <<"heinhe"<< (p*pa*pb*pc) <<endl;
	S=sqrt(p*pa*pb*pc);
	if(a==0||b==0||c==0||S==0.0)
	  return 0.0;
	radius=double (a*b*c/S);
	//cout << "sq  " <<a << " " << b << " " << c << " "<< pc<< endl;//p*pa*pb*pc; 
	
	
	return 1.0/radius;
}




inline double algebraCurvatureAt(Point2D point1, Point2D point2, Point2D point3)
{
	long dentaX, dentaY;
	double a,b,c,p,pa,pb,pc,S,radius;
//	double angle;
//	double pi=3.1415926;

	dentaX=point2.getX()-point1.getX();
	dentaY=point2.getY()-point1.getY();
	a=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));
	
	dentaX=point2.getX()-point3.getX();
	dentaY=point2.getY()-point3.getY();
	b=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));

	dentaX=point3.getX()-point1.getX();
	dentaY=point3.getY()-point1.getY();
	c=sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));
	
	p=a+b+c; pa=b+c-a;
	pb=a+c-b; pc=a+b-c;
	S=sqrt(p*pa*pb*pc);
	radius=double (a*b*c/S);
	/*
	Line line1=Line(point2,point1);
	Line line2=Line(point2,point3);
	double angle=line2.slope()-line1.slope();
	if(angle<pi) return 1/radius;
	else return (-1/radius);
	*/
	return 1/radius;
	
// 	if(angle>0) return 1/radius;
// 	else	return
	
}


inline double distance(Point2D point1, Point2D point2)
{
	long dentaX=point2.getX()-point1.getX();
	long dentaY=point2.getY()-point1.getY();
	return sqrt(pow((double)dentaX,2)+pow((double)dentaY,2));
}









  












/*
inline double curvatureAt(Point2D point1, Point2D point2, Point2D point3)
{
	double a=distance(point1,point2);
	double b=distance(point3,point2);
	double c=distance(point1,point3);
	double p=a+b+c;
	double pa=b+c-a;
	double pb=a+c-b;
	double pc=a+b-c;
	double S=sqrt(p*pa*pb*pc);
	double radius=double(a*b*c/S);
	
 	return (1/radius);
}
*/

#endif
