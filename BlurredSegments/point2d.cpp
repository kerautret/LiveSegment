/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                      *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/


#include "point2d.h"
#include <fstream>

//using namespace std;

Point2D::Point2D()
{
color=1;
}

Point2D::Point2D(long x, long y)
{
	this->x=x;
	this->y=y;

}




Point2D::~Point2D()
{

}


long Point2D::getX() const
    {return x;}

    
long Point2D::getY() const
    {return y;}

int Point2D::getColor() const
    {return color;}
    

        
ostream& operator<<(ostream& os, const Point2D & p)
    {
      os <<"Point: x=";
      os<<p.getX()<<" y="<<p.getY()<<endl;    
      return os;
    }     







void Point2D::affichePixel(Graphic graphic,Color color) 
{
  affichePixel(graphic, color.getR(), color.getG(), color.getB());
}

void Point2D::affichePixelPoint(Graphic graphic,Color color) 
{
  affichePixelPoint(graphic, color.getR(), color.getG(), color.getB());
}




void Point2D::affichePixelPoint(Graphic graphic, int r, int g, int b)
{
  //Declaration des variables
  fstream flux;
  
  //Initialisation des parametres
  char* nomFichierSvg= graphic.getFilename();
  int denta=graphic.getAmpleur();
//  long startX=graphic.getStartX();
//  long startY=graphic.getStartY();
  long  x=this->getX()*denta;
  long  y=this->getY()*denta;
  

  flux.open (nomFichierSvg,ios::in| ios::out);
  flux.seekp(-0,ios::end);
  flux<<endl;
  flux<<x <<" "<< y << " " << denta << " " << denta << "  " << r << " "<< g << " "<< b << " drawCroix";
  flux<<endl;	
  
}




void Point2D::afficheLine(Graphic graphic, Color color, Point2D p)
{
  fstream flux;
  int denta=graphic.getAmpleur();
  char* nomFichierSvg= graphic.getFilename();
  flux.open (nomFichierSvg,ios::in| ios::out);

  flux.seekp(-0,ios::end);
  flux<<endl;
  flux<<x*denta+denta/2 <<" "<< y*denta+denta/2 << " " << p.getX()*denta+denta/2 << " " << p.getY()*denta+denta/2 << "  " << color.getR() << " "<< color.getG() << " "<< color.getB() << " line";
  flux<<endl;	
  

}




    
void Point2D::affichePixel(Graphic graphic,int r, int g,int b) 
{
  //Declaration des variables
  fstream flux;
  
  //Initialisation des parametres
  char* nomFichierSvg= graphic.getFilename();
  int denta=graphic.getAmpleur();
//  long startX=graphic.getStartX();
//  long startY=graphic.getStartY();
  long  x=this->getX()*denta;
  long  y=this->getY()*denta;
  
  
  flux.open (nomFichierSvg,ios::in| ios::out);
  flux.seekp(-0,ios::end);
  flux<<endl;
  flux<<x <<" "<< y << " " << denta << " " << denta << "  " << r << " "<< g << " "<< b << " drawPix";
  flux<<endl;	
  
}








