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

#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <cstdlib>
//#include <alloc.h>

using namespace std;

class Color
{
protected:
	int r;
	int g;
	int b;
public:
  Color();
  Color(int r, int g, int b);
  int getR() const;
  int getG() const;
  int getB() const;

  ~Color();
  char* export2SVG();

};
#endif
