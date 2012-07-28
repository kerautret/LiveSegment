/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                                  *									   *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/

#include "color.h"
#include <stdio.h>

Color::Color()
{
	r=10; 
	g=10;
	b=10;
}

Color::Color(int r, int g, int b)
{
	this->r=r;
	this->g=g;
	this->b=b;
}

int Color::getR() const
{return r;}

int Color::getG() const
{return g;}

int Color::getB() const
{return b;}



Color::~Color()
{
}


char* Color::export2SVG()
{
  char* str;	
  str= (char*) calloc(8,sizeof(char));
  sprintf(str,"#%02x%02x%02x;",r,g,b);
  // 	printf("color: %s",str);
  return str;
}
