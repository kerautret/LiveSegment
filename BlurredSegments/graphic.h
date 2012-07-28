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
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>

using namespace std;

class Graphic
{
protected:
	long startX;
	long startY;
	int coeAmpleur;
	char* filename;
public:
	Graphic();
	Graphic(long startX, long startY, long ampleur);
	Graphic(long startX, long startY, long ampleur, char* file);
	long getStartX();
	long getStartY();
	int getAmpleur();
	void setStartX(long startX);
	void setStartY(long startY);
	void setAmpleur(int ampleur);
	char* getFilename();
	void setFilename(char* filename);
	void print();
	~Graphic();
};

#endif


